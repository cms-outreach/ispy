#ifndef IG_OPEN_INVENTOR_IG_SO_LEGO_TOWERS_H
# define IG_OPEN_INVENTOR_IG_SO_LEGO_TOWERS_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Inventor/interface/IgSoShapeKit.h"
# include <Inventor/fields/SoSFFloat.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_OPEN_INVENTOR_API IgSoLegoTowers : public IgSoShapeKit
{
    SO_KIT_HEADER (IgSoLegoTowers);
    SO_KIT_CATALOG_ENTRY_HEADER (font);
    SO_KIT_CATALOG_ENTRY_HEADER (phiLabels);
    SO_KIT_CATALOG_ENTRY_HEADER (etaLabels);
    SO_KIT_CATALOG_ENTRY_HEADER (labels);
    SO_KIT_CATALOG_ENTRY_HEADER (labelScale);
    SO_KIT_CATALOG_ENTRY_HEADER (labelX);
    SO_KIT_CATALOG_ENTRY_HEADER (labelZ);
    SO_KIT_CATALOG_ENTRY_HEADER (grid);
    SO_KIT_CATALOG_ENTRY_HEADER (minorGridSep);
    SO_KIT_CATALOG_ENTRY_HEADER (legos);

public:
    IgSoLegoTowers (void);
    static void initClass (void);

    SoSFFloat	heightScale;

protected:
    virtual void refresh (void);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_OPEN_INVENTOR_IG_SO_LEGO_TOWERS_H
