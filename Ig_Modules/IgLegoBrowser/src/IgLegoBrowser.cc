#include "Ig_Modules/IgLegoBrowser/interface/IgLegoBrowser.h"
#include "Ig_Modules/Ig3DBaseModel/interface/Ig3DBaseRep.h"
#include "Ig_Modules/IgLegoModel/interface/IgLegoModel.h"
#include <Inventor/nodes/SoGroup.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/SbLinear.h>
#include <Inventor/nodes/SoCamera.h>
#include <Inventor/fields/SoSFVec3f.h>
#include <iostream>
#include <qpopupmenu.h>
#include <cmath>

const char *
IgLegoBrowser::catalogLabel (void)
{ return "Lego"; }

IgLegoBrowser::IgLegoBrowser (IgState *state,
			      IgSite *site,
			      IgLegoModel *model)
    : Ig3DBaseBrowser (state,
		       site,
		       model ? model : new IgLegoModel (state))
{
    init ();
}

void
IgLegoBrowser::init (void)
{
    SoQtExaminerViewer::setCameraType (SoOrthographicCamera::getClassTypeId ());
    SoOrthographicCamera * const camera = dynamic_cast <SoOrthographicCamera *> (this->getCamera ());
    if (!camera) return; // probably a scene-less viewer

    camera->position.setValue (7.2122893, 4.4723949, 2.5151112);
    camera->orientation.setValue (-0.53453183, 0.81074113, 0.23869397, 1.1617218);
    camera->aspectRatio = 1.0;
    camera->focalDistance = 6.5153651;
    camera->height = 13.03073;
    camera->nearDistance = 0.1;
    camera->farDistance = 32767;

    setBackgroundColor (SbColor (1.0, 1.0, 1.0));
}

QWidget *
IgLegoBrowser::buildLeftTrim (QWidget *parent)
{
    QWidget	*result = new QWidget (parent);
    result->setFixedWidth (0);
    return result;    
}

QWidget *
IgLegoBrowser::buildBottomTrim (QWidget *parent)
{
    QWidget	*result = new QWidget (parent);
    result->setFixedHeight (0);
    return result;    
}

QWidget *
IgLegoBrowser::buildRightTrim (QWidget *parent)
{
    QWidget	*result = new QWidget (parent);
    result->setFixedWidth (0);
    return result;    
}

void
IgLegoBrowser::buildDecoration (QWidget *parent)
{
    Ig3DBaseBrowser::buildDecoration (parent);    
    return;    
}

void
IgLegoBrowser::toggleCameraType (void)
{
    SoQtExaminerViewer::toggleCameraType ();
    Ig3DBaseBrowser::toggleCameraType ();    
}

void
IgLegoBrowser::invertCamera (void)
{
    SoCamera * const camera = this->getCamera ();
    if (!camera) return; // probably a scene-less viewer

    camera->position = camera->position.getValue () * -1.0F;    
}

void
IgLegoBrowser::viewPlaneX (void)
{
    SoCamera * const camera = this->getCamera ();
    if (!camera) return; // probably a scene-less viewer
    
    camera->position = SbVec3f (-1, 0, 0) * camera->position.getValue ().length ();
    camera->orientation = SbRotation (SbVec3f (0, 1, 0), -M_PI / 2.f);
}

void
IgLegoBrowser::viewPlaneY (void)
{
    SoCamera * const camera = this->getCamera ();
    if (!camera) return; // probably a scene-less viewer

    SbVec3f norient = SbVec3f (0, -1, 0);
    camera->position = -norient * camera->position.getValue().length();
    camera->orientation = SbRotation (SbVec3f (0, 0, 1), -M_PI / 2.f) * 
                          SbRotation (SbVec3f (0, 0, -1), norient);
}

void
IgLegoBrowser::viewPlaneZ (void)
{
    SoCamera * const camera = this->getCamera ();
    if (!camera) return; // probably a scene-less viewer
  
    camera->position = SbVec3f (0, 0, 1) * camera->position.getValue ().length ();
    camera->orientation = SbRotation::identity ();
}
