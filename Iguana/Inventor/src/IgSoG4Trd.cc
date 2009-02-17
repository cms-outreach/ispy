//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Inventor/interface/IgSoG4Trd.h"
#include <Inventor/nodes/SoIndexedFaceSet.h>
#include <Inventor/nodes/SoVertexProperty.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

SO_KIT_SOURCE (IgSoG4Trd);

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

void
IgSoG4Trd::initClass (void)
{ SO_KIT_INIT_CLASS (IgSoG4Trd, IgSoShapeKit, "IgSoShapeKit"); }

IgSoG4Trd::IgSoG4Trd (void)
{
    SO_KIT_CONSTRUCTOR (IgSoG4Trd);
    SO_KIT_ADD_FIELD (dx1, (1.0));
    SO_KIT_ADD_FIELD (dx2, (1.0));
    SO_KIT_ADD_FIELD (dy1, (1.0));
    SO_KIT_ADD_FIELD (dy2, (1.0));
    SO_KIT_ADD_FIELD (dz, (1.0));
    SO_KIT_ADD_CATALOG_ENTRY (faces, SoIndexedFaceSet, FALSE, separator,\x0, TRUE);
    SO_KIT_INIT_INSTANCE ();
    setUpConnections (true, true);
}

void
IgSoG4Trd::refresh (void)
{
    // FIXME: Only do non-zero faces

    static const int	NPOINTS = 8;
    static const int	NFACES = 6;
    static const int	NINDICES = NFACES*5;

    SoIndexedFaceSet	*faces = new SoIndexedFaceSet;
    SoVertexProperty	*vtx = new SoVertexProperty;

    // Indices for the eight faces
    static const int	indices [NINDICES] = {
	3,2,1,0, SO_END_FACE_INDEX, // bottom
	4,5,6,7, SO_END_FACE_INDEX, // top
	0,1,5,4, SO_END_FACE_INDEX, 
	1,2,6,5, SO_END_FACE_INDEX,
	2,3,7,6, SO_END_FACE_INDEX,
	3,0,4,7, SO_END_FACE_INDEX
    };

    float		points [NPOINTS][3] = {
	{  dx1.getValue (),  dy1.getValue (), -dz.getValue () },
	{ -dx1.getValue (),  dy1.getValue (), -dz.getValue () },
	{ -dx1.getValue (), -dy1.getValue (), -dz.getValue () },
	{  dx1.getValue (), -dy1.getValue (), -dz.getValue () },
	{  dx2.getValue (),  dy2.getValue (),  dz.getValue () },
	{ -dx2.getValue (),  dy2.getValue (),  dz.getValue () },
	{ -dx2.getValue (), -dy2.getValue (),  dz.getValue () },
	{  dx2.getValue (), -dy2.getValue (),  dz.getValue () }
    };

    float t1 = atan ((dx2.getValue ()-dx1.getValue ()) / (2*dz.getValue ()));
    float t2 = atan ((dy2.getValue ()-dy1.getValue ()) / (2*dz.getValue ()));
    float st1 = sin (t1);
    float st2 = sin (t2);
    float ct1 = cos (t1);
    float ct2 = cos (t2);

    float		normals [NFACES][3] = {
	{    0,    0,   -1 },
	{    0,    0,    1 },
	{    0,  ct2, -st2 },
	{ -ct1,    0, -st1 },
	{    0, -ct2, -st2 },
	{  ct1,    0, -st1 }
    };

    for (int i = 0; i < NPOINTS; ++i)
	vtx->vertex.set1Value (i, points[i][0], points[i][1], points[i][2]);

    for (int i = 0; i < NFACES; ++i)
	vtx->normal.set1Value (i, normals[i][0], normals[i][1], normals[i][2]);

    vtx->normalBinding = SoVertexProperty::PER_FACE;
    faces->coordIndex.setValues (0, NINDICES, indices);
    faces->vertexProperty = vtx;

    // Set parts
    setPart ("faces", faces);
}
