#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoNurbsCurve.h>
#include <Inventor/nodes/SoComplexity.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include "test_main.h"

// Create the nodes needed for the B-Spline curve.
SoSeparator *
makeCurve()
{
    // The control points for this curve

    SbVec3f pts[4] = {
	SbVec3f ( 4.0, -6.0,  6.0),
	SbVec3f (-4.0,  1.0,  0.0),
	SbVec3f (-1.5,  5.0, -6.0),
	SbVec3f ( 0.0,  2.0, -2.0)
    };
    
    
// 	SbVec3f ( 1.5,  5.0, -6.0)
// 	SbVec3f ( 4.0,  1.0,  0.0),
// 	SbVec3f (-4.0, -6.0,  6.0)};

    // The knot vector
    //    float knots[10] = {1, 2, 3, 4, 5, 5, 6, 7, 8, 9};
    float knots[7] = {1, 2, 3, 4, 5, 5, 6};

    SoSeparator *curveSep = new SoSeparator();
    curveSep->ref();

    // Set the draw style of the curve.
    SoDrawStyle *drawStyle  = new SoDrawStyle;
    drawStyle->lineWidth = 4;
    curveSep->addChild(drawStyle);

    // Define the NURBS curve including the control points
    // and a complexity.
    SoComplexity  *complexity = new SoComplexity;
    SoCoordinate3 *controlPts = new SoCoordinate3;
    SoNurbsCurve  *curve      = new SoNurbsCurve;
    complexity->value = 0.8;
    controlPts->point.setValues(0, 4, pts);
    curve->numControlPoints = 4;
    curve->knotVector.setValues(0, 7, knots);
    curveSep->addChild(complexity);
    curveSep->addChild(controlPts);
    curveSep->addChild(curve);

    curveSep->unrefNoDelete();
    return curveSep;
}

QString makeTest (SoSeparator *root)
{	
    SoSeparator  *curveSep  = makeCurve();
    root->addChild(curveSep);

    return "IGUANA: SoNurbsCurve Test";
}
