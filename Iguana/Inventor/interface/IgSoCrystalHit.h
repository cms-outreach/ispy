#ifndef IG_OPEN_INVENTOR_IG_SO_CRYSTAL_HIT_H
# define IG_OPEN_INVENTOR_IG_SO_CRYSTAL_HIT_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Inventor/interface/IgSoShapeKit.h"
# include <Inventor/fields/SoSFFloat.h>
# include <Inventor/fields/SoSFBool.h>
# include <Inventor/fields/SoSFVec3f.h>
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
 *	\class IgSoCrystalHit IgSoCrystalHit.h
 *	\brief Draws a hit in an individual crystal specified by:
 *
 *    The crystal is drawn as a trapezoid and the energy is represented by 
 *    a 3D trapezoidal "histogram bar" extending outwards from the back face  
 *    of the crystal a distance which is proportional to the energy.
 *
 *    Original version for HEPVis.
 *
 *	\author L. Taylor, CMS/Northeastern University Software Group
 */
class IG_OPEN_INVENTOR_API IgSoCrystalHit : public IgSoShapeKit
{
    SO_KIT_HEADER (IgSoCrystalHit);
    SO_KIT_CATALOG_ENTRY_HEADER (shapeHints);
    SO_KIT_CATALOG_ENTRY_HEADER (faces);
    SO_KIT_CATALOG_ENTRY_HEADER (lines);

public:
    IgSoCrystalHit (void);
    static void initClass (void);

    SoSFFloat energy;		//< Energy (in GeV)
    SoSFFloat minimumEnergy;	//< Minimum energy to draw (in GeV)
    SoSFFloat scale;		//< Scale for energy (GeV / unit distance)
    SoSFFloat relativeWidth;	//< Width of energy bump relative to crystal [0,1]

    SoSFBool  drawCrystal;	//< Draw crystal (if TRUE)
    SoSFBool  drawHit;		//< Draw hit     (if TRUE)
    SoSFBool  showLines;
 
    SoSFVec3f front1;		//< corner 1 of front face	    
    SoSFVec3f front2;		//< corner 2 of front face    
    SoSFVec3f front3;		//< corner 3 of front face    
    SoSFVec3f front4;		//< corner 4 of front face    
	        	
    SoSFVec3f back1;		//< corner 1 of back face	        
    SoSFVec3f back2;		//< corner 2 of back face    
    SoSFVec3f back3;		//< corner 3 of back face    
    SoSFVec3f back4;		//< corner 4 of back face    
    					
protected:
    virtual void refresh (void);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

# ifdef WIN32
#  include <SoWinEnterScope.h>
# endif
#endif // IG_OPEN_INVENTOR_IG_SO_CRYSTAL_HIT_H
