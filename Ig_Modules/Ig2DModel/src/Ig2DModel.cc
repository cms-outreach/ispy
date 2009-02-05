//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/Ig2DModel/interface/Ig2DModel.h"
#include "Ig_Modules/Ig2DModel/interface/Ig2DRep.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgRepSet.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgRepContext.h"
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/nodes/SoVertexProperty.h>
#include <Inventor/nodes/SoIndexedFaceSet.h>
#include <Inventor/nodes/SoShapeHints.h>
#include <Inventor/nodes/SoGroup.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/nodes/SoScale.h>
#include <Inventor/nodes/SoSphere.h>
#include <Inventor/nodes/SoQuadMesh.h>
#include <classlib/utils/StringOps.h>
#include <classlib/utils/DebugAids.h>
#include <classlib/utils/Log.h>

#include <cassert>
#include <fstream>
#include <sstream>
#include <iostream>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

#define GRID_SIZE 20

/**Ig2DModel is used to represents objects in 2d. There is the option
   to provide a input configuration file (at the moment, shared among
   all the 2d models) with the following syntax: 
   ---CUT-HERE---
   #comments start after a # 
   /Object/Foo 
   /Object/Bar          #Layer containing Foo comes first the layer containing Bar
   /Object/FooBar=nocut #Layer containing /Object/FooBar must not be sliced 
   ---CUT-HERE---
*/

lat::logflag LFcommon2d = { 0, "common2d", true, -1 };

Ig2DModel::Ig2DModel (IgState *state, Ig3DBaseModel *sourceModel)
    : Ig3DBaseModel (state),
      m_sourceModel (sourceModel),
      m_objectTransform (0),
      m_cutGroup (0)
{
    LOG (0, trace, LFcommon2d, "Ig2DModel created\n");	

    std::ifstream cfgFile ("layers.dat");

    SoScale *scale = new SoScale;
    scale->scaleFactor = SbVec3f (1, 1, 100);
    scale->setName ("SUBLAYER_SCALING");
    attachPoint ()->magic ()->addChild (scale);
    
    if (cfgFile.is_open ())
    {
	/*reading of the input file*/
	while (cfgFile.good ())
	{
	    std::stringstream fullName;
	    std::stringbuf buffer;
	    std::stringbuf element;
	    
	    cfgFile.get (buffer, '\n');	   
	    std::string line = 
		lat::StringOps::split (buffer.str(), "#").front ();
	    
	    if (line != "")
	    {
		lat::StringList stringList = 
		    lat::StringOps::split (line, "=");		
		
		if (stringList.size () > 1)
		{
		    if (find (stringList.begin (), 
			      stringList.end (), 
			      std::string ("nocut")) != stringList.end ())
		    {
			std::string layerName = stringList.front ();
			m_cutConf.insert (layerName);
			LOG (0, trace, LFcommon2d, 
			     "Not cutting layer:" << layerName << "\n");			
		    }
		    else
		    {
			LOG (0, trace, LFcommon2d,
			     "nocut not found in " << line << "\n");			
		    }		    
		}
		else
		{
		    LOG (0, trace, LFcommon2d, "Layer:" << line << "\n");
		    
		    SoSwitch	*node = new SoSwitch;
		    node->whichChild = SO_SWITCH_ALL;
		    node->setName (encode (line));					
		    LOG (0, trace, lat::LFgeneral, line << "\n");
		    LOG (0, trace, lat::LFgeneral, 
			 node->getName ().getString () << "\n");
		    
		    Ig2DRep *rep = new Ig2DRep (this, 0, 0, node);
		    
		    ASSERT (rep);
		}		
	    }
	    //FIXME: max 1024 trash characters after layer position.
	    cfgFile.ignore (1024,'\n');
	}
	cfgFile.close ();
    }else
    {
	LOG (0, trace, LFcommon2d, "Cannot read configuration file");	
    }
    
    // Rotate the object to face the correct direction
    m_objectTransform = new SoTransform;
    m_objectTransform->rotation.setValue (SbVec3f (0, 1, 0), M_PI/2);
    m_objectTransform->ref ();    

    // Create a plane made of quads.
    SbVec3f vertices[2*2];

    vertices[0] = SbVec3f (-1, 1, 0);
    vertices[1] = SbVec3f (1, 1, 0);
    vertices[2] = SbVec3f (-1, -1, 0);
    vertices[3] = SbVec3f (1, -1, 0);
    
    //for (int y = 0; y < 2; y++)
    //	for (int x = 0; x < 2; x++)
    //	{
    //	    vertices [x+y*2] = SbVec3f (((float) (x - 1)) * 0.5,
    //					 ((float) (y - 1)) * 0.5,
    //					 0);
    //	}

    SoVertexProperty *properties = new SoVertexProperty;
    properties->vertex.setValues (0, 2*2, vertices);

    SoQuadMesh *mesh = new SoQuadMesh;
    mesh->verticesPerRow = 2;
    mesh->verticesPerColumn = 2;
    mesh->vertexProperty = properties;
    
    // Project the semi-sphere and translate it a bit from 0 to avoid
    // numerical problems.
    m_cutScale = new SoTransform;
    m_cutScale->scaleFactor = SbVec3f (1, 1, 1);
    m_cutPosition = new SoTransform;
    m_cutPosition->translation = SbVec3f (0, 0, 0);
    
    m_cutGroup = new SoSeparator;
    m_cutGroup->ref ();
    m_cutGroup->addChild (m_cutPosition);
    m_cutGroup->addChild (m_cutScale);
    m_cutGroup->addChild (mesh);
}

