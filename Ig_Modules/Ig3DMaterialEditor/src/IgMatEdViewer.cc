//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/Ig3DMaterialEditor/interface/IgMatEdViewer.h"
#include <qwidget.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSphere.h>
#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/Qt/SoQtRenderArea.h> 
#include <Inventor/actions/SoGLRenderAction.h>
//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgMatEdViewer::IgMatEdViewer (QWidget *parent, SoNode *model)
    :m_parent(parent),
     m_renderArea (0),
     m_light (0),
     m_camera (0),
     m_root (0),
     m_userSeparator (0),
     m_model (model)
{    
    ASSERT (m_parent);
    
    m_root = new SoSeparator ();
    m_root->ref ();
    
    m_light = new SoDirectionalLight ();
    m_light->ref ();
    
    m_camera = new SoOrthographicCamera ();
    m_camera->ref ();

    m_userSeparator = new SoSeparator ();
    m_userSeparator->ref ();

    m_userSeparator->addChild (m_model);        

    m_root->addChild (m_camera);
    m_root->addChild (m_light);    
    m_root->addChild (m_userSeparator);

    m_renderArea = new SoQtRenderArea (m_parent, "IgMatEdViewer", true, false, false);

    m_camera->viewAll (m_userSeparator, m_renderArea->getViewportRegion());
    m_renderArea->setSceneGraph (m_root);
    m_renderArea->setBackgroundColor (SbColor(0.0f, 0.2f, 0.3f));
    m_renderArea->setTransparencyType (SoGLRenderAction::BLEND);
    
    m_renderArea->show ();    
}
