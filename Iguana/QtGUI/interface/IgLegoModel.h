#ifndef VIEW_IG_LEGO_MODEL_H
# define VIEW_IG_LEGO_MODEL_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Framework/interface/config.h"
# include "Iguana/Framework/interface/IgModel.h"
# include <classlib/utils/Callback.h>
# include <vector>
# include <string>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgState;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IGUANA_FRAMEWORK_API IgLegoModel : public IgModel
{
public:
    IgLegoModel (IgState *state);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIEW_IG_LEGO_MODEL_H
