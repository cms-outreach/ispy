//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/Ig3DMaterialEditor/src/IgMatEdModificationAction.h"
#include <Inventor/actions/SoSubAction.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/nodes/SoMaterial.h>
#include <classlib/utils/DebugAids.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

SO_ACTION_SOURCE(IgMatEdModificationAction)


IgMatEdModificationAction::IgMatEdModificationAction (void)
{
    static bool initialized=false;
    
    if (!initialized)
    {
	IgMatEdModificationAction::initClass();	
	initialized=true;	
    }
    
    SO_ACTION_CONSTRUCTOR(IgMatEdModificationAction);
}

IgMatEdModificationAction::~IgMatEdModificationAction (void)
{
}

void
IgMatEdModificationAction::initClass (void)
{
    SO_ACTION_INIT_CLASS(IgMatEdModificationAction, SoAction);
    SO_ACTION_ADD_METHOD(SoNode, nullAction);    
    SO_ACTION_ADD_METHOD(SoMaterial, &modifyMaterial);
}

void
IgMatEdModificationAction::modifyMaterial( SoAction */*action*/, SoNode *node)
{
    SoMaterial *material = dynamic_cast<SoMaterial *> (node);
    ASSERT (material);    
    material->shininess.setValue (m_shininess);	    
    material->transparency.setValue (m_transparency);	    
    material->ambientColor.setValue (m_ambientColor);	    
    material->diffuseColor.setValue (m_diffuseColor);	    
    material->specularColor.setValue (m_specularColor);	    
    material->emissiveColor.setValue (m_emissiveColor);	    	
}


float IgMatEdModificationAction::m_transparency=0;
float IgMatEdModificationAction::m_shininess=0.2;
float IgMatEdModificationAction::m_diffuseColor[3]={0.8,0.8,0.8};
float IgMatEdModificationAction::m_ambientColor[3]={0.2,0.2,0.2};
float IgMatEdModificationAction::m_specularColor[3]={0,0,0};
float IgMatEdModificationAction::m_emissiveColor[3]={0,0,0};    

