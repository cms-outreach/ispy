#ifndef IG_OPEN_INVENTOR_IG_PARTICLE_CHAR_H
# define IG_OPEN_INVENTOR_IG_PARTICLE_CHAR_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Extensions/IgOpenInventor/interface/config.h"
# include <Inventor/nodes/SoMaterial.h>
# include <Inventor/nodes/SoDrawStyle.h>
# include <map>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class  IgParticleChar;

/*!  \class IgParticleChar IgParticleChar.h Ig_Extensions/IgOpenInventor/interface/IgParticleChar.hpp
 *   \brief Provides a mini-database for particle characteristics
 *
 *   This class maintains a small database of standard particle names,
 *   masses, charges, Monte Carlo IDs (PYTHIA, ISAJET, GEANT), as well
 *   as the OpenInventor characteristics which could be used to represent
 *   pure particle tracks: material and line type.
 *
 *   Each IgParticleChar maintains both a name and a short tag suitable for use
 *   in a display.
 *
 * 
 *   The standard particles loaded into the database are:
 *   unknown, antiunknown, electron, positron, muon, antimuon, proton, antiproton,
 *   neutron, antineutron, pion, antipion, deuteron, tritium, alpha, and he3.
 *   Particles are taken to have negative charge, anti-particles have positive.
 *   Neutrals haven't been included yet.
 *
 */
class IG_OPEN_INVENTOR_API IgParticleChar
{
public:
    IgParticleChar (int geantCode, int pythiaCode, int isaCode, const char *name,
		    const char *label, float mass, float charge);

    void		setMaterial (SoMaterial* particleMaterial);
    void		setStyle (SoDrawStyle* particleStyle);

    const char *	getName () const;
    float		getMass () const;
    float		getCharge () const;

    int			getGeantCode () const;
    SoMaterial *	getMaterial () const;
    SoDrawStyle *	getStyle () const;

    static void		initParticles ();
    static void		deleteParticles ();
    static IgParticleChar *getByName (const char* string);
    static IgParticleChar *getByGeantID (const int p_code);
    static IgParticleChar *getByPythiaID (const int p_code);
    static IgParticleChar *getByIsajetID (const int p_code);
    static void		dumpDB (std::ostream & os);

    friend std::ostream &operator<< (std::ostream &os, const IgParticleChar &pc);

protected:
    void		print (std::ostream & os) const;

private:
    typedef std::map< std::string, IgParticleChar*> CMapToParticleList;
    const int geantCode;
    const int pythiaCode;
    const int isajetCode; 
    SoMaterial* particleMaterial;
    SoDrawStyle* particleStyle;
    const SbString name;
    const SbString label;
    static CMapToParticleList* particleList;
    const float mass;
    const float charge;
    static SbBool inited; 
    ~IgParticleChar ();
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

//! accessor for GEANT code 
inline int
IgParticleChar::getGeantCode () const
{ return geantCode; }

//! accessor for the SoMaterial used to represent the particle
inline SoMaterial *
IgParticleChar::getMaterial () const
{ return particleMaterial; }

//! accessor for the SoDrawStyle used to represent the particle
inline SoDrawStyle *
IgParticleChar::getStyle () const
{ return particleStyle; 	}

//! extractor for output
inline std::ostream &
operator<< (std::ostream & os, const IgParticleChar &pc)
{
    pc.print (os);
    return os;
}

#endif // IG_OPEN_INVENTOR_IG_PARTICLE_CHAR_H
