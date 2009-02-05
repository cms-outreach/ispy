//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgConfig/interface/IgConfigElement.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgArgsElement.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgEnvsElement.h"
#include <qfile.h>
#include <qbuffer.h>
#include <qdom.h>
#include <fstream>
#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <classlib/iobase/Filename.h>
#include <classlib/iobase/File.h>
#include <classlib/utils/UserInfo.h>
#include <classlib/utils/DebugAids.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

IG_DEFINE_STATE_ELEMENT (IgConfigElement, "Services/Global/Config");

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
class IgConfigElementNode {
public:
    
    IgConfigElementNode ()
	{hasValue = false;}
    
    std::string name;
    std::string	value;
    bool	hasValue;
    std::vector<IgConfigElementNode> childs;

    static IgConfigElementNode* find (IgConfigElementNode* node,
				      const std::string& name);
    static void 		print (IgConfigElementNode* node,
				       std::ostream& out,
				       const std::string& itab = "");
};

//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgConfigElement::IgConfigElement (IgState *state)
    : m_state (state),
      m_prefix (""),
      m_file (""),
      m_dirty (true)
{
    ASSERT (m_state);
    m_state->put (s_key, this);
    
    initConfiguration ();
}

IgConfigElement::~IgConfigElement ()
{ m_state->detach (s_key); }

const IgConfigElement::KeyMap& IgConfigElement::getKeyMap () const
{ return m_config; }

void
IgConfigElement::initConfiguration ()
{
    IgArgsElement *args = IgArgsElement::get (m_state);
    ASSERT (args);
    
    lat::UserInfo *user = lat::UserInfo::self ();
    if (args->find ("--ig-config", m_file, true))
    {
        lat::Filename f (m_file);
	if (f.isDirectory () || !f.isReadable ())
	{
	    ASSERT ("ERROR: IGUANA configuration file does not exist or not a regular file" == 0);
	    m_file = "";
	}
    }
    else
    {
	std::string home (user->homedir ());
	m_file = home + "/.iguana";
	lat::Filename dir (m_file);
	if(!dir.isDirectory ())
	    dir.makedir (m_file, 0755, false, true);
	else if (!dir.isWritable () || !dir.isExecutable ())
	    ASSERT ("ERROR: IGUANA configuration directory ~/.iguana is not writable. Please make sure that directory permission are 0755 and if it exists on an AFS area then you should have a valid AFS token." == 0);

	m_file = m_file + "/iguana.conf";
	lat::Filename file (m_file);
	if (!file.exists ())
	{
	    lat::File f;
	    f.open (m_file, lat::IOFlags::OpenWrite | lat::IOFlags::OpenCreate);
	    f.close();
	}
    }
    putValue ("/IgConfigElementInternal/PID",		getpid ());
    putValue ("/IgConfigElementInternal/PPID",		getppid ());
    putValue ("/IgConfigElementInternal/UID",		user->uid ());
    putValue ("/IgConfigElementInternal/GID",		user->uid ());
    putValue ("/IgConfigElementInternal/ID",		user->id ());
    putValue ("/IgConfigElementInternal/USER",		user->name ());
    putValue ("/IgConfigElementInternal/HOMEDIR",	user->homedir ());
    putValue ("/IgConfigElementInternal/SHELL",		user->shell ());
    
    if (!m_file.empty ())
    {
	QDomDocument doc ("IGUANA_CONFIG");
	std::ifstream file;
	file.open (m_file.c_str ());
	ASSERT (file.good ());
	
	QBuffer buffer;
	buffer.open (IO_ReadWrite);
	buffer.writeBlock ("<C>", 3);
	
	char ch;
	file.get (ch);
	while (!file.eof ())
	{
	    buffer.writeBlock (&ch, 1);
	    file.get (ch);
	}
	
	buffer.writeBlock ("</C>", 4);
	file.close ();
	buffer.reset ();
	
	if (!doc.setContent (&buffer))
	    ASSERT ("ERROR: Reading IGUANA configuration file" == 0);
	else
	    processNode (doc.firstChild (), "");
    }
    m_dirty = false;
}

void
IgConfigElement::processNode (const QDomNode& node, const std::string& path)
{
    if (node.isElement ())
    {
        if (node.nodeName () == "IgConfigElementInternal") return;
	std::string npath = path + "/" + node.nodeName ().latin1 ();
	if(node.hasChildNodes ())
        {
            QDomNode child = node.firstChild ();
	    while (!child.isNull ())
	    {
	        processNode (child, npath);
	        child = child.nextSibling ();
	    }
	}
	else
	    putValue (npath.substr (2), "", true);
    }
    else if (node.isText ())
    {
	std::string value (node.nodeValue ().latin1 ());
	std::string::iterator chr = value.begin ();

	while (chr != value.end ())
	{
	    if (*chr == '\n') value.erase (chr);
	    else chr++;
	}

	while (*value.begin () == ' ')
	    value.erase (value.begin ());

        while (*(--value.end ()) == ' ')
	    value.erase (--value.end ());

	putValue (path.substr (2), value, true);
    }
}

void
IgConfigElement::setPrefix (const std::string& prefix)
{
    m_prefix = "/" + prefix;
    int index = -1;
    while ((index = m_prefix.find ("//")) != -1)
	m_prefix = m_prefix.replace (index, 2, "/");
}

const std::string& 
IgConfigElement::prefix (void) const
{ return m_prefix; }

