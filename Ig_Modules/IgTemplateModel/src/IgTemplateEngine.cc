//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgTemplateModel/interface/IgTemplateEngine.h"
#include "Ig_Modules/IgTemplateModel/interface/IgTemplateItemDB.h"
#include "Ig_Modules/IgTemplateModel/interface/IgTemplateItem.h"
#include "Ig_Modules/IgCommonData/interface/IgTwig.h"
#include <classlib/utils/StringOps.h>
#include <classlib/utils/Callback.h>
#include <classlib/utils/Hook.h>
#include <classlib/utils/DebugAids.h>
#include <classlib/utils/StringList.h>
#include <classlib/utils/Regexp.h>
#include <iostream>
#include <string>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>


// TODO: merge together the plugins for the webservices and the ones
// for the template, as they do the same thing.

IgTemplateEngine::TemplateCommand::TemplateCommand (std::string element)
{
    ASSERT (element.size ());
    
    using namespace lat;
    
    StringList tokens 
	= StringOps::split (element, " ", StringOps::TrimEmpty);
    
    ASSERT (tokens.size ());
    
    m_commandName = tokens[0];

    element = element.erase (0, m_commandName.size () + 1);
	    
    while (element != "")
    {
	std::string key = StringOps::split (element, "=")[0];
	element = element.erase (0, key.size () + 1);
	
	ASSERT (element[0] == '"');
	
	std::string value 
	    = StringOps::split (element, "\"", StringOps::TrimEmpty)[0];
	element = element.erase (0, value.size () + 1);		
	ASSERT (element[0] == '"');
	element = element.erase (0, 1);		
	
	m_args.insert (ArgsMap::value_type (key, value));
	element = lat::StringOps::remove (element, lat::Regexp ("^[ ]*"));		
    }
}
    
std::string 
IgTemplateEngine::TemplateCommand::name (void)
{
    return m_commandName;	    
}
    
std::string
IgTemplateEngine::TemplateCommand::arg (const std::string &key)
{
    if (m_args.find (key) != m_args.end ())
	return m_args[key];
    return "";	    
}


// FIXME: root should go away and the IgTemplateItems should be held
// responsibile for getting it via state, if needed.

IgTemplateEngine::IgTemplateEngine (const std::string &inputTemplate,
				    IgState *state,
				    IgTwig *root)
    : m_template (inputTemplate),
      m_state (state),
      m_root (root)
{
    registerPlugins ();    
    parse (inputTemplate);    
}

void
IgTemplateEngine::registerPlugins (void)
{
    // Register all the template items
    IgTemplateItemDB *itemDB = IgTemplateItemDB::get ();
    
    for (IgTemplateItemDB::Iterator i = itemDB->begin ();
	 i != itemDB->end ();
	 i++)
    {
	m_itemMap.insert (ItemMap::value_type ((*i)->name (),
					       (*i)->create (this)));	
    }
}

void
IgTemplateEngine::parse (std::string inputTemplate)
{
    m_docCallbacks.clear ();
    
    lat::StringList tokens 
	= lat::StringOps::split (inputTemplate, "@@@", 
				 lat::StringOps::KeepLeadingEmpty);
    
    lat::StringList::iterator i = tokens.begin ();

    TemplateHook repHook;
    
    while (i != tokens.end ())
    {

	// Odd tokens (1st, 3rd, 5th...) are for sure text.
	ASSERT ((*i)[0] != '@');	

	repHook
	    = lat::CreateHook (this, &IgTemplateEngine::textCallback, *i);
	
	
	m_docCallbacks.push_back (repHook);	
	i++;

	// This one is for sure a section contained between two @
	if (i != tokens.end ())
	{
	    TemplateCommand command (*i);

	    if (command.name () == "rep")
	    {
		m_docCallbacks.push_back (this->rep (command));		
	    }

	    i++;	    
	}	
    }
}

IgTemplateEngine::TemplateHook 
IgTemplateEngine::rep (IgTemplateEngine::TemplateCommand &command)
{
    TemplateHook repHook;

    ItemMap::iterator i 
	= m_itemMap.find (command.arg ("kind"));
    
    if (i == m_itemMap.end ())
    {
	repHook = lat::CreateHook (this,
				   &IgTemplateEngine::textCallback,
				   "@@@ERROR, template item " + command.arg ("kind") + " not found @@@");		    
    }
    else
    {
	IgTemplateItem *t = i->second;
	
	IgTwig * twig = m_root->lookup (command.arg ("twig"));
	repHook = lat::CreateHook (t, 
				   &IgTemplateItem::represent,
				   twig);
    }
    return repHook;    
}



std::string
IgTemplateEngine::text (void)
{
    std::string finalString;
    
    for (HookList::iterator i = m_docCallbacks.begin ();
	 i != m_docCallbacks.end ();
	 i++)
    {
	finalString += (*i) ();	
    }
    
    return finalString;    
}

std::string
IgTemplateEngine::textCallback (std::string text)
{
    return text;
}

// FIXME: this should go away and the plugins should be the one doing
// the work of retrieving document data from state.

void
IgTemplateEngine::setRoot (IgTwig *root)
{
    m_root = root;    
}

IgState *
IgTemplateEngine::state (void)
{
    return m_state;    
}
