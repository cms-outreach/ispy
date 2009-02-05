#include "Ig_Modules/IgRZBrowser/interface/IgRZBrowser.h"
#include "Ig_Modules/Ig3DBaseModel/interface/Ig3DBaseRep.h"
#include "Ig_Modules/IgRZModel/interface/IgRZModel.h"
#include <Inventor/nodes/SoGroup.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/SbLinear.h>
#include <Inventor/nodes/SoCamera.h>
#include <Inventor/fields/SoSFVec3f.h>
#include <iostream>
#include <qpopupmenu.h>
#include <cmath>

const char *
IgRZBrowser::catalogLabel (void)
{ return "RZ"; }

IgRZBrowser::IgRZBrowser (IgState *state,
			      IgSite *site,
			      IgRZModel *model)
    : Ig3DBaseBrowser (state,
		       site,
		       model ? model : new IgRZModel (state))
{
    init ();
}

void
IgRZBrowser::init (void)
{
    SoQtExaminerViewer::setCameraType (SoOrthographicCamera::getClassTypeId ());
    SoOrthographicCamera * const camera = dynamic_cast <SoOrthographicCamera *> (this->getCamera ());
    if (!camera) return; // probably a scene-less viewer

    if (SoQtViewer::isHeadlight ()) SoQtViewer::setHeadlight (false);

//     SbVec3f norient = SbVec3f (-1, 0, 0);
//     camera->position = -norient * camera->position.getValue ().length ();
//     camera->orientation = SbRotation (SbVec3f (0, 1, 0), -M_PI / 2.f);

    camera->position = SbVec3f (-14.815891, 0, 0);
    camera->orientation = SbRotation (SbVec3f (-0, -1, -0), 1.5707964);
    camera->aspectRatio = 1;
    camera->focalDistance = 14.815891;
    camera->height = 12.273887;
    camera->nearDistance = 0.1;
    camera->farDistance = 32767;

    setEventCallback (eventCallback, this); 
    setBackgroundColor (SbColor (1.0, 1.0, 1.0));
}

SbBool
IgRZBrowser::eventCallback (void *closure, QEvent *event)
{
    static bool firstEvent = true;    
    static float oldX = 0.;
    static float oldY = 0.;
    IgRZBrowser *browser = static_cast<IgRZBrowser *> (closure);
    if (! browser->isViewing ())
        return false;
    if (! browser->getParentWidget ()->hasMouse () && firstEvent)
        return false;
    if (! Ig3DBaseBrowser::eventCallback (closure, event))
    {      
        if (QMouseEvent *mouseEvent = dynamic_cast<QMouseEvent *> (event))
        {
	    if (mouseEvent->type () == QEvent::MouseMove
	        && mouseEvent->stateAfter () == Qt::NoButton)
	    {
	        if (! firstEvent)
	        {
	            firstEvent = true;
	            return true;
	        }
	    }
	    else if (mouseEvent->state () == Qt::LeftButton)
	    {
	        if (mouseEvent->stateAfter () == Qt::NoButton)
  	            firstEvent = true;
	        else if (mouseEvent->type () == QEvent::MouseMove)
	        {
	            if (firstEvent)
	            {
	                oldX = mouseEvent->x ();
	                oldY = mouseEvent->y ();
	                firstEvent = false;
	            }
	            else
	            {
	                float deltaX = mouseEvent->x () - oldX;
	                float deltaY = oldY - mouseEvent->y ();
	                SoCamera *camera = browser->getCamera ();
	                SbVec3f oldPosition = camera->position.getValue ();
	                camera->position = oldPosition - 
					   SbVec3f (deltaX/10, deltaY/10, 0);
	                oldX = mouseEvent->x ();
	                oldY = mouseEvent->y ();
	            }
	            return true;
	        }
	    }
	    return false;
        }
        return false;
    }
    return true;
}

QWidget *
IgRZBrowser::buildLeftTrim (QWidget *parent)
{
    QWidget	*result = new QWidget (parent);
    result->setFixedWidth (0);
    return result;    
}

QWidget *
IgRZBrowser::buildBottomTrim (QWidget *parent)
{
    QWidget	*result = new QWidget (parent);
    result->setFixedHeight (0);
    return result;    
}

QWidget *
IgRZBrowser::buildRightTrim (QWidget *parent)
{
    QWidget	*result = new QWidget (parent);
    result->setFixedWidth (0);
    return result;    
}

void
IgRZBrowser::buildDecoration (QWidget *parent)
{
    Ig3DBaseBrowser::buildDecoration (parent);    
    return;    
}

void
IgRZBrowser::toggleCameraType (void)
{
    SoQtExaminerViewer::toggleCameraType ();
    Ig3DBaseBrowser::toggleCameraType ();    
}

void
IgRZBrowser::invertCamera (void)
{
    SoCamera * const camera = this->getCamera ();
    if (!camera) return; // probably a scene-less viewer

    camera->position = camera->position.getValue () * -1.0F;    
}

void
IgRZBrowser::viewPlaneX (void)
{
    SoCamera * const camera = this->getCamera ();
    if (!camera) return; // probably a scene-less viewer
    
    camera->position = SbVec3f (-1, 0, 0) * camera->position.getValue ().length ();
    camera->orientation = SbRotation (SbVec3f (0, 1, 0), -M_PI / 2.f);
}

void
IgRZBrowser::viewPlaneY (void)
{
    SoCamera * const camera = this->getCamera ();
    if (!camera) return; // probably a scene-less viewer

    SbVec3f norient = SbVec3f (0, -1, 0);
    camera->position = -norient * camera->position.getValue().length();
    camera->orientation = SbRotation (SbVec3f (0, 0, 1), -M_PI / 2.f) * 
                          SbRotation (SbVec3f (0, 0, -1), norient);
}

void
IgRZBrowser::viewPlaneZ (void)
{
    SoCamera * const camera = this->getCamera ();
    if (!camera) return; // probably a scene-less viewer
  
    camera->position = SbVec3f (0, 0, 1) * camera->position.getValue ().length ();
    camera->orientation = SbRotation::identity ();
}
