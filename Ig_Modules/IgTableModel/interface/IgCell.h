#ifndef IG_TABLE_MODEL_IG_CELL_H
# define IG_TABLE_MODEL_IG_CELL_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgTableModel/interface/config.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

// FIXME: this class (and IgSimpleCell<T>) belong to a package of their own
//        so that trees, tables and treetables (and other models) can share
//        simple cell values (strings, ints, floats, bools, 3-vectors, ...)

class IG_TABLE_MODEL_API IgCell
{
public:
    virtual ~IgCell (void);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_TABLE_MODEL_IG_CELL_H
