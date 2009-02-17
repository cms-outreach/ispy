#ifndef IG_OPEN_INVENTOR_IG_PARTICLE_CHAR_H
# define IG_OPEN_INVENTOR_IG_PARTICLE_CHAR_H

//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Inventor/interface/config.h"
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <map>
#include <ostream>

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
 *
 *   Note that several "particles" may have the same code: GEANT only has one neutrino
 *   type while Isajet and Pythia have the full set.
 *
 */
class IG_OPEN_INVENTOR_API IgParticleChar
{
public:
	//! create a particleChar with the given characteristics
    IgParticleChar (int geantCode, int pythiaCode, int isaCode, const char *name,
		    const char *label, float mass, float charge);

	//! sets the SoMaterial used when drawing a particle of this type
	void		setMaterial (SoMaterial* particleMaterial);
	//! sets the SoStyle used when drawing a particle of this type
    void		setStyle (SoDrawStyle* particleStyle);

	//! gets the pretty (long) name of the particle
	const char *	getName () const;
    float		getMass () const;
    float		getCharge () const;


	int			getGeantCode () const;
    int			getIsajetCode () const;
    int			getPythiaCode () const;

	//! accessor for the SoMaterial used to represent the particle
    SoMaterial *	getMaterial () const;

	//! accessor for the SoDrawStyle used to represent the particle
    SoDrawStyle *	getStyle () const;

	//! initialise the internal database with the standard particle set
	static void		initParticles ();
	//! zero out the internal database of the standard particle set
    static void		deleteParticles ();

    static IgParticleChar *getByName (const char* string);

	//! accessor for GEANT code: note that this is not guaranteed to be unique 
    static IgParticleChar *getByGeantID (const int p_code);
    static IgParticleChar *getByPythiaID (const int p_code);
    static IgParticleChar *getByIsajetID (const int p_code);

	//! print out the entire database
    static void		dumpDB (std::ostream & os);

	//! extractor for output
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

inline int
IgParticleChar::getGeantCode () const
{ return geantCode; }

inline SoMaterial *
IgParticleChar::getMaterial () const
{ return particleMaterial; }

inline SoDrawStyle *
IgParticleChar::getStyle () const
{ return particleStyle; 	}

inline std::ostream &
operator<< (std::ostream & os, const IgParticleChar &pc)
{
    pc.print (os);
    return os;
}

#endif // IG_OPEN_INVENTOR_IG_PARTICLE_CHAR_H
