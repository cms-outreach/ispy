//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgSocketDriver/interface/IgFileManager.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgState.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgArgsElement.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgEnvsElement.h"
#include <classlib/utils/DebugAids.h>
#include <classlib/utils/Log.h>
#include <classlib/iobase/Filename.h>
#include <iostream>
#include <qfile.h>
#include <vector>
#include <string>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>

lat::logflag LFfileManager = {0, "fileManager", true, -1 };

//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

IG_DEFINE_STATE_ELEMENT (IgFileManager, "Services/File Manager");

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgFileManager::IgFileManager (IgState *state)
    : m_state (state),
      m_debugFiles (false)
{
    IgArgsElement *args = IgArgsElement::get (m_state);
    ASSERT (args);
    
    m_debugFiles = args->exists ("--ig-debug-file-manager");
    
    m_state->put (s_key, this);
    addPaths ("/share/");
}


IgFileManager::~IgFileManager (void)
{
    m_state->detach (s_key);
}

void
IgFileManager::addPackagePath (const std::string &packageName,
                               bool prepend /*= false*/)
{ addPaths ("/src/"+packageName, prepend); }

void
IgFileManager::addPaths (const std::string &relPath,
                         bool prepend /*= false*/)
{ addPaths (relPath, "IGUANA_PATH", prepend); }

void
IgFileManager::addPaths (const std::string& relPath,
                         const std::string& envVariable,
			 bool prepend /*= false*/)
{
    IgEnvsElement* envs = IgEnvsElement::get(m_state);
    std::vector<std::string> paths;
    if (envs->getEnv(envVariable, paths))
    {
        for(size_t i=0; i<paths.size(); i++)
	{
            if (!paths[i].empty())
	        this->addSearchPath (paths[i]+relPath, prepend);
	}
    }
}

QByteArray
IgFileManager::lookup (const std::string &alias)
{
     if (m_debugFiles)
     {
 	m_cacheMap.erase (alias);	
     }
    
    if (m_cacheMap.find (alias) == m_cacheMap.end ())
    {
	LOG (0, trace, LFfileManager, 
	     "Alias " << alias 
	     << " not found. Adding it as if it was a filename" << std::endl);
	
	this->add (alias, alias);	
    }    

    return m_cacheMap[alias];    
}

void
IgFileManager::add (QByteArray data, 
		    const std::string &alias, 
		    bool /*save*/)
{
    m_cacheMap.insert (CacheMap::value_type (alias, data));    
}

void
IgFileManager::add (const std::string &name, const std::string &alias)
{
    lat::Filename filename (name);
    lat::Filename completeFilename;
    
    if (filename.isRelative ())
    {
	for (PathList::iterator i = m_pathList.begin ();
	     i != m_pathList.end () 
	     && completeFilename.empty ();
	     i++)
	{
	    std::string path = *i;
	    lat::Filename tmpCompleteFilename
		= path + filename.name ();
	    LOG (0, trace, LFfileManager, "Looking up for name " 	
		 << tmpCompleteFilename.name () << std::endl);
	    
	    if (tmpCompleteFilename.exists ())
		completeFilename = tmpCompleteFilename.name ();
	}	
    }
    else
    {
	if (filename.exists ())
	    completeFilename = filename;	
    }
    
    LOG (0, trace, LFfileManager, 
	 "Loading file: " << completeFilename << std::endl);
    
    QFile f (completeFilename.name ());
    
    if (f.open (IO_ReadOnly))
    {
	QByteArray buffer = f.readAll ();
	this->add (buffer, alias, false);		    
	f.close ();	
   }	
    return;    
}

void
IgFileManager::addAlias (const std::string &oldAlias, 
			 const std::string &newAlias)
{
    if (m_cacheMap.find (oldAlias) != m_cacheMap.end ())
    {
	m_cacheMap.insert (CacheMap::value_type (newAlias, m_cacheMap[oldAlias]));	
    }    
}


void
IgFileManager::addSearchPath (const std::string &path,
                              bool prepend /*= false*/)
{
    lat::Filename pathName (path);
    if (pathName.exists ())
    {
        if (prepend)
	    m_pathList.push_front (path);
	else
	    m_pathList.push_back (path);
        m_pathList.unique();
    }
    else
    {
         LOG (0, trace, LFfileManager, 
	     "Path not Added (no exists): " << path << std::endl);
    }
}
