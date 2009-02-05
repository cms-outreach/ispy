#ifndef IG_WEB_TREE_BROWSER_IG_WEB_TREE_SERVICE_H
# define IG_WEB_TREE_BROWSER_IG_WEB_TREE_SERVICE_H

//<<<<<< INCLUDES                                                       >>>>>>
# include "Ig_Modules/IgWebTree/interface/config.h"
# include "Ig_Modules/IgSocketDriver/interface/IgWebService.h"
# include <classlib/utils/Hook.h>
# include <list>
# include <string>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
class IgState;
class IgUIDModel;
class IgRepresentable;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_WEB_TREE_API IgWebTreeService : public IgWebService
{
public:
    static const char *catalogLabel (void);    
    IgWebTreeService (IgState *state);

    void browse (Arguments *arguments);    
    void getRepID (Arguments *arguments);
    void twigMenu (Arguments *arguments);
    
    IgUIDModel *	uidModel (void);    
    
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    typedef lat::Hook1<std::string, IgRepresentable *> MenuHook;

    void registerMenuHook (MenuHook hook);
private:
    IgState *		m_state;    
    IgUIDModel *	m_uidModel;    

    typedef std::list < MenuHook > MenuHooks;
    MenuHooks 		m_menuHooks;    
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_WEB_TREE_BROWSER_IG_WEB_TREE_SERVICE_H
