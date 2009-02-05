#include "Ig_Modules/Ig2DBrowser/interface/Ig2DBrowser.h"
#include "Ig_Modules/Ig2DModel/interface/Ig2DModel.h"
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/SbLinear.h>
#include <Inventor/nodes/SoCamera.h>
#include <Inventor/fields/SoSFVec3f.h>
#include <qpopupmenu.h>
#include <cmath>

Ig2DBrowser::Ig2DBrowser (IgState *state, IgSite *site, Ig2DModel *model)
    : Ig3DBaseBrowser (state, site, model ? model : new Ig2DModel (state, 0))
{
    SoQtExaminerViewer::setCameraType (SoOrthographicCamera::getClassTypeId ());
    
    if (SoQtViewer::isHeadlight ()) SoQtViewer::setHeadlight (false);
    
    setAnimationEnabled (false);
    viewAll ();    
    setEventCallback (eventCallback, this);
}

const char *
Ig2DBrowser::catalogLabel (void)
{ return "2D"; }

void
Ig2DBrowser::viewPlaneX (void)
{
    //    bodyArea ()->setCursor (WaitCursor);
    
    Ig2DModel *tModel = dynamic_cast <Ig2DModel *> (model ());
    ASSERT (tModel);

    SbVec3f axis = SbVec3f (0, 1, 0);
    float angle = M_PI/2;	    
    	
    tModel->setCutTransform (axis, angle);
    //  bodyArea ()->setCursor (ArrowCursor);
}

void
Ig2DBrowser::viewPlaneY (void)
{
    //    bodyArea ()->setCursor (WaitCursor);
    Ig2DModel *tModel = dynamic_cast <Ig2DModel *> (model ());
    ASSERT (tModel);
    
    SbVec3f axis1 = SbVec3f (1, 0, 0);
    SbVec3f axis2 = SbVec3f (0, 0, 1);
    SbVec3f axis;
    float angle;	    
    SbRotation r = SbRotation (axis1, M_PI/2) * SbRotation (axis2, M_PI/2);
    r.getValue (axis, angle);
    
    tModel->setCutTransform (axis, angle);
    //    bodyArea ()->setCursor (ArrowCursor);
}

void
Ig2DBrowser::viewPlaneZ (void)
{
    //    bodyArea ()->setCursor (WaitCursor);
    
    Ig2DModel *tModel = dynamic_cast <Ig2DModel *> (model ());
    ASSERT (tModel);

    SbVec3f axis = SbVec3f (0, 0, 1);
    float angle = 0;    
    
    tModel->setCutTransform (axis, angle);
    
    //    bodyArea ()->setCursor (ArrowCursor);
}

SbBool
Ig2DBrowser::eventCallback (void *closure, QEvent *event)
{
    static bool firstEvent = true;    
    static float oldX = 0.;
    static float oldY = 0.;
    Ig2DBrowser *browser = static_cast<Ig2DBrowser *> (closure);
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

void
Ig2DBrowser::buildDecoration (QWidget *parent)
{
    Ig3DBaseBrowser::buildDecoration (parent);    
    return;    
}

QWidget *
Ig2DBrowser::buildLeftTrim (QWidget *parent)
{
    QWidget	*result = new QWidget (parent);
    result->setFixedWidth (0);
    return result;    
}

QWidget *
Ig2DBrowser::buildBottomTrim (QWidget *parent)
{
    QWidget	*result = new QWidget (parent);
    result->setFixedHeight (0);
    return result;    
}

QWidget *
Ig2DBrowser::buildRightTrim (QWidget *parent)
{
    QWidget	*result = new QWidget (parent);
    result->setFixedWidth (0);
    return result;    
}
