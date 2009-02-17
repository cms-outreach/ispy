#ifndef IG_OPEN_INVENTOR_IG_SB_FIELD_H
# define IG_OPEN_INVENTOR_IG_SB_FIELD_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Inventor/interface/config.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** @class IgSbField Iguana/Inventor/interface/IgSbField.h

    @brief Abstraction of a vector field.  */

class IG_OPEN_INVENTOR_API IgSbField
{
public:
    virtual ~IgSbField (void);

    /** Determine field value at @a point, returning the value in @a field.
        The caller must ensure that @a field has enough space for all the
	components that the field will return (normally 3).  */
    virtual void evaluate (const double point [3], double field []) const = 0;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_OPEN_INVENTOR_IG_SB_FIELD_H
