#ifndef IG_OPEN_INVENTOR_IG_SO_ROT_SOLID_H
# define IG_OPEN_INVENTOR_IG_SO_ROT_SOLID_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Inventor/interface/IgSoShapeKit.h"
# include <Inventor/fields/SoSFFloat.h>
# include <Inventor/fields/SoSFInt32.h>
# include <Inventor/fields/SoSFBool.h>
# include <Inventor/fields/SoMFVec2f.h>
# include <vector>
# ifdef WIN32
#  include <SoWinLeaveScope.h>
# endif

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/*! 
 *  \class IgSoRotSolid IgSoRotSolid.h
 *
 *  \brief This class creates an OpenInventor shape for a rotational solid
 *  defined by a rotation of a cross section which is specified by a generic 
 *  polygon.
 *
 *  The rotation is defined from phiStart to a total of the phiDelta angle.
 *
 */
class IG_OPEN_INVENTOR_API IgSoRotSolid : public IgSoShapeKit
{
    SO_KIT_HEADER (IgSoRotSolid);
    SO_KIT_CATALOG_ENTRY_HEADER (hints);
    SO_KIT_CATALOG_ENTRY_HEADER (faces);
    SO_KIT_CATALOG_ENTRY_HEADER (lines);

public:
    IgSoRotSolid (void);
    static void initClass (void);

    SoSFFloat 	phiStart;		//< Start angle for the rotation.
    SoSFFloat 	phiDelta;		//< Angle of rotation.
    SoSFInt32	divisions;    		//< Number of sides in phiDelta.
    SoMFVec2f 	polygon; 		//< Points defining the rotational polygon.   
    SoSFBool	smooth;    
    SoSFBool	showLines;    

    
    void	makePgon (const std::vector<float> &zvals, 
			  const std::vector<float> &rmin, 
			  const std::vector<float> &rmax,
			  float phiStart = 0, 
			  float phiDelta = 2*M_PI,
			  int divisions = 5);

    void	makePcon (const std::vector<float> &zvals, 
			  const std::vector<float> &rmin, 
			  const std::vector<float> &rmax,
			  float phiStart = 0, 
			  float phiDelta = 2*M_PI,
			  int divisions = -1);

    void	makeCons (float rmin1, 
			  float rmax1,
			  float rmin2, 
			  float rmax2,
			  float dz,
			  float phiStart = 0, 
			  float phiDelta = 2*M_PI,
			  int divisions = -1);

    void	makeTubs (float rmin, 
			  float rmax,
			  float dz,
			  float phiStart = 0, 
			  float phiDelta = 2*M_PI,
			  int divisions = -1);

protected:
    virtual void refresh (void);
private:
    void cleanParts (void);    
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

# ifdef WIN32
#  include <SoWinEnterScope.h>
# endif
#endif // IG_OPEN_INVENTOR_IG_SO_ROT_SOLID_H
