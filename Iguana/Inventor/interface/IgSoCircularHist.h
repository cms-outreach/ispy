#ifndef IG_OPEN_INVENTOR_IG_SO_CIRCULARHIST_H
# define IG_OPEN_INVENTOR_IG_SO_CIRCULARHIST_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Inventor/interface/IgSoShapeKit.h"
# include <Inventor/fields/SoSFFloat.h>
# include <Inventor/fields/SoSFInt32.h>
# include <Inventor/fields/SoSFBool.h>
# include <Inventor/fields/SoMFFloat.h>

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
 *  \class IgSoCircularHist IgSoCircularHist.h
 *
 *  \brief Creates an OpenInventor representation for a circular histogram, histogram values are supposed to be positiv
 *	
 *  \author Alexandra Junghans, CMS, CERN Summerstudent Program
 *  \date  25 Jul - 22 Aug 2005
 *
 */
class IG_OPEN_INVENTOR_API IgSoCircularHist : public IgSoShapeKit
{
    SO_KIT_HEADER (IgSoCircularHist);
    SO_KIT_CATALOG_ENTRY_HEADER (shapeHints);
    SO_KIT_CATALOG_ENTRY_HEADER (faceSet);
    SO_KIT_CATALOG_ENTRY_HEADER (lines);     

    SO_KIT_CATALOG_ENTRY_HEADER (ruler);
//     SO_KIT_CATALOG_ENTRY_HEADER (material);
//     SO_KIT_CATALOG_ENTRY_HEADER (yrot);
//     SO_KIT_CATALOG_ENTRY_HEADER (zrot);    
//     SO_KIT_CATALOG_ENTRY_HEADER (ytrans);
//     SO_KIT_CATALOG_ENTRY_HEADER (ruler);
  
public:
    IgSoCircularHist (void);
    static void initClass (void);
    SoSFFloat 	minRadius;
    SoSFFloat 	maxRadius;
    SoSFInt32 	numberOfBins ;	
    SoMFFloat 	energies;
    SoSFBool 	logScale;
    SoSFFloat 	layer;    
    SoSFFloat 	scaleFactor;
    SoSFBool 	showAnnotations;

protected:
    virtual void refresh (void);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

# ifdef WIN32
#  include <SoWinEnterScope.h>
# endif
#endif // IG_OPEN_INVENTOR_IG_SO_CIRCULARHIST_H
