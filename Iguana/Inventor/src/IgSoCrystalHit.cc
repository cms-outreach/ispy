//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Inventor/interface/IgSoCrystalHit.h"
#include <Inventor/nodes/SoVertexProperty.h>
#include <Inventor/nodes/SoIndexedFaceSet.h>
#include <Inventor/nodes/SoIndexedLineSet.h>
#include <Inventor/nodes/SoShapeHints.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
 
SO_KIT_SOURCE (IgSoCrystalHit);

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

void
IgSoCrystalHit::initClass (void)
{ SO_KIT_INIT_CLASS (IgSoCrystalHit, IgSoShapeKit, "IgSoShapeKit"); }

IgSoCrystalHit::IgSoCrystalHit (void)
{
    SO_KIT_CONSTRUCTOR (IgSoCrystalHit);
    SO_KIT_ADD_FIELD (energy,		(0.1));
    SO_KIT_ADD_FIELD (minimumEnergy,	(0.001));  // 1 MeV 
    SO_KIT_ADD_FIELD (scale,		(1.0));
    SO_KIT_ADD_FIELD (relativeWidth,	(0.85));
 
    SO_KIT_ADD_FIELD (drawCrystal,	(TRUE));
    SO_KIT_ADD_FIELD (drawHit,		(TRUE));
    SO_KIT_ADD_FIELD (showLines,	(FALSE));
 
    SO_KIT_ADD_FIELD (front1,		(0.5,  0.5, 0.0));   // initialise to unit box in +z
    SO_KIT_ADD_FIELD (front2,		(-0.5,  0.5, 0.0));
    SO_KIT_ADD_FIELD (front3,		(-0.5, -0.5, 0.0));
    SO_KIT_ADD_FIELD (front4,		(0.5, -0.5, 0.0));

    SO_KIT_ADD_FIELD (back1,		(0.5,  0.5, 1.0));
    SO_KIT_ADD_FIELD (back2,		(-0.5,  0.5, 1.0));
    SO_KIT_ADD_FIELD (back3,		(-0.5, -0.5, 1.0));
    SO_KIT_ADD_FIELD (back4,		(0.5, -0.5, 1.0));

    SO_KIT_ADD_CATALOG_ENTRY (shapeHints, SoShapeHints, FALSE, separator,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY (faces,  SoIndexedFaceSet, FALSE, separator,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY (lines,  SoIndexedLineSet, FALSE, separator,\x0, TRUE);
    SO_KIT_INIT_INSTANCE ();
    setUpConnections (true, true);
}

void
IgSoCrystalHit::refresh (void)
{
    if (energy.getValue () < 0
	|| scale.getValue () < 0
	|| relativeWidth.getValue () < 0
	|| relativeWidth.getValue () > 1)
    {
	setPart ("shapeHints", NULL);
	setPart ("faces", NULL);
	setPart ("lines", NULL);
	return;
    }
    
    SoShapeHints *shapeHints = new SoShapeHints;
    shapeHints->vertexOrdering = SoShapeHints::COUNTERCLOCKWISE;
    shapeHints->shapeType = SoShapeHints::SOLID;
    shapeHints->faceType = SoShapeHints::CONVEX;

    SoIndexedFaceSet *faces = new SoIndexedFaceSet;

    // Crystal part of shape
    SbVec3f vertices [16];
    vertices [0] = front1.getValue ();
    vertices [1] = front2.getValue ();
    vertices [2] = front3.getValue ();
    vertices [3] = front4.getValue ();

    vertices [4] = back1.getValue ();
    vertices [5] = back2.getValue ();
    vertices [6] = back3.getValue ();
    vertices [7] = back4.getValue ();

    // Energy part

    // First find shape of crystal shrunk laterally according to
    // relativeWidth.  Take half off each side.  Then calculate
    // shrunken front and back planes coplanar with the original front
    // and back.  Move a tiny distance away along crystal axis from
    // the back face of the crystal so line around base is not lost.
    // Finally make the length of the hit proportional to the energy.
    float eScale = scale.getValue () * energy.getValue ();
    float delta = .5 * (1. + relativeWidth.getValue ());
    float epsilon = 0.0001;

    SbVec3f eFront1 = front1.getValue () + delta * (front3.getValue () - front1.getValue ());
    SbVec3f eFront2 = front2.getValue () + delta * (front4.getValue () - front2.getValue ());
    SbVec3f eFront3 = front3.getValue () + delta * (front1.getValue () - front3.getValue ());
    SbVec3f eFront4 = front4.getValue () + delta * (front2.getValue () - front4.getValue ());
	
    SbVec3f eBack1  = back1.getValue ()  + delta * (back3.getValue ()  - back1.getValue ());
    SbVec3f eBack2  = back2.getValue ()  + delta * (back4.getValue ()  - back2.getValue ());
    SbVec3f eBack3  = back3.getValue ()  + delta * (back1.getValue ()  - back3.getValue ());
    SbVec3f eBack4  = back4.getValue ()  + delta * (back2.getValue ()  - back4.getValue ());

    vertices [8]    = eBack1 + (eBack1 - eFront1) * epsilon;
    vertices [9]    = eBack2 + (eBack2 - eFront2) * epsilon;
    vertices [10]   = eBack3 + (eBack3 - eFront3) * epsilon;
    vertices [11]   = eBack4 + (eBack4 - eFront4) * epsilon;
    vertices [12]   = vertices [8]  + (eBack1 - eFront1) * eScale;
    vertices [13]   = vertices [9]  + (eBack2 - eFront2) * eScale;
    vertices [14]   = vertices [10] + (eBack3 - eFront3) * eScale;
    vertices [15]   = vertices [11] + (eBack4 - eFront4) * eScale;

    // Order vertices in each face so that right-handed normal points
    // outwards.  Order indices for composite (crystal+bump) face set.
    // 11 faces = (2 trapezoids * 6 faces) - (1 hidden one where they
    // join)

    static const int indices [11*5]  = {
	3,     2,    1,    0,	SO_END_FACE_INDEX,
	4,     5,    6,    7,	SO_END_FACE_INDEX,
	5,     1,    2,    6,	SO_END_FACE_INDEX,
	2,     3,    7,    6,	SO_END_FACE_INDEX,
	7,     3,    0,    4,	SO_END_FACE_INDEX,
	1,     5,    4,    0,	SO_END_FACE_INDEX, // end of crystal vertices: 6*5
	12,   13,   14,   15,   SO_END_FACE_INDEX,
	9,    10,   14,   13,	SO_END_FACE_INDEX,
	10,   11,   15,   14,   SO_END_FACE_INDEX,
	11,    8,   12,   15,   SO_END_FACE_INDEX,
	8,     9,   13,   12,	SO_END_FACE_INDEX
    };
 			         
    // Decide whether hit should be drawn based on user's desire and energy threshold
    SbBool drawHitPart
	= (drawHit.getValue ()
	   && energy.getValue () >= minimumEnergy.getValue ());

    // Draw crystal and/or hit if requested
    SoVertexProperty *vtx = new SoVertexProperty;
    if (drawCrystal.getValue ())
    {
	if (drawHitPart)
	{
	    // draw crystal and hit
	    vtx->vertex.setValues (0, 16, vertices); 
	    faces->coordIndex.setValues (0, 11*5, indices);
	}
	else
	{
	    // draw only crystal
	    vtx->vertex.setValues (0, 8, vertices); 
	    faces->coordIndex.setValues (0, 6*5, indices);
	}
    }
    else if (drawHitPart)
    {
	// draw only hit
	vtx->vertex.setValues (0, 8, &vertices[8]); 
	faces->coordIndex.setValues (0, 6*5, indices);
    }

    faces->vertexProperty = vtx;

    // set parts
    setPart ("shapeHints",	shapeHints);
    setPart ("faces",		faces);

    if (showLines.getValue () == true)
    {
	SoIndexedLineSet *lines = new SoIndexedLineSet;
	if (drawCrystal.getValue ())
	{
	    if (drawHitPart)
	    {
		lines->coordIndex.setValues (0, 11*5, indices);
	    }
	    else
	    {
		lines->coordIndex.setValues (0, 6*5, indices);
	    }
	}
	else if (drawHitPart)
	{
	    lines->coordIndex.setValues (0, 6*5, indices);
	}
	lines->vertexProperty = vtx;
	setPart ("lines", lines);
    }
    else
    {
 	setPart ("lines", NULL);
    }
}
