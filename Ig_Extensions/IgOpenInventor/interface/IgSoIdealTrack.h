#ifndef IG_OPEN_INVENTOR_IG_SO_IDEAL_TRACK_H
# define IG_OPEN_INVENTOR_IG_SO_IDEAL_TRACK_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Extensions/IgOpenInventor/interface/IgSoShapeKit.h"
# include <Inventor/fields/SoSFVec3f.h>
# include <Inventor/fields/SoSFFloat.h>
# include <Inventor/fields/SoSFString.h>
# ifdef WIN32
#  include <SoWinLeaveScope.h>
# endif

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgParticleChar;
class SbVec2f;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/*! \class IgSoIdealTrack IgSoIdealTrack.h Ig_Extensions/IgOpenInventor/interface/IgSoIdealTrack.h
 *  \brief Represents an ideal track (i.e., a perfect helix) as a NURB
 *
 *  \author G. Alverson
 *
 *  IgSoIdealTrack is a dynamic kit in that it checks against a global time to 
 *  determine how it represents a track. As the time changes, the track grows to 
 *  match.
 *
 *  Times are measured in 0.01 nanosecond ticks, magnetic fields in Tesla, and distances in meters.
 */
class IG_OPEN_INVENTOR_API IgSoIdealTrack : public IgSoShapeKit
{
    SO_KIT_HEADER (IgSoIdealTrack);
    SO_KIT_CATALOG_ENTRY_HEADER (material);
    SO_KIT_CATALOG_ENTRY_HEADER (style);
    SO_KIT_CATALOG_ENTRY_HEADER (complexity);
    SO_KIT_CATALOG_ENTRY_HEADER (points);
    SO_KIT_CATALOG_ENTRY_HEADER (curve);
    SO_KIT_CATALOG_ENTRY_HEADER (debugStyle);
    SO_KIT_CATALOG_ENTRY_HEADER (debugPoints);

public:
    IgSoIdealTrack (void);
    static void initClass (void);

    // Track properties
    SoSFFloat	phi;		//< Dip angle od helix  wrt z axis
    SoSFFloat	radius;		//< Radius of curvature
    SoSFFloat	zeta;		//< Radius/tan (theta), theta being the dip angle
    SoSFVec3f	vertex;		//< Vertex position
    SoSFFloat	tstart;		//< time at beginning of track (not necessarily visible)    
    SoSFFloat	tend;		//< time at end of track (not necessarily visible)
    SoSFString	particleType;	//< Name follow IgParticleChar conventions

    // Display parameters
    SoSFFloat	t0;		//< time at beginning of visible track
    SoSFFloat	dt;		//< timespan of visible track


    // Helper methods to initialise track properties
    void		initialise (double *vx, double *vy, double *vz, 
				    double *px, double *py, double *pz, 
				    float *t0, IgParticleChar *pc);
    void		initialise (double vx, double vy, double vz, 
				    double px, double py, double pz, 
				    float t0, IgParticleChar *pc);
    void		initialise (double vx, double vy, double vz, 
				    double px, double py, double pz, 
				    float t0, int pid);
    // methods to initialise environmental variables
    static void		setBField (float val);
    static void		setRMax (float val);
    static void		setZMax (float val);

    // convenience methods to get physics parameters
    float		ptot (void) const;
    float		pt (void) const;

protected:
    virtual void	refresh (void);
    
protected:
    void		initEndPts (void);
    float		timeToAngle (float time);
    float		angleToTime (float angle);
    float		zToTime (float z);
    float		timeToZ (float time);
    SbVec2f		timeToXY (float angle);

    IgParticleChar	*m_particleChar;
    float		m_ptot;   //total momentum
    float		m_pt;     //transverse momentum
    float		m_charge;
private:
    // Environment parameters
    static float	bfield;		//< constant bfield in tesla
    static float	rmax;		//< max radius in central tracker
    static float	zmax;		//< max z extent

};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
inline void IgSoIdealTrack::setBField (float val) { bfield = val; }
inline void IgSoIdealTrack::setRMax   (float val) { rmax = val; }
inline void IgSoIdealTrack::setZMax   (float val) { zmax = val; }
inline float IgSoIdealTrack::ptot      (void) const { return m_ptot; }
inline float IgSoIdealTrack::pt	      (void) const { return m_pt; }
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

# ifdef WIN32
#  include <SoWinEnterScope.h>
# endif
#endif // IG_OPEN_INVENTOR_IG_SO_IDEAL_TRACK_H