Ig3DBaseModel *
Ig2DModel::sourceModel (void) const
{ return m_sourceModel; }

SoNode *
Ig2DModel::cutTransform (void) const
{ return m_objectTransform; }

void
Ig2DModel::setCutTransform (SbVec3f axis, float angle)
{
    m_objectTransform->rotation.setValue (axis, angle);

    Ig3DBaseRep *start = attachPoint ();
    for (int i = 0; i < start->children (); i++)
    {
	ASSERT (dynamic_cast<Ig2DRep *> (start->child (i)));
	if (IgRepContext *context = start->child (i)->context ())
	{
	    ASSERT (context->object ());
	    // FIXME: Cheat with bits.  Move to invalidation
	    IgRepSet::invalidate (context->object (), this, ~0u);
	}
	else
	    LOG (0, trace, lat::LFgeneral,
		 "2d model not updated, child does not"
		 << " have a context: " << i << " = "
		 << start->child (i)->getChild (1)->getName ().getString ()
		 << "\n");
    }	
}

SoNode *
Ig2DModel::cutPlane (void) const
{ return m_cutGroup; }

std::string
Ig2DModel::removeLast (const std::string &str)
{
    std::string result = str;    

    int position = result.rfind ("/");

    if (position == 1)
    {
    // If the slash is found in at position 1, than it its the root
    // one, skip!
	return "";	
    }
        
    result.erase (position, result.size ()-position);
    
    return result;
}

std::string
Ig2DModel::removeFirst (const std::string &str)
{
    std::string result = str;    
    
    unsigned int position = result.find ("/", 2);
    if (position == std::string::npos)
    {
	// If the slash is not found, than we are at a toplevel name.
	// Remove everthing!
	return "";	
    }
    
    result.erase (0, position);
    result = "_" + result;
    
    return result;
}

std::string
Ig2DModel::getFirst (const std::string &str)
{
    std::string result = str;
    unsigned int position = result.find ("/", 2);
    if (position != std::string::npos)
    {	    	
	result.erase (position, str.size() - position);
    }    

    return result;    
}

std::string
Ig2DModel::getLast (const std::string &str)
{
    std::string result = str;
    int position = result.rfind ("/");    
    
    if (position != 1)
    {
	// If position != 1, then it means we have to remove one
	// entry. Remember to add _ in front of it!
	result.erase (0, position);
	result = "_" + result;	
    }
        
    return result;
}


