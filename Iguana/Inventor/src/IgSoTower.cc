//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Inventor/interface/IgSoTower.h"
#include "Inventor/nodes/SoCoordinate3.h"
#include "Inventor/nodes/SoIndexedFaceSet.h"
#include "Inventor/nodes/SoMaterialBinding.h"
#include "Inventor/nodes/SoShapeHints.h"
#include "Inventor/nodes/SoVertexProperty.h"
#include "Inventor/SbColor.h"
#include <vector>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

SO_KIT_SOURCE (IgSoTower);

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

void
IgSoTower::initClass (void)
{ SO_KIT_INIT_CLASS (IgSoTower, IgSoShapeKit, "IgSoShapeKit"); }

IgSoTower::IgSoTower (void)
{
    SO_KIT_CONSTRUCTOR (IgSoTower);

    SO_KIT_ADD_FIELD (position, 	(0.0f,0.0f));
    SO_KIT_ADD_FIELD (energy,		(0.0f));
    SO_KIT_ADD_FIELD (emFraction,	(0.0f));
    SO_KIT_ADD_FIELD (scaleFactor,	(1.0f));
    SO_KIT_ADD_FIELD (minimumEnergy,	(0.0f));
    SO_KIT_ADD_FIELD (etaWidth,		(0.087f));
    SO_KIT_ADD_FIELD (phiWidth,		(0.087f));
    SO_KIT_ADD_FIELD (orderedRGBA,	(0));

    orderedRGBA.set1Value (0, SbColor (0.622, 0.0, 0.622).getPackedValue ());
    orderedRGBA.set1Value (1, orderedRGBA [0]);
    orderedRGBA.set1Value (2, orderedRGBA [0]);
    orderedRGBA.set1Value (3, orderedRGBA [0]);
    orderedRGBA.set1Value (4, orderedRGBA [0]);
    orderedRGBA.set1Value (5, orderedRGBA [0]);
    orderedRGBA.set1Value (6, SbColor (0.0, 0.0, 1.0).getPackedValue ());
    orderedRGBA.set1Value (7, orderedRGBA [6]);
    orderedRGBA.set1Value (8, orderedRGBA [6]);
    orderedRGBA.set1Value (9, orderedRGBA [6]);
    orderedRGBA.set1Value (10, orderedRGBA [6]);
    orderedRGBA.set1Value (11, orderedRGBA [6]);
   
    SO_KIT_ADD_CATALOG_ENTRY (shapeHints, SoShapeHints, FALSE, separator,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY (faces, SoIndexedFaceSet, FALSE, separator,\x0, TRUE);
    SO_KIT_INIT_INSTANCE ();
    setUpConnections (true, true);
}

void
IgSoTower::cleanParts (void)
{
    setPart ("shapeHints", NULL);
    setPart ("faces", NULL);
}

void
IgSoTower::refresh (void)
{
    if ((energy.getValue () == 0.0) || (fabs (energy.getValue ()) < fabs (minimumEnergy.getValue ())))
    {
	cleanParts ();
	return;
    }

    SoShapeHints *shapeHints = new SoShapeHints;
    SoIndexedFaceSet *faces = new SoIndexedFaceSet;
    SoVertexProperty *vtx = new SoVertexProperty;
 
    int faceIndices [] = 
	{0, 1, 5, 4, SO_END_FACE_INDEX,
	 3, 7, 6, 2, SO_END_FACE_INDEX,
	 1, 2, 6, 5, SO_END_FACE_INDEX,
	 5, 6, 7, 4, SO_END_FACE_INDEX,
	 4, 7, 3, 0, SO_END_FACE_INDEX,
	 0, 3, 2, 1, SO_END_FACE_INDEX,

	 8,  9, 13, 12, SO_END_FACE_INDEX,
	11, 15, 14, 10, SO_END_FACE_INDEX,
	 9, 10, 14, 13, SO_END_FACE_INDEX,
	13, 14, 15, 12, SO_END_FACE_INDEX,
	12, 15, 11,  8, SO_END_FACE_INDEX,
	 8, 11, 10,  9, SO_END_FACE_INDEX
	};
    
    float	cx, cz;
    float	dx = etaWidth.getValue () / 2.0F;
    float	dz = phiWidth.getValue () / 2.0F;
    float 	cMax = energy.getValue () / scaleFactor.getValue ();
    float	cMin = cMax * emFraction.getValue ();
    bool 	noEm;
    bool 	noHm;
    bool 	top = false;
    bool	bottom = false;
    
    (emFraction.getValue () == 0.0) ? noEm = true : noEm = false;
    (emFraction.getValue () == 1.0) ? noHm = true : noHm = false;
    
    position.getValue ().getValue (cx, cz);

    if (cMax < 0.0)
    {
	shapeHints->vertexOrdering = SoShapeHints::CLOCKWISE;

	orderedRGBA.set1Value (0, orderedRGBA [0] xor 0xffffff00);
	orderedRGBA.set1Value (1, orderedRGBA [0]);
	orderedRGBA.set1Value (2, orderedRGBA [0]);
	orderedRGBA.set1Value (3, orderedRGBA [0]);
	orderedRGBA.set1Value (4, orderedRGBA [0]);
	orderedRGBA.set1Value (5, orderedRGBA [0]);
	orderedRGBA.set1Value (6, orderedRGBA [6] xor 0xffffff00);
	orderedRGBA.set1Value (7, orderedRGBA [6]);
	orderedRGBA.set1Value (8, orderedRGBA [6]);
	orderedRGBA.set1Value (9, orderedRGBA [6]);
	orderedRGBA.set1Value (10, orderedRGBA [6]);
	orderedRGBA.set1Value (11, orderedRGBA [6]);
    }
    else
    {
	shapeHints->vertexOrdering = SoShapeHints::COUNTERCLOCKWISE;
    }
    
    shapeHints->shapeType = SoShapeHints::SOLID;
    shapeHints->faceType = SoShapeHints::CONVEX;
    
    int nverts = 16;
    std::vector<SbVec3f> vertexData (nverts);

    if (! noEm) 
    {		
	vertexData [0][0] = cx - dx;
	vertexData [0][1] = cMin;
	vertexData [0][2] = cz - dz;
    
	vertexData [1][0] = cx + dx;
	vertexData [1][1] = cMin;
	vertexData [1][2] = cz - dz;

	vertexData [2][0] = cx + dx;
	vertexData [2][1] = cMin;
	vertexData [2][2] = cz + dz;
    
	vertexData [3][0] = cx - dx;
	vertexData [3][1] = cMin;
	vertexData [3][2] = cz + dz;

	vertexData [4][0] = cx - dx;
	vertexData [4][1] = 0;
	vertexData [4][2] = cz - dz;

	vertexData [5][0] = cx + dx;
	vertexData [5][1] = 0;
	vertexData [5][2] = cz - dz;

	vertexData [6][0] = cx + dx;
	vertexData [6][1] = 0;
	vertexData [6][2] = cz + dz;

	vertexData [7][0] = cx - dx;
	vertexData [7][1] = 0;
	vertexData [7][2] = cz + dz;
  
	bottom = true;
    } 
    
    if (! noHm) 
    {
	vertexData [8][0] = cx - dx;
	vertexData [8][1] = cMax;
	vertexData [8][2] = cz - dz;
    
	vertexData [9][0] = cx + dx;
	vertexData [9][1] = cMax;
	vertexData [9][2] = cz - dz;

	vertexData [10][0] = cx + dx;
	vertexData [10][1] = cMax;
	vertexData [10][2] = cz + dz;
    
	vertexData [11][0] = cx - dx;
	vertexData [11][1] = cMax;
	vertexData [11][2] = cz + dz;

	vertexData [12][0] = cx - dx;
	vertexData [12][1] = cMin;
	vertexData [12][2] = cz - dz;

	vertexData [13][0] = cx + dx;
	vertexData [13][1] = cMin;
	vertexData [13][2] = cz - dz;

	vertexData [14][0] = cx + dx;
	vertexData [14][1] = cMin;
	vertexData [14][2] = cz + dz;

	vertexData [15][0] = cx - dx;
	vertexData [15][1] = cMin;
	vertexData [15][2] = cz + dz;
	
	top = true;
    }
    
    vtx->materialBinding = SoMaterialBinding::PER_FACE;

    if (top && bottom)
    {
	vtx->vertex.setValues (0, 16, &vertexData [0]);
	vtx->orderedRGBA = orderedRGBA;

	faces->vertexProperty = vtx;
	faces->coordIndex.setValues (0, sizeof (faceIndices) / sizeof (int), faceIndices);
	setPart ("shapeHints",	shapeHints);
	setPart ("faces",      	faces);
    }
    else if (top && (! bottom))
    {
	orderedRGBA.set1Value (0, orderedRGBA [6]);
	orderedRGBA.set1Value (1, orderedRGBA [6]);
	orderedRGBA.set1Value (2, orderedRGBA [6]);
	orderedRGBA.set1Value (3, orderedRGBA [6]);
	orderedRGBA.set1Value (4, orderedRGBA [6]);
	orderedRGBA.set1Value (5, orderedRGBA [6]);
	vtx->orderedRGBA = orderedRGBA;
	
	vtx->vertex.setValues (0, 8, &vertexData [8]);
	faces->vertexProperty = vtx;
	faces->coordIndex.setValues (0, 6 * 5, faceIndices);
	setPart ("shapeHints",	shapeHints);
	setPart ("faces",      	faces);
    }
    else if ((! top) && bottom)
    {
	orderedRGBA.set1Value (6, orderedRGBA [0]);
	orderedRGBA.set1Value (7, orderedRGBA [0]);
	orderedRGBA.set1Value (8, orderedRGBA [0]);
	orderedRGBA.set1Value (9, orderedRGBA [0]);
	orderedRGBA.set1Value (10, orderedRGBA [0]);
	orderedRGBA.set1Value (11, orderedRGBA [0]);
	vtx->orderedRGBA = orderedRGBA;

	vtx->vertex.setValues (0, 8, &vertexData [0]);
	faces->vertexProperty = vtx;
	faces->coordIndex.setValues (0, 6 * 5, faceIndices);
	setPart ("shapeHints",	shapeHints);
	setPart ("faces",      	faces);
    }
    else 
    {
	vtx->orderedRGBA = orderedRGBA;
	faces->vertexProperty = vtx;
	shapeHints->ref(); shapeHints->unref();
	faces->ref(); faces->unref();
	setPart ("shapeHints",	NULL);
	setPart ("faces",	NULL);
    }
}
