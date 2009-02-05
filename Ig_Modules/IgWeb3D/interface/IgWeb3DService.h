#ifndef IG_WEB_3D_IG_WEB_3D_SERVICE_H
# define IG_WEB_3D_IG_WEB_3D_SERVICE_H

//<<<<<< INCLUDES                                                       >>>>>>
# include "Ig_Modules/IgWeb3D/interface/config.h"
# include "Ig_Modules/IgSocketDriver/interface/IgWebService.h"
# include <string>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
class IgState;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_WEB_3D_API IgWeb3DService : public IgWebService
{
public:
    static const char *catalogLabel (void);
    IgWeb3DService (IgState *state);

    void create (Arguments *arguments);
    void browse (Arguments *arguments);

    void doCreate (IgState *state, std::string browserName);
    
    
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_WEB_3D_IG_WEB_3D_SERVICE_H
