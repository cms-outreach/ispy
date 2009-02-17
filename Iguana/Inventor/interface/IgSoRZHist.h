#ifndef IG_OPEN_INVENTOR_IG_SO_RZ_HIST_H
# define IG_OPEN_INVENTOR_IG_SO_RZ_HIST_H

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

class IG_OPEN_INVENTOR_API IgSoRZHist: public IgSoShapeKit
{
    SO_KIT_HEADER (IgSoRZHist);
    SO_KIT_CATALOG_ENTRY_HEADER (shapeHints);
    SO_KIT_CATALOG_ENTRY_HEADER (faceSet);
    SO_KIT_CATALOG_ENTRY_HEADER (lines); 

    SO_KIT_CATALOG_ENTRY_HEADER (ruler);
    
  
    
public:
    IgSoRZHist (void);
    static void initClass (void);
    SoSFFloat 	minRadius;
    SoSFFloat 	maxRadius;
    SoSFBool  	upper ;
    SoMFFloat  	energies;
    SoSFBool 	logScale;
    SoSFFloat 	layer;    
    SoSFFloat 	scaleFactor;
    SoSFBool 	showAnnotations;
    SoSFFloat 	deltaEta;
    
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

protected:
    virtual void refresh (void);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

# ifdef WIN32
#  include <SoWinEnterScope.h>
# endif

#endif // IG_OPEN_INVENTOR_IG_SO_RZ_HIST_H

