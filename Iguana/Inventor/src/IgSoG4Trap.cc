//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Inventor/interface/IgSoG4Trap.h"
#include <Inventor/nodes/SoIndexedFaceSet.h>
#include <Inventor/nodes/SoVertexProperty.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

SO_KIT_SOURCE (IgSoG4Trap);

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

void
IgSoG4Trap::initClass (void)
{ SO_KIT_INIT_CLASS (IgSoG4Trap, IgSoShapeKit, "IgSoShapeKit"); }

IgSoG4Trap::IgSoG4Trap (void)
{
    SO_KIT_CONSTRUCTOR (IgSoG4Trap);
    SO_KIT_ADD_FIELD (dz,    (1.0));
    SO_KIT_ADD_FIELD (theta, (0.0));
    SO_KIT_ADD_FIELD (phi,   (0.0));
    SO_KIT_ADD_FIELD (dy1,   (1.0));
    SO_KIT_ADD_FIELD (dx1,   (1.0));
    SO_KIT_ADD_FIELD (dx2,   (1.0));
    SO_KIT_ADD_FIELD (dy2,   (1.0));
    SO_KIT_ADD_FIELD (dx3,   (1.0));
    SO_KIT_ADD_FIELD (dx4,   (1.0));
    SO_KIT_ADD_FIELD (alp1,  (0.0));
    SO_KIT_ADD_FIELD (alp2,  (0.0));
    SO_KIT_ADD_CATALOG_ENTRY (faces, SoIndexedFaceSet, FALSE, separator,\x0, TRUE);
    SO_KIT_INIT_INSTANCE ();
    setUpConnections (true, true);
}

void
IgSoG4Trap::refresh (void)
{
    // FIXME: Only do non-zero faces
    static const int	NPOINTS = 8;
    static const int	NFACES = 6;
    static const int	NINDICES = NFACES*5;

    SoIndexedFaceSet	*faces = new SoIndexedFaceSet;
    SoVertexProperty	*vtx = new SoVertexProperty;
    float		dy1TAlp1 = dy1.getValue () * tan (alp1.getValue ());
    float		dy2TAlp2 = dy2.getValue () * tan (alp2.getValue ());
    float		dzTThetaCPhi = dz.getValue ()
				       * tan (theta.getValue ())
				       * cos (phi.getValue ());
    float		dzTThetaSPhi = dz.getValue ()
				       * tan (theta.getValue ())
				       * sin (phi.getValue ());
    static const int	indices [NINDICES] = { 3,2,1,0, SO_END_FACE_INDEX,
					       4,5,6,7, SO_END_FACE_INDEX,
					       0,1,5,4, SO_END_FACE_INDEX, 
					       1,2,6,5, SO_END_FACE_INDEX,
					       2,3,7,6, SO_END_FACE_INDEX,
					       3,0,4,7, SO_END_FACE_INDEX };
    float		points [NPOINTS][3] = {
	{  dx2.getValue ()+dy1TAlp1 - dzTThetaCPhi,  dy1.getValue () - dzTThetaSPhi, -dz.getValue () },
	{ -dx2.getValue ()+dy1TAlp1 - dzTThetaCPhi,  dy1.getValue () - dzTThetaSPhi, -dz.getValue () },
	{ -dx1.getValue ()-dy1TAlp1 - dzTThetaCPhi, -dy1.getValue () - dzTThetaSPhi, -dz.getValue () },
	{  dx1.getValue ()-dy1TAlp1 - dzTThetaCPhi, -dy1.getValue () - dzTThetaSPhi, -dz.getValue () },
	{  dx4.getValue ()+dy2TAlp2 + dzTThetaCPhi,  dy2.getValue () + dzTThetaSPhi,  dz.getValue () },
	{ -dx4.getValue ()+dy2TAlp2 + dzTThetaCPhi,  dy2.getValue () + dzTThetaSPhi,  dz.getValue () },
	{ -dx3.getValue ()-dy2TAlp2 + dzTThetaCPhi, -dy2.getValue () + dzTThetaSPhi,  dz.getValue () },
	{  dx3.getValue ()-dy2TAlp2 + dzTThetaCPhi, -dy2.getValue () + dzTThetaSPhi,  dz.getValue () }
    };

    for (int i = 0; i < NPOINTS; ++i)
	vtx->vertex.set1Value (i, points[i][0], points[i][1], points[i][2]);

    for (int i = 0; i < 6; ++i)
    {
	SbVec3f p0 = vtx->vertex [indices [5*i + 0]]; 
	SbVec3f p1 = vtx->vertex [indices [5*i + 1]]; 
	SbVec3f p2 = vtx->vertex [indices [5*i + 2]]; 
	SbVec3f normal = (p1-p0).cross (p2-p0);
	normal.normalize ();
	vtx->normal.set1Value (i, normal);
    }

    vtx->normalBinding = SoVertexProperty::PER_FACE;
    faces->coordIndex.setValues (0, NINDICES, indices);
    faces->vertexProperty = vtx;

    // Set parts
    setPart ("faces", faces);
}
