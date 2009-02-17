//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Inventor/interface/IgSoPolyVol.h"
#include <Inventor/nodes/SoVertexProperty.h>
#include <Inventor/nodes/SoShapeHints.h>
#include <Inventor/nodes/SoIndexedFaceSet.h>
#include <Inventor/nodes/SoTranslation.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

SO_KIT_SOURCE (IgSoPolyVol);

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

void
IgSoPolyVol::initClass (void)
{ SO_KIT_INIT_CLASS (IgSoPolyVol, IgSoShapeKit, "IgSoShapeKit"); }

IgSoPolyVol::IgSoPolyVol (void)
{
    SO_KIT_CONSTRUCTOR (IgSoPolyVol);
    SO_KIT_ADD_FIELD (vtxPts, (SbVec3f (0,0,0)));
    SO_KIT_ADD_FIELD (center, (SbVec3f (0,0,0)));
    SO_KIT_ADD_FIELD (dZ, (1.0));
    SO_KIT_ADD_CATALOG_ENTRY (hints, SoShapeHints, FALSE, separator,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY (trans, SoTranslation, FALSE, separator,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY (face, SoIndexedFaceSet, FALSE, separator,\x0, TRUE);
    SO_KIT_INIT_INSTANCE ();
    setUpConnections (true, true);

    const SbVec3f defPolyVol[10] = {
	SbVec3f (0.0,0.0,-0.5), SbVec3f (1.0,0.0,-0.5), SbVec3f (1.5,1.0,-0.5),
        SbVec3f (0.2,1.2,-0.5), SbVec3f (-0.2,1.0,-0.5),
	SbVec3f (0.0,0.0,0.5), SbVec3f (1.0,0.0,0.5), SbVec3f (1.5,1.0,0.5),
	SbVec3f (0.2,1.2,0.5), SbVec3f (-0.2,1.0,0.5)	
    };
    vtxPts.setValues (0,10,defPolyVol);
}

void
IgSoPolyVol::refresh (void)
{
    if (vtxPts.getNum () < 2) 
    {
	// set parts
	setPart ("hints", NULL);
	setPart ("trans", NULL);
	setPart ("face",  NULL);

	return;
    }
    
    SoTranslation	*trans = new SoTranslation;
    SoShapeHints	*hints = new SoShapeHints;
    SoVertexProperty	*vtx = new SoVertexProperty;
    SoIndexedFaceSet	*face = new SoIndexedFaceSet;

    hints->vertexOrdering = SoShapeHints::COUNTERCLOCKWISE;
    hints->faceType = SoShapeHints::UNKNOWN_FACE_TYPE;
    hints->shapeType = SoShapeHints::SOLID;
    hints->creaseAngle = 0;

    trans->translation = center;

    //  Create the face set
    vtx->vertex = vtxPts;
    face->vertexProperty = vtx;

    int nsides = vtxPts.getNum ()/2; // number of sides of front polygon
    face->coordIndex.setNum (7 * nsides + 2); //5*nsides for side polys+ 2* (nsides+1) for front and back

    int *indx = face->coordIndex.startEditing ();
    int i; 

    //  sides
    for (i = 0; i < nsides-1; i++)
    {
	*indx++ = i;
	*indx++ = i+1;
	*indx++ = i+nsides+1;
	*indx++ = i+nsides;
	*indx++ = SO_END_FACE_INDEX;
    } 
    *indx++ = i;  *indx++ = 0;  *indx++ = nsides; *indx++ = i+nsides;
    *indx++ = SO_END_FACE_INDEX;

    //  front & back faces; back face counts down
    for (i = nsides-1; i > -1; i--)
    {
	*indx = i;
	*((indx++) + nsides + 1) = 2*nsides-1-i; // bump past n vertices+end_face
    }
    *indx++ = SO_END_FACE_INDEX;
    *(indx+nsides) = SO_END_FACE_INDEX;
    face->coordIndex.finishEditing ();

    // set parts
    setPart ("hints", hints);
    setPart ("trans", trans);
    setPart ("face",  face);
}

//! method to set actual values: note angles are in radians
void
IgSoPolyVol::initialise (const int sides, const float dZHalf, 
			 const float *angles, const float *sideHalf)
{
    assert (sides > 2);

    //  Stuff the points into the vtxPts
    vtxPts.setNum (2*sides);

    SbVec3f	*pt = vtxPts.startEditing ();
    float	x = 0;
    float	y = 0;
    float	z = 0;

    for (int i = 0; i < sides; i++, pt++)
    {
	*pt = SbVec3f (x,y,z-dZHalf);
	*(pt+sides) = SbVec3f (x,y,z+dZHalf);
	x += 2 * (*sideHalf) * cos (*angles);
	y += 2 * (*sideHalf++) * sin (*angles++);
    }

    vtxPts.finishEditing ();
}
