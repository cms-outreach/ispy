#ifndef IG_WEB_STUDIO_IG_STUDIO_WEB_SERVICE_H
# define IG_WEB_STUDIO_IG_STUDIO_WEB_SERVICE_H

//<<<<<< INCLUDES                                                       >>>>>>
# include "Ig_Modules/IgWebStudio/interface/config.h"
# include "Ig_Modules/IgSocketDriver/interface/IgWebService.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
class QTextStream;
class IgState;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_WEB_STUDIO_API IgStudioWebService : public IgWebService
{
public:
    static const char *catalogLabel (void);
    
    IgStudioWebService (IgState *state);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    void initSession (Arguments *arguments);
    void login (Arguments *arguments);
    void logout (Arguments *arguments);
    void launch (Arguments *arguments);
private:
    std::string		m_sessionUrl;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_WEB_STUDIO_IG_STUDIO_WEB_SERVICE_H
