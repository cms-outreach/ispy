#ifndef IG_WEB_STUDIO_IG_BROWSER_MANAGER_H
# define IG_WEB_STUDIO_IG_BROWSER_MANAGER_H

//<<<<<< INCLUDES                                                       >>>>>>
# include "Ig_Modules/IgWebStudio/interface/config.h"
# include "Ig_Framework/IgObjectBrowser/interface/IgStateElement.h"
# include <map>
# include <vector>
# include <string>
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
class IgState;
class IgBrowser;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>



class IG_WEB_STUDIO_API IgBrowserManager : public IgStateElement
{
    IG_DECLARE_STATE_ELEMENT (IgBrowserManager);
    typedef std::map<std::string, IgBrowser *> BrowserMap;
public:
    IgBrowserManager (IgState *state);
    ~IgBrowserManager (void);
    
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    
    /** Searches for browser of type T within the list of all browsers, ,
     * create it if id does not exist and change the passed
     * pointer so that it point to the object of the kind requested.*/
    template <class T>
    T* lookup (std::string name = "")
    {
	BrowserMap::iterator i =  m_browsers.find (name);
	if (i != m_browsers.end ())
	    return dynamic_cast<T *> (i->second);
	else
	    return 0;	
    }
    
    void add (IgBrowser* browser, std::string name = "");
private:
    typedef std::pair <std::string, IgBrowser *> MapEntry;

    IgState *		m_state;
    BrowserMap   	m_browsers;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_WEB_STUDIO_IG_BROWSER_MANAGER_H