Ig2DRep *
Ig2DModel::fullMatch (Ig3DBaseRep *from, const std::string &name)
{
    //    std::cerr << "Looking for: " << name << "...";
    Ig3DBaseRep *child = (from ? from : attachPoint ())->child (encode (name));
    //    if (child)
    //	std::cerr << "Found @" << std::hex << child << std::dec << "\n";
    //    else
    //	std::cerr << "Not found\n";

    return static_cast<Ig2DRep *> (child);
}

std::string
Ig2DModel::closestMatchName (const std::string &name)
{
    // if we are looking for an empty string, just return.
    if (name == "")
    {
	return "";
    }
    //    std::cerr << "Looking for partial match: " << name << "...";

    // Search for the separator with the wanted name
    Ig3DBaseRep *child = attachPoint ()->child (encode (name));
    if (child)
    {
	//std::cerr << "Found @" << std::hex << child << std::dec << "\n";
	return name;
    }
    else
    {
	//	std::cerr << "Not found\n";
	return closestMatchName (removeLast (name));	
    }
}

Ig2DRep *
Ig2DModel::createFull (Ig2DRep *startingRep, const std::string &name)
{
    getFirst (name);
    
    std::string currentPath = name;
    Ig2DRep *currentRep = startingRep;

    while (getFirst (currentPath) != "")
    {
	Ig2DRep *rep;
	
	if ((rep = fullMatch (currentRep, getFirst (currentPath))))
	{
	    //  std::cerr << "Found " << getFirst (currentPath) << "...descending..." << std::endl;	    
	    currentPath = removeFirst (currentPath);
	    currentRep = rep;	    
	}
	else
	{
	    //std::cerr << "Creating: " << getFirst (currentPath) << std::endl;
	    
	    // Create a new separator with a scaling operation in front
	    SoSwitch	*node = new SoSwitch;
	    node->setName (encode (getFirst (currentPath)));
	    node->whichChild = SO_SWITCH_ALL;

	    rep = new Ig2DRep (this, currentRep, 0, node);

	    currentPath = removeFirst (currentPath);	
	    currentRep = rep;	
	}
    }
    return currentRep;
}

Ig2DRep *
Ig2DModel::getLayer (const std::string &name)
{       
    Ig2DRep *result = 0;
    
    std::string encodedName = "_" + name;
    //    std::cerr << "Getting layer: " << encodedName << std::endl;
        
    // Search for a full match
    Ig2DRep *fullMatchRep = fullMatch (attachPoint (), encodedName);
    if (fullMatchRep != 0) 
    {
	result = fullMatchRep;
    }
    else
    {
	// Search for a partial match
	std::string closestMatchName = this->closestMatchName (encodedName);
    
	if (closestMatchName != "")
	{
	    // If a partial match is found, look for the rep associate to
	    // it and add the remaining part.
	    Ig2DRep *closestMatchRep
		= static_cast<Ig2DRep *> (attachPoint ()->child
					  (encode (closestMatchName)));
	    std::string relativeName = encodedName;
	    relativeName.erase (0, closestMatchName.size ());	
	    result = createFull (closestMatchRep, "_"+relativeName);	
	}
	else
	{
	    //    std::cerr << "Creating path: " << encodedName << std::endl;
	
	    // Create a new top-level entry for the name specified.
	    result = createFull (0, encodedName);
	}
    }
    return result;    
}

void
Ig2DModel::setCutPlaneSize (float w, float h)
{
    m_cutScale->scaleFactor = SbVec3f (w, h, 0);    
}

void
Ig2DModel::setCutPlanePosition (float x, float y)
{
    m_cutPosition->translation = SbVec3f (x, y, 0);
}

bool
Ig2DModel::isToBeCut (std::string twigName)
{
    for(NotCutTwigSet::iterator i = m_cutConf.begin ();
	i != m_cutConf.end ();
	i++)
    {
	std::string matchString = *i;
	if (twigName.find (matchString) != std::string::npos)
	{
	    LOG (0, trace, LFcommon2d,
		 "Not cutting " << twigName
		 << " because matches " << matchString << "\n");
	    return false;	    
	}
    } 
    return true;    
}
