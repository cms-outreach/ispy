#ifndef IG_WEB_2D_IG_WEB_2D_SERVICE_H
# define IG_WEB_2D_IG_WEB_2D_SERVICE_H

//<<<<<< INCLUDES                                                       >>>>>>
# include "Ig_Modules/IgWeb2D/interface/config.h"
# include "Ig_Modules/IgSocketDriver/interface/IgWebService.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
class IgState;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_WEB_2D_API IgWeb2DService : public IgWebService
{
public:
    static const char *catalogLabel (void);
    IgWeb2DService (IgState *state);

    void create (Arguments *arguments);
    void browse (Arguments *arguments);
    
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_WEB_2D_IG_WEB_2D_SERVICE_H
