//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Inventor/interface/IgSoG4Torus.h"
#include <Inventor/nodes/SoNurbsSurface.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoCoordinate4.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>

#define	SO_TORUS_U_DIM	7
#define	SO_TORUS_V_DIM	7
#define	SO_TORUS_U_ORDER 3
#define	SO_TORUS_V_ORDER 3

//<<<<<< PRIVATE CONSTANTS                                              >>>>>>

const int NPTS = SO_TORUS_U_DIM * SO_TORUS_V_DIM;
const float u_knot_vals [] = {0, 0, 0, 1, 1, 2, 2, 3, 3, 3};
const float v_knot_vals [] = {0, 0, 0, 1, 1, 2, 2, 3, 3, 3};

//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

SO_KIT_SOURCE (IgSoG4Torus);

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

void
IgSoG4Torus::initClass (void)
{ SO_KIT_INIT_CLASS (IgSoG4Torus, IgSoShapeKit, "IgSoShapeKit"); }

IgSoG4Torus::IgSoG4Torus (void)
{
    SO_KIT_CONSTRUCTOR (IgSoG4Torus);
    SO_KIT_ADD_FIELD (minRadius, (0.f));
    SO_KIT_ADD_FIELD (maxRadius, (1.f));
    SO_KIT_ADD_FIELD (phiStart,	 (0.0));
    SO_KIT_ADD_FIELD (phiDelta,	 (2 * M_PI));
    SO_KIT_ADD_CATALOG_ENTRY (torus,  SoSeparator, FALSE, separator,\x0, TRUE);
    SO_KIT_INIT_INSTANCE ();
    setUpConnections (true, true);
}

void
IgSoG4Torus::refresh (void)
{
    SoSeparator   *torSeparator   = new SoSeparator;
    SoCoordinate4 *torCoordinate4 = new SoCoordinate4;
    torCoordinate4->point.setNum (NPTS);
    torSeparator->addChild (torCoordinate4);

    //  Create the surface 
    SoNurbsSurface *torSurface = new SoNurbsSurface;
    torSurface->numUControlPoints.setValue (SO_TORUS_U_DIM);
    torSurface->numVControlPoints.setValue (SO_TORUS_V_DIM);
    torSurface->uKnotVector.setValues (0, SO_TORUS_U_DIM + SO_TORUS_U_ORDER, u_knot_vals);
    torSurface->vKnotVector.setValues (0, SO_TORUS_V_DIM + SO_TORUS_V_ORDER, v_knot_vals);
    torSeparator->addChild (torSurface);

    int iu, iv, i;
    double angleU, angleV;
    
    /*  Generate the Control Points */
    double weight, weightU, weightV;
    SbVec3f cpt;
    SbRotation rotator;
 
    /*  Set up the basic grid of control points */
    for (iv = 0, angleV = 0; iv < SO_TORUS_V_DIM; ++iv, angleV += M_PI/3) 
    {
	rotator.setValue (SbVec3f (0, 0, 1), (float) angleV);
	if (iv % 2 == 1) 
	{
	    weightV = 0.5;
	}
	else 
	{
	    weightV = 1;
	}
	for (iu = 0, angleU = 0; iu < SO_TORUS_U_DIM; ++iu, angleU -= M_PI/3) 
	{  
	    /* get the order this way to set exterior of surf */
	    i = iu + iv * SO_TORUS_U_DIM;
	    if (iu % 2 == 1) 
	    {
		weightU = 0.5;
	    }
	    else 
	    {
		weightU = 1;
	    }
	    
	    weight = weightU * weightV;
	    cpt = SbVec3f (maxRadius.getValue (), 0, 0) / (float) weightV + 
		  SbVec3f ((float)(minRadius.getValue () * cos (angleU) / weight),	
			   0,
			   (float)(minRadius.getValue () * sin (angleU) / weightU));
	    rotator.multVec (cpt, cpt);
	    cpt *= (float) weight;
	    torCoordinate4->point.set1Value (i, cpt [0], cpt [1], cpt [2], (float) weight);
	}
    }

    setPart ("torus", torSeparator);
}
