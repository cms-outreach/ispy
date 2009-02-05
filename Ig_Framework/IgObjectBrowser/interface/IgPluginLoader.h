#ifndef IG_OBJECT_BROWSER_IG_PLUGIN_LOADER_H
# define IG_OBJECT_BROWSER_IG_PLUGIN_LOADER_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Framework/IgObjectBrowser/interface/IgStateElement.h"
# include <string>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgState;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_OBJECT_BROWSER_API IgPluginLoader : public IgStateElement
{
    IG_DECLARE_STATE_ELEMENT (IgPluginLoader);
public:
    IgPluginLoader (IgState *in);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    
    virtual void		load (const std::string &name,
				      bool prefix = false);
    virtual void		load (IgState *state,
				      const std::string &name,
				      bool prefix = false);

private:
    IgState			*m_state;	//< State object.
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_OBJECT_BROWSER_IG_PLUGIN_LOADER_H
