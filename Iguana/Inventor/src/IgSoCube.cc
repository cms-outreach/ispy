//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Inventor/interface/IgSoCube.h"
#include <Inventor/nodes/SoIndexedFaceSet.h>
#include <Inventor/nodes/SoIndexedLineSet.h>
#include <Inventor/nodes/SoVertexProperty.h>
#include <Inventor/nodes/SoShapeHints.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

SO_KIT_SOURCE (IgSoCube);

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

void
IgSoCube::initClass (void)
{ SO_KIT_INIT_CLASS (IgSoCube, IgSoShapeKit, "IgSoShapeKit"); }

IgSoCube::IgSoCube (void)
{
    SO_KIT_CONSTRUCTOR (IgSoCube);
    SO_KIT_ADD_FIELD (width,		(1.0));
    SO_KIT_ADD_FIELD (length,		(1.0));
    SO_KIT_ADD_FIELD (thickness,	(1.0));
    SO_KIT_ADD_CATALOG_ENTRY (hints, SoShapeHints, FALSE, separator,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY (faces, SoIndexedFaceSet, FALSE, separator,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY (lines, SoIndexedLineSet, FALSE, separator,\x0, TRUE);
    SO_KIT_INIT_INSTANCE ();
    setUpConnections (true, true);
}

void
IgSoCube::refresh (void)
{
    SoShapeHints	*hints = new SoShapeHints;
    SoIndexedFaceSet	*faces = new SoIndexedFaceSet;
    SoIndexedLineSet	*lines = new SoIndexedLineSet;
    SoVertexProperty	*vtx = new SoVertexProperty;
    float		x = width.getValue ();
    float		y = length.getValue ();
    float		z = thickness.getValue ();

    // Set shape hints
    hints->vertexOrdering = SoShapeHints::CLOCKWISE;
    hints->shapeType = SoShapeHints::SOLID;
    hints->faceType = SoShapeHints::CONVEX;

    // Prepare coordinates and normals into the vertex property
    SbVec3f points [8] = {
	SbVec3f (-0.5f * x, -0.5f * y,  0.5f * z),	// (-x, -y,  z)
	SbVec3f (-0.5f * x,  0.5f * y,  0.5f * z),	// (-x,  y,  z)
	SbVec3f ( 0.5f * x,  0.5f * y,  0.5f * z),	// ( x,  y,  z)
	SbVec3f ( 0.5f * x, -0.5f * y,  0.5f * z),	// ( x, -y,  z)

	SbVec3f ( 0.5f * x, -0.5f * y, -0.5f * z),	// ( x, -y, -z)
	SbVec3f ( 0.5f * x,  0.5f * y, -0.5f * z),	// ( x,  y, -z)
	SbVec3f (-0.5f * x,  0.5f * y, -0.5f * z),	// (-x,  y, -z)
	SbVec3f (-0.5f * x, -0.5f * y, -0.5f * z)	// (-x, -y, -z)
    };
    
    SbVec3f normals [6] = {
	SbVec3f ( 0,  0,  1),
	SbVec3f ( 1,  0,  0),
	SbVec3f ( 0,  0, -1),
	SbVec3f (-1,  0,  0),
	SbVec3f ( 0,  1,  0),
	SbVec3f ( 0, -1,  0)
    };

    static const int faceIndices [] = {
	// each face has a quad + a stop flag
	0, 1, 2, 3, SO_END_FACE_INDEX, // +z face
	3, 2, 5, 4, SO_END_FACE_INDEX, // +x face
	4, 5, 6, 7, SO_END_FACE_INDEX, // -z face
	7, 6, 1, 0, SO_END_FACE_INDEX, // -x face
	1, 6, 5, 2, SO_END_FACE_INDEX, // +y face
	0, 3, 4, 7, SO_END_FACE_INDEX  // -y face
    };

    static int lineIndices [] = {
	// the first line does bottom (-y) and left (-x) sides,
	// second back (-z) and final the right (+x)
        0, 1, 2, 3, 0, 7, 6, 1, SO_END_LINE_INDEX,
        2, 5, 4, 3, SO_END_LINE_INDEX,
        4, 7, SO_END_LINE_INDEX,
        5, 6, SO_END_LINE_INDEX
    };

    vtx->vertex.setValues (0, 8, points);
    vtx->normal.setValues (0, 6, normals);
    vtx->normalBinding = SoVertexProperty::PER_PART;
    vtx->materialBinding = SoVertexProperty::OVERALL;

    // Set faces and lines
    faces->coordIndex.setValues (0, 6 * 5, faceIndices);
    faces->vertexProperty = vtx;

    lines->coordIndex.setValues (0, 9 + 5 + 3 + 3, lineIndices);
    lines->vertexProperty = vtx;

    setPart ("hints", hints);
    setPart ("faces", faces);
    setPart ("lines", lines);
}
