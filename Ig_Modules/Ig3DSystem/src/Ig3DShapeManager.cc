//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/Ig3DSystem/interface/Ig3DShapeManager.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgState.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgEnvsElement.h"
#include <classlib/utils/DebugAids.h>
#include <classlib/utils/Log.h>
#include <classlib/iobase/Filename.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/SoInput.h>
#include <Inventor/SoDB.h>
#include <iostream>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
lat::logflag LFshapeManager = {0, "shapeManager", true, -1 };

//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
IG_DEFINE_STATE_ELEMENT (Ig3DShapeManager, "Services/Shape Manager");

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

Ig3DShapeManager::Ig3DShapeManager (IgState *state)
    : m_state (state)
{
    m_state->put (s_key, this);

    IgEnvsElement* envs = IgEnvsElement::get(m_state);
    std::vector<std::string> iguanaPath;
    if (envs->getEnv("IGUANA_PATH", iguanaPath))
    {
        for(size_t i=0; i<iguanaPath.size(); i++)
	{
            if (!iguanaPath[i].empty())
	        this->addSearchPath (iguanaPath[i]+"/share/ivs/");
	}
    }
}

Ig3DShapeManager::~Ig3DShapeManager (void)
{
    m_state->detach (s_key);
}

SoNode *
Ig3DShapeManager::lookup (const std::string &alias)
{
    if (m_nodeMap.find (alias) == m_nodeMap.end ())
    {
	LOG (0, trace, LFshapeManager, 
	     "Alias " << alias 
	     << " not found. Adding it as if it was a filename" << std::endl);
	
	this->add (alias, alias);	
    }    

    SoNode *returnNode =  m_nodeMap[alias];    
    return returnNode;    
}

void
Ig3DShapeManager::add (SoNode *node, const std::string &alias, bool /*save*/)
{
    node->ref ();    
    m_nodeMap.insert (NodeMap::value_type (alias, node));    
}

void
Ig3DShapeManager::add (const std::string &name, const std::string &alias)
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
	    LOG (0, trace, LFshapeManager, "Looking up for name " 	
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
    
    LOG (0, trace, LFshapeManager, 
	 "Loading file: " << completeFilename << std::endl);
    
    SoInput in;
    if (in.openFile (completeFilename))
    {
	SoNode *node = SoDB::readAll (&in);
	if (node)
	{
	    this->add (node, alias, false);		    
	}	    
    }	
    return;    
}

void
Ig3DShapeManager::addAlias (const std::string &oldAlias, 
			    const std::string &newAlias)
{
    if (m_nodeMap.find (oldAlias) != m_nodeMap.end ())
    {
	m_nodeMap.insert (NodeMap::value_type (newAlias, m_nodeMap[oldAlias]));	
    }    
}


void
Ig3DShapeManager::addSearchPath (const std::string &path)
{
    lat::Filename pathName (path);
    if (pathName.exists ())
        m_pathList.push_back (path);
    else
    {
        LOG (0, trace, LFshapeManager, 
	     "Path not Added (no exists): " << path << std::endl);
    }
}
