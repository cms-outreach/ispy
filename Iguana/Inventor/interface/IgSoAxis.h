#ifndef IG_OPEN_INVENTOR_IG_SO_AXIS_H
# define IG_OPEN_INVENTOR_IG_SO_AXIS_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Inventor/interface/IgSoShapeKit.h"
# include <Inventor/fields/SoSFInt32.h>
# include <Inventor/fields/SoSFFloat.h>
# include <Inventor/fields/SoSFString.h>
# include <Inventor/fields/SoSFBool.h>
# ifdef WIN32
#  include <SoWinLeaveScope.h>
# endif

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** @class IgSoAxis Iguana/Inventor/interface/IgSoAxis.h

    @brief Provides a co-ordinate axis shape with label and adjustable
    length markers.  */

class IG_OPEN_INVENTOR_API IgSoAxis : public IgSoShapeKit
{
    SO_KIT_HEADER (IgSoAxis);
    SO_KIT_CATALOG_ENTRY_HEADER (line);
    SO_KIT_CATALOG_ENTRY_HEADER (head);
    SO_KIT_CATALOG_ENTRY_HEADER (headTrans);
    SO_KIT_CATALOG_ENTRY_HEADER (headCone);
    SO_KIT_CATALOG_ENTRY_HEADER (markers);
    SO_KIT_CATALOG_ENTRY_HEADER (font);
    SO_KIT_CATALOG_ENTRY_HEADER (labelSep);
    SO_KIT_CATALOG_ENTRY_HEADER (labelTrans);
    SO_KIT_CATALOG_ENTRY_HEADER (labelText);

public:
    IgSoAxis (void);
    static void initClass (void);

    SoSFInt32  divisions;        //< Number of divisions
    SoSFFloat  divisionLength;   //< Length of one division in the default units
    SoSFString label;		 //< Axis label
    SoSFFloat  totalLength;      //< total Length of the axis including arrowhead for rendering with offset
    SoSFBool   offset;		 //< flag indicating if offset is allowed

protected:
    virtual void refresh (void);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

# ifdef WIN32
#  include <SoWinEnterScope.h>
# endif
#endif // IG_OPEN_INVENTOR_IG_SO_AXIS_H