const std::string&
IgConfigElement::substituteValue(std::string &value) const
{
    IgEnvsElement* envs = IgEnvsElement::get (m_state);
    int spos = 0;
    while ((spos = value.rfind ("$", spos - 1)) != -1)
    {
        if (((int)value.size () > spos + 1) && (value[spos + 1] == '{'))
        {
            int epos = value.find ("}", spos);
            if (epos != -1)
	    {
                std::string nval ("");
                if (epos > spos + 2)
		{
                    std::string var (value.substr (spos + 2, epos - spos - 2));
		    if (!getValue (var, nval) && !getValue ("/IgConfigElementInternal/" + var, nval))
		        envs->getEnv (var, nval);
		}
		value = value.substr (0, spos) + nval + value.substr (epos + 1);
            }
        }
        if (spos == 0) break;
    }
    return value;
}

bool
IgConfigElement::getValue (const std::string& key, std::string& value) const
{
    std::string fullkey = m_prefix + "/" + key;
    int sindex = -1;
    while ((sindex = fullkey.find ("//")) != -1)
	fullkey = fullkey.replace (sindex, 2, "/");
    std::map<std::string, std::string>::const_iterator itr = 
	m_config.find (fullkey);
    if (itr != m_config.end ())
    {
        value = itr->second;
        substituteValue (value);
        return true;
    }
    return false;
}

bool
IgConfigElement::getValues (const std::string& key,
			    std::vector<std::string>& values) const
{
    std::string val;
    if (getValue (key, val))
    {
        std::string item ("");
        std::string separator (",");
        getValue ("separator", separator);
        size_t size = val.size ();
        for (size_t i = 0; i < size; i++)
        {
            if (val [i] == separator [0])
            {
	        while (*item.begin () == ' ')
	            item.erase (item.begin ());
                while (*(--item.end ()) == ' ')
	            item.erase (--item.end ());
                values.push_back (item);
	        item = "";
            }
            else
                item.push_back (val [i]);
        }
    
        while (*item.begin () == ' ')
            item.erase (item.begin ());
        while (*(--item.end ()) == ' ')
	    item.erase (--item.end ());
        values.push_back (item);
        return true;
    }
    return false;
}

void
IgConfigElement::putValue (const std::string& key,
			   const std::string& value,
			   bool override /* = true*/)
{
    std::string fullkey = m_prefix + "/" + key;
    int sindex = -1;
    while ((sindex = fullkey.find ("//")) != -1)
	fullkey = fullkey.replace (sindex, 2, "/");

    if (override || (m_config.find (fullkey) == m_config.end ()))
    {
        m_config [fullkey] = value;
	m_dirty = true;
    }
}

void
IgConfigElement::putValues (const std::string& key,
			    const std::vector<std::string>& values,
			    bool override /* = true*/)
{
    std::string nvalue ("");
    if (values.size () > 0)
    {
        nvalue = values [0];
        std::string separator (",");
        getValue ("separator", separator);
	for (size_t i = 1; i < values.size (); i++)
	    nvalue += separator + values [i];
    }
    putValue (key, nvalue, override);
}

void
IgConfigElement::convert (const std::string& str, bool& value) const
{
    if (str.size () > 1)
    {
        if ((str == "true") || (str == "TRUE") || (str == "True"))
            value = true;
        else
            value = false;
    }
    else if ((str == "t") || (str == "T") || (str == "1"))
        value = true;
    else
        value = false;
}

void
IgConfigElement::save (const std::string& filename /* = ""*/)
{
    std::string file (filename);
    if (filename.empty ())
        file = m_file;
    if (!file.empty ())
    {
	if ((file == m_file) && !m_dirty) return;
	IgConfigElementNode head;
	head.name = "IgConfigElementHeadNodeDummy";
	std::map<std::string, std::string>::const_iterator itr = 
	    m_config.begin ();
        while (itr != m_config.end ())
	{
	    if (itr->first.find ("/IgConfigElementInternal/") == 0)
	    {
	        itr++;
		continue;
	    }
	    IgConfigElementNode* cur = &head;
	    std::string key = itr->first;
	    std::string part ("");
            size_t size = key.size ();
	    for (size_t i = 0; i < size; i++)
            {
                char ch = key [i];
	        if (ch == '/')
                {
	            if (part.empty ()) continue;
		    cur = IgConfigElementNode::find (cur, part);
		    part = "";
                }
                else
		    part.push_back (ch);
            }
	    if (!part.empty ())
	        cur = IgConfigElementNode::find (cur, part);
	    cur->value = itr->second;
	    cur->hasValue = true;
	    itr++;
	}
	
	std::ofstream out;
	out.open (file.c_str ());
	ASSERT (out.good ());
	for (size_t i = 0; i < head.childs.size (); i++)
	    IgConfigElementNode::print (&head.childs [i], out);
	out.close ();
    }
}
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////

IgConfigElementNode* IgConfigElementNode::find (IgConfigElementNode* node,
						const std::string& name)
{
    if (node->name == "")
	node->name = name;
    else if (node->name == name)
        return node;
    else
    {
	for (size_t i = 0; i < node->childs.size (); i++)
	{
            if (node->childs [i].name == name)
		return &node->childs [i];
	}
	IgConfigElementNode child;
	child.name = name;
	node->childs.push_back (child);
	node = &node->childs [node->childs.size () - 1];
    }
    return node;
}

void IgConfigElementNode::print (IgConfigElementNode* node,
				 std::ostream& out,
				 const std::string& itab /* = "" */)
{
    if (node->name.empty()) return;
    out << itab.c_str () << "<" << node->name.c_str () << ">";
    if (node->hasValue)
        out << node->value.c_str () << "</" << node->name.c_str () << ">" << std::endl;
    else
    {
        out << std::endl;
	for (size_t i = 0; i < node->childs.size (); i++)
	    print (&node->childs [i], out, itab + "  ");
        out << itab.c_str () << "</" << node->name.c_str () << ">" << std::endl;	    
    }
}

