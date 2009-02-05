//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/Ig2DModel/interface/Ig2DRep.h"
#include "Ig_Modules/Ig2DModel/interface/Ig2DModel.h"
#include "Inventor/nodes/SoTranslation.h"
#include "Inventor/nodes/SoScale.h"
#include "Inventor/nodes/SoGroup.h"
#include "classlib/utils/DebugAids.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

Ig2DRep::Ig2DRep (Ig2DModel *model,
		  Ig2DRep *parent,
		  Ig2DRep *prev,
		  SoGroup *node)
    : Ig3DBaseRep (model, parent, prev, node)
{
    // Translate by a fixed amount: the rep will have a scaling
    // operation in front which takes care of everything.
    SoTranslation *translation = new SoTranslation;    
    translation->translation = SbVec3f (0, 0, 1);
    translation->setName ("LAYER_OFFSET");
    magic ()->addChild (translation);	    

    SoScale *scale = new SoScale;
    scale->setName ("SUBLAYER_SCALING");    
    magic ()->addChild (scale);

    if (parent)
	parent->repositionChildren ();	
}

void
Ig2DRep::repositionChildren (void)
{
    for (int i = 0; i < children (); i++)
    {
	SoNode *node = child (i)->findMagic ("LAYER_OFFSET");
	SoTranslation *translation = dynamic_cast<SoTranslation *>(node);
	ASSERT (translation);
	translation->translation = SbVec3f(0,0,i);		
    }
    SoNode *node = findMagic ("SUBLAYER_SCALING");
    SoScale *scale = dynamic_cast<SoScale *> (node);
    ASSERT (scale);
    scale->scaleFactor = SbVec3f (1, 1, 1./(children () + 1));    
}
