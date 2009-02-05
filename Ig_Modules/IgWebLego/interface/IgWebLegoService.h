#ifndef IG_WEB_LEGO_IG_WEB_LEGO_SERVICE_H
# define IG_WEB_LEGO_IG_WEB_LEGO_SERVICE_H

//<<<<<< INCLUDES                                                       >>>>>>
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
# include "Ig_Modules/IgWebLego/interface/config.h"
# include "Ig_Modules/IgSocketDriver/interface/IgWebService.h"
# include <string>

//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
class IgState;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_WEB_LEGO_API IgWebLegoService : public IgWebService
{
public:
    static const char * catalogLabel (void);
    
    IgWebLegoService (IgState *state);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    void create (Arguments *arguments);
    void browse (Arguments *args);    
    void doCreate (IgState *state, const std::string &name);    
private:
    IgState *		m_state;    
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_WEB_LEGO_IG_WEB_LEGO_SERVICE_H
