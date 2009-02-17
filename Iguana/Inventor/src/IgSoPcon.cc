//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Inventor/interface/IgSoPcon.h"
#include <Inventor/nodes/SoNurbsSurface.h>
#include <Inventor/nodes/SoFaceSet.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoVertexProperty.h>
#include <Inventor/nodes/SoShapeHints.h>
#include <Inventor/nodes/SoCoordinate4.h>
#include <Inventor/nodes/SoTranslation.h>
#include <algorithm>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
#ifdef WIN32
#define copysign _copysign
#endif

//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>

SO_KIT_SOURCE (IgSoPcon);

//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

void
IgSoPcon::initClass (void)
{ SO_KIT_INIT_CLASS (IgSoPcon, IgSoShapeKit, "IgSoShapeKit"); }

IgSoPcon::IgSoPcon (void)
{
    SO_KIT_CONSTRUCTOR (IgSoPcon);
    SO_KIT_ADD_FIELD (vertices, (SbVec3f (0,0,0)));
    SO_KIT_ADD_FIELD (center, (SbVec3f (0,0,0)));
    SO_KIT_ADD_FIELD (phiStart, (0));
    SO_KIT_ADD_FIELD (phiDelta, ( (2*M_PI)));

    SbVec3f defPcon [] = {
	SbVec3f (0.2F, 0.F, -1.F),
	SbVec3f (1.F,  0.F, -0.8F),
	SbVec3f (1.F,  0.F,  0.8F),
	SbVec3f (0.2F, 0.F,  1.F)
    };
    vertices.setValues (0, 4, defPcon);

    SO_KIT_ADD_CATALOG_ENTRY (translation,  SoTranslation,  FALSE, separator,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY (outerCoords,  SoCoordinate4,  FALSE, separator,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY (outerSurface, SoNurbsSurface, FALSE, separator,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY (innerCoords,  SoCoordinate4,  FALSE, separator,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY (innerSurface, SoNurbsSurface, FALSE, separator,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY (topCoords,    SoCoordinate4,  FALSE, separator,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY (topSurface,   SoNurbsSurface, FALSE, separator,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY (bottomCoords, SoCoordinate4,  FALSE, separator,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY (bottomSurface,SoNurbsSurface, FALSE, separator,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY (side1Rot,     SoTransform,    TRUE, separator,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY (side1Hints,   SoShapeHints,   TRUE, separator,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY (side1Face,    SoFaceSet,      TRUE, separator,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY (side2Rot,     SoTransform,    TRUE, separator,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY (side2Hints,   SoShapeHints,   TRUE, separator,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY (side2Face,    SoFaceSet,      TRUE, separator,\x0, TRUE);
    SO_KIT_INIT_INSTANCE ();
    setUpConnections (true, true);
}

void
IgSoPcon::refresh (void)
{
    if (vertices.getNum () <= 1)
    {	
	// set parts
	setPart ("translation",	NULL);
	setPart ("outerCoords",	NULL);
	setPart ("outerSurface", NULL);
	setPart ("innerCoords",	NULL);
	setPart ("innerSurface", NULL);
	setPart ("topCoords", NULL);
	setPart ("topSurface", NULL);
	setPart ("bottomCoords", NULL);
	setPart ("bottomSurface", NULL);
	return;
    }
    
    //  We generate a NURB surface to represent a surface of rotation
    //	The v dimension represents the rotation. The u represents the cross section.
    //
    //  Number of control points = DIM.
    //	DIM = 7, ORDER = 3 (quadratic) (minimum required: COIN needs more)
    //	Number of knots = DIM+ORDER=10.
    //	It requires 2*nz + 1 to describe a closed polygon:
    //	DIM = 2*nz+1, ORDER = 2 (linear)
    //  Number of knots = DIM+ORDER = 2*nz+1+2

    //  Outer & inner surfaces are completely equivalent in structure

    SoTranslation	*translation = new SoTranslation;
    SoCoordinate4	*outerCoords = new SoCoordinate4;
    SoNurbsSurface	*outerSurface = new SoNurbsSurface;
    SoCoordinate4	*innerCoords = new SoCoordinate4;
    SoNurbsSurface	*innerSurface = new SoNurbsSurface;
    SoCoordinate4	*topCoords = new SoCoordinate4;
    SoNurbsSurface	*topSurface = new SoNurbsSurface;
    SoCoordinate4	*bottomCoords = new SoCoordinate4;
    SoNurbsSurface	*bottomSurface = new SoNurbsSurface;
    SoTransform		*side1Rot = new SoTransform;
    SoShapeHints	*side1Hints = new SoShapeHints;
    SoFaceSet		*side1Face = new SoFaceSet;
    SoTransform		*side2Rot = new SoTransform;
    SoShapeHints	*side2Hints = new SoShapeHints;
    SoFaceSet		*side2Face = new SoFaceSet;

    float		phiStart = this->phiStart.getValue ();
    float		phiDelta = this->phiDelta.getValue ();
    int			uDim = vertices.getNum () / 2;
#ifdef TGS_VERSION
    static const float maxNurbAngle =  M_PI / 2.; // max angle to cover with 3 control points
#else
    static const float maxNurbAngle =  M_PI / 8.; 
#endif
    const int nurbSegs = std::max<int>(1,static_cast<int>(phiDelta/maxNurbAngle));
    const int	V_DIM = 2*nurbSegs+1; // the dimension of the surfaces around the direction of revolution
    static const int	V_ORDER = 3; // the order of the surface (quadratic)
    static const int	U_ORDER = 2; // the order of the surface along the z axis (linear)

    outerSurface->numUControlPoints = uDim;
    outerSurface->numVControlPoints = V_DIM;
    innerSurface->numUControlPoints = uDim;
    innerSurface->numVControlPoints = V_DIM;

    //  Generate the knot vectors
    //  For v, this should be {0,0,0,1,1,2,2,3,3,3}
    int iu, iv, ik, ik_count;
    std::vector<float> u_knot_vals(U_ORDER+uDim);
    std::vector<float> v_knot_vals(V_ORDER+V_DIM);

    //	Do the first point ORDER times	
    for (iv = 0, ik = 0, ik_count = 0; iv < V_ORDER; iv++, ik++)
	v_knot_vals[ik] = static_cast<float>(ik_count);
    ik_count++;

    //	All but the end point get ORDER-1
    for (iv = 0; iv < (V_DIM+V_ORDER-2*V_ORDER)/(V_ORDER-1); iv++, ik_count++)
	for (int j = 0; j < (V_ORDER-1); j++, ik++)
	    v_knot_vals[ik] = static_cast<float>(ik_count);

    //	Now the last point, ORDER times
    for (iv = 0; iv < V_ORDER; iv++, ik++)
	v_knot_vals[ik] = static_cast<float>(ik_count);

    //	Now for u
    for (iu = 0, ik = 0, ik_count = 0; iu < U_ORDER; iu++, ik++) 
	u_knot_vals[ik] = static_cast<float>(ik_count);
    ik_count++;

    for (iu = 0; iu < (uDim+U_ORDER-2*U_ORDER)/(U_ORDER-1); iu++, ik_count++)
	for (int j = 0; j < (U_ORDER-1); j++, ik++)
	    u_knot_vals[ik] = static_cast<float>(ik_count);

    for (iu = 0; iu < U_ORDER; iu++, ik++)
	u_knot_vals[ik] = static_cast<float>(ik_count);

    //  Should look like {0,0,1,2,3,...,n-1,n,n}
    outerSurface->uKnotVector.setValues (0,uDim+U_ORDER,&u_knot_vals[0]);
    outerSurface->vKnotVector.setValues (0,V_DIM+V_ORDER,&v_knot_vals[0]);
    innerSurface->uKnotVector.setValues (0,uDim+U_ORDER,&u_knot_vals[0]);
    innerSurface->vKnotVector.setValues (0,V_DIM+V_ORDER,&v_knot_vals[0]);
	
    /*  Set the number of co-ordinates... */
    outerCoords->point.setNum (uDim*V_DIM);
    innerCoords->point.setNum (uDim*V_DIM);

    /*  Generate the co-ordinates (Control Points) */

    /*  Set up the basic grid of control points */
    for (iv = 0; iv < V_DIM; iv++)
    {
	float		angleV = phiStart + phiDelta - iv * phiDelta/(2*nurbSegs);
	SbRotation	rotator (SbVec3f (0,0,1.F), angleV);
	float		weightV = iv % 2 == 1 ? fabs (cos (phiDelta/(2*nurbSegs))) : 1.0F;

	for (iu = 0; iu < uDim; iu++)
	{
	    int		i = iu + iv*uDim;
	    float	x;
	    float	y;
	    float	z;
	    SbVec3f	cpt;

	    // get the order this way to set exterior of surf
	    vertices [iu].getValue (x, y, z);
	    cpt = SbVec3f ((x / weightV), y, z);
	    rotator.multVec (cpt, cpt);
	    cpt *= (weightV);
	    innerCoords->point.set1Value (i, cpt[0], cpt[1], cpt[2], (weightV));

	    vertices [iu+uDim].getValue (x, y, z);
	    cpt = SbVec3f ((x / weightV), y, z);
	    rotator.multVec (cpt, cpt);
	    cpt *= (weightV);
	    outerCoords->point.set1Value (i, cpt[0], cpt[1], cpt[2], (weightV));
	}
    }

    //  We instituted an additional surface at the top and bottom to try and
    //  minimize the blurring at the intersections when the faces are at large
    //  angles wrt one another when using the G5G (TGS OIV) NURB libraries.
    //  Coin apparently doesn't have this problem.

    //  now for end plates; again u is linear (this time in r) and v
    //  is quadratic (the rotation angle)
    //  vend should look like vend [10] = { 0, 0, 0, 1, 1, 2, 2, 3, 3, 3 };

    float	uend [4] = { 0, 0, 1, 1 };

    bottomCoords->point.setNum (12);
    topCoords->point.setNum (12);
    bottomSurface->numUControlPoints = 2;
    bottomSurface->numVControlPoints = V_DIM;
    bottomSurface->uKnotVector.setValues (0, 4, &uend[0]);
    bottomSurface->vKnotVector.setValues (0, V_DIM+V_ORDER,&v_knot_vals[0]);
    topSurface->numUControlPoints = 2;
    topSurface->numVControlPoints = V_DIM;
    topSurface->uKnotVector.setValues (0, 4, &uend[0]);
    topSurface->vKnotVector.setValues (0, V_DIM+V_ORDER, &v_knot_vals[0]);

    int iu_indices [4] = { uDim-1, uDim, 2*uDim-1, 0}; // flip top end to get orientation
    for (iv = 0; iv < V_DIM; iv++)
    {
	float		angleV = phiStart + phiDelta - iv * phiDelta/(2*nurbSegs);
	SbRotation	rotator (SbVec3f (0,0,1.F), (angleV));
	float		weightV = iv % 2 == 1 ? fabs (cos (phiDelta/(2*nurbSegs))) : 1.0F;

	for (iu = 0; iu < 2; iu++)
	{  
	    int		i = iu + iv*2;
	    float	x;
	    float	y;
	    float	z;
	    SbVec3f	cpt;

	    vertices [iu_indices [iu]].getValue (x, y, z);
	    cpt = SbVec3f ((x / weightV), (y / weightV), z);
	    rotator.multVec (cpt, cpt);
	    cpt *= (weightV);
	    bottomCoords->point.set1Value (i, cpt[0], cpt[1], cpt[2], (weightV));

	    vertices [iu_indices [iu+2]].getValue (x, y, z);
	    cpt = SbVec3f ((x / weightV), (y / weightV), z);
	    rotator.multVec (cpt, cpt);
	    cpt *= (weightV);
	    topCoords->point.set1Value (i, cpt[0], cpt[1], cpt[2], (weightV));
	}
    }

    translation->translation = center;

    // set parts
    setPart ("translation",	translation);
    setPart ("outerCoords",	outerCoords);
    setPart ("outerSurface",	outerSurface);
    setPart ("innerCoords",	innerCoords);
    setPart ("innerSurface",	innerSurface);
    setPart ("topCoords",	topCoords);
    setPart ("topSurface",	topSurface);
    setPart ("bottomCoords",	bottomCoords);
    setPart ("bottomSurface",	bottomSurface);

    //	Now check to see if we need cut faces
    if (! (fabs (phiDelta - 2*M_PI) < 0.0001 || phiDelta < 0.0001))
    {
	SoVertexProperty *sideVtx = new SoVertexProperty;
	sideVtx->vertex = vertices;
	side1Face->numVertices = 2*uDim;
	side2Face->numVertices = 2*uDim;
	side1Face->vertexProperty = sideVtx;
	side2Face->vertexProperty = sideVtx;
	side1Rot->rotation.setValue (SbVec3f (0.,0.,1.), phiStart);
	side2Rot->rotation.setValue (SbVec3f (0.,0.,1.), phiDelta);
	side1Hints->vertexOrdering = SoShapeHints::CLOCKWISE;
	side1Hints->shapeType = SoShapeHints::UNKNOWN_SHAPE_TYPE;
	side1Hints->faceType = SoShapeHints::UNKNOWN_FACE_TYPE;
	side2Hints->vertexOrdering = SoShapeHints::COUNTERCLOCKWISE;
	side2Hints->shapeType = SoShapeHints::UNKNOWN_SHAPE_TYPE;
	side2Hints->faceType = SoShapeHints::UNKNOWN_FACE_TYPE;
	setPart ("side1Rot",	side1Rot);
	setPart ("side1Hints",	side1Hints);
	setPart ("side1Face",	side1Face);
	setPart ("side2Rot",	side2Rot);
	setPart ("side2Hints",	side2Hints);
	setPart ("side2Face",	side2Face);
    }

}

// Use this routine to order SbVec3f's by the z co-ordinate
bool
vec3fCompare (const SbVec3f &v1, const SbVec3f &v2)
{
    float x1,y1,z1, x2,y2,z2;
    v1.getValue (x1,y1,z1);
    v2.getValue (x2,y2,z2);
    return z1 > z2;
}

// Use this routine to order SbVec3f's by the reverse of the z co-ordinate
bool
vec3fCompareReverse (const SbVec3f &v1, const SbVec3f &v2)
{
    float x1,y1,z1, x2,y2,z2;
    v1.getValue (x1,y1,z1);
    v2.getValue (x2,y2,z2);
    return z1 < z2;
}

/** GEANT3 style arguments
    @a nzee: number of z breaks
    @a phi: starting angle for the surfaces of revolution (same as startAngle field)
    @a delPhi: range of revolution (same as deltaAngle)
    @a zvals: array of the z locations of the breaks
    @a rmin: r value for the inner surface of revolution at the z points given in \a zvals
    @a rmax: same as rmin but for the outer surface */
void
IgSoPcon::initialise (const int nzee, const float phi, const float delPhi, 
		      const float *zvals, const float *rmin, const float *rmax)
{
    // Set up some typedefs so if we change the object we don't have
    // to change the code.
    typedef std::vector<SbVec3f> OrderedPair;
    typedef OrderedPair::iterator OrderedPairIt;
    using std::sort;

    assert (nzee > 1);

    phiStart = phi;
    if (fabs(delPhi) > 2*M_PI)
    {
	phiDelta = static_cast<float>(copysign(2*M_PI,delPhi));
    }
    else
    {
	phiDelta = delPhi;
    }

    // Sort the incoming points so that the path is counterclockwise
    // viewed along the y axis; start with upmost inner z and work
    // down, then up.
    OrderedPair pts (2*nzee+1);
    for (int i = 0; i < nzee; i++)
    {
	pts [i] = SbVec3f (rmin[i], 0, zvals[i]);
	pts [i+nzee] = SbVec3f (rmax[nzee-i-1], 0, zvals[nzee-i-1]);
    }

    // Order the points: first rmin, then rmax; then invert the rmax
    // set to get a loop.  This ordering doesn't seem to do what it is
    // supposed to, but everything has been jiggered to use the result
    // so we're not touching it for now.

    OrderedPairIt startDown = pts.begin ();
    OrderedPairIt endDown = startDown+nzee;
    OrderedPairIt startUp = endDown;
    OrderedPairIt endUp = startUp+nzee;

    sort (startDown, endDown, vec3fCompare);
    sort (startUp, endUp, vec3fCompareReverse);

    pts [2*nzee] = pts [0];

    // Stuff the points into the vertex list
    vertices.deleteValues (0);
    vertices.setValues (0, 2*nzee+1, &pts[0]);
}
