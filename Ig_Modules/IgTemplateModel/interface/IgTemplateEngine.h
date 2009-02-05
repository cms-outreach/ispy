#ifndef IG_TEMPLATE_MODEL_IG_TEMPLATE_ENGINE_H
# define IG_TEMPLATE_MODEL_IG_TEMPLATE_ENGINE_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgTemplateModel/interface/config.h"
# include <string>
# include <list>
# include <map>
# include <classlib/utils/Hook.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgState;
class IgTemplateItem;
class IgTwig;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_TEMPLATE_MODEL_API IgTemplateEngine
{
public:
    typedef lat::Hook<std::string> TemplateHook;    

    class TemplateCommand 
    {
    public:
	TemplateCommand (std::string element);
    
	std::string name (void);
	std::string arg (const std::string &key);
    private:
	typedef std::map <std::string, std::string> ArgsMap;
	std::string m_commandName;    
	ArgsMap m_args;	
    };
    

    IgTemplateEngine (const std::string &templateConfiguration,
		      IgState *state,
		      IgTwig *root);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    std::string text (void);
    std::string textCallback (std::string text);
    void	registerPlugins (void);
    void 	parse (std::string inputTemplate);    
    void	setRoot (IgTwig *root);    
    TemplateHook	rep (TemplateCommand &command);
    IgState *	state (void);
        
private:
    std::string 	m_template;   
    IgState * 		m_state;
    IgTwig *		m_root;    

    typedef std::map <std::string, IgTemplateItem *> ItemMap;
    ItemMap  		m_itemMap;

    typedef std::list <TemplateHook> HookList;
    HookList 	m_docCallbacks;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_TEMPLATE_MODEL_IG_TEMPLATE_H
