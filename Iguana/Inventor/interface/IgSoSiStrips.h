#ifndef IG_OPEN_INVENTOR_IG_SO_SI_STRIPS_H
# define IG_OPEN_INVENTOR_IG_SO_SI_STRIPS_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Inventor/interface/IgSoShapeKit.h"
# include <Inventor/fields/SoSFFloat.h>
# include <Inventor/fields/SoMFShort.h>
# ifdef WIN32
#  include <SoWinLeaveScope.h>
# endif

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/*! \class            IgSoSiStrips
 * 
 * \brief     Represents a single-sided silicon strip detector
 * 
 * \author	    Joe Boudreau 
 * \date	    Nov 11 1996
 */
class IG_OPEN_INVENTOR_API IgSoSiStrips : public IgSoShapeKit
{
    SO_KIT_HEADER (IgSoSiStrips);
    SO_KIT_CATALOG_ENTRY_HEADER (translation);
    SO_KIT_CATALOG_ENTRY_HEADER (pulses);

public:
    IgSoSiStrips (void);
    static void initClass (void);

    SoSFFloat length;		//< Strip length
    SoSFFloat width;		//< Strip width
    SoSFFloat thickness;	//< Strip thickness
    SoSFFloat pitch;		//< Strip pitch (angle)
    SoSFFloat offset;		//< Strip offset
    SoMFShort strip;		//< Strip length
    SoMFShort adc;		//< Strip adc
    SoSFFloat gain;		//< Strip gain

protected:
    virtual void refresh (void);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

# ifdef WIN32
#  include <SoWinEnterScope.h>
# endif
#endif // IG_OPEN_INVENTOR_IG_SO_SI_STRIPS_H
