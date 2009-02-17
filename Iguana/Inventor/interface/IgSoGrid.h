#ifndef IG_OPEN_INVENTOR_IG_SO_GRID_H
# define IG_OPEN_INVENTOR_IG_SO_GRID_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Inventor/interface/IgSoShapeKit.h"
# include <Inventor/fields/SoSFFloat.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_OPEN_INVENTOR_API IgSoGrid : public IgSoShapeKit
{
public:
    SO_KIT_HEADER (IgSoGrid);
    SO_KIT_CATALOG_ENTRY_HEADER (xLabels);
    SO_KIT_CATALOG_ENTRY_HEADER (zLabels);
    SO_KIT_CATALOG_ENTRY_HEADER (grid);
    SO_KIT_CATALOG_ENTRY_HEADER (font);
    SO_KIT_CATALOG_ENTRY_HEADER (labels);

public:
    IgSoGrid (void);

    static void initClass (void);

    SoSFFloat	maxX;
    SoSFFloat	maxZ;
    
protected:
    virtual void refresh (void);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_OPEN_INVENTOR_IG_SO_GRID_H
