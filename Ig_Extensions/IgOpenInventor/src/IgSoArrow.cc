//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Extensions/IgOpenInventor/interface/IgSoArrow.h"
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoRotation.h>
#include <Inventor/nodes/SoRotationXYZ.h>
#include <Inventor/nodes/SoCylinder.h>
#include <Inventor/nodes/SoCone.h>
#include <Inventor/nodes/SoScale.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

SO_KIT_SOURCE (IgSoArrow);

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

void
IgSoArrow::initClass ()
{ SO_KIT_INIT_CLASS (IgSoArrow, IgSoShapeKit, "IgSoShapeKit"); }

IgSoArrow::IgSoArrow (void)
{
    SO_KIT_CONSTRUCTOR (IgSoArrow);

    SO_KIT_ADD_FIELD (tail, (0, 0, 0));
    SO_KIT_ADD_FIELD (tip, (0, 1, 0));

    SO_KIT_ADD_CATALOG_ENTRY (finalTranslation,	SoTranslation,	FALSE, separator,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY (scale,		SoScale,	FALSE, separator,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY (myRotation,	SoRotation,	FALSE, separator,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY (rot,		SoRotationXYZ,	FALSE, separator,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY (zTranslation,	SoTranslation,	FALSE, separator,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY (axisCyl,		SoCylinder,	FALSE, separator,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY (cTranslation,	SoTranslation,	FALSE, separator,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY (axisCone,		SoCone,		FALSE, separator,\x0, TRUE);
    SO_KIT_INIT_INSTANCE ();
    setUpConnections (true, true);
}

void
IgSoArrow::refresh (void)
{
    static const float	ARROWSIZE = 0.02F;
    SoTranslation	*finalTranslation    = new SoTranslation;
    SoScale		*scale               = new SoScale;
    SoRotation		*myRotation          = new SoRotation;
    SoRotationXYZ	*rot                 = new SoRotationXYZ;
    SoTranslation	*zTranslation        = new SoTranslation;
    SoCylinder		*axisCyl             = new SoCylinder;
    SoTranslation	*cTranslation        = new SoTranslation;
    SoCone		*axisCone            = new SoCone;
    SbVec3f		mtail = tail.getValue ();
    SbVec3f		mtip  = tip.getValue ();
    SbVec3f		mdiff = mtip - mtail;
    float		length = mdiff.length ();
	SbVec3f     axisDirection(0.F, 1.F, 0.F); // construct along y-axis

    finalTranslation->translation.setValue (mtail);
    scale->scaleFactor.setValue (length/2, length/2, length/2);
	myRotation->rotation.setValue (SbRotation(axisDirection, mdiff));
    zTranslation->translation.setValue (0, 1, 0);
    rot->axis = SoRotationXYZ::X;
    rot->angle = (mdiff[2] < 0.F) ? -M_PI/2. : M_PI/2.;
    axisCyl->radius.setValue (2 * ARROWSIZE);
    cTranslation->translation.setValue (0, 1, 0);
    axisCone->bottomRadius.setValue (4 * ARROWSIZE);
    axisCone->height.setValue (8 * ARROWSIZE);

    setPart ("finalTranslation", finalTranslation);
    setPart ("scale", scale);
    setPart ("myRotation", myRotation);
    setPart ("rot", rot);
    setPart ("zTranslation", zTranslation);
    setPart ("axisCyl", axisCyl);
    setPart ("cTranslation", cTranslation);
    setPart ("axisCone", axisCone);
}
