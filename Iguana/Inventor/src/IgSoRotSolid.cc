//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Inventor/interface/IgSoRotSolid.h"
#include <Inventor/nodes/SoIndexedFaceSet.h>
#include <Inventor/nodes/SoIndexedLineSet.h>
#include <Inventor/nodes/SoVertexProperty.h>
#include <Inventor/nodes/SoShapeHints.h>
#include <Inventor/SbLinear.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

SO_KIT_SOURCE (IgSoRotSolid);

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

void
IgSoRotSolid::initClass (void)
{ SO_KIT_INIT_CLASS (IgSoRotSolid, IgSoShapeKit, "IgSoShapeKit"); }

IgSoRotSolid::IgSoRotSolid (void)
{
    SO_KIT_CONSTRUCTOR (IgSoRotSolid);
    SO_KIT_ADD_FIELD (phiStart,		(0.0));
    SO_KIT_ADD_FIELD (phiDelta,		(2 * M_PI));
    SO_KIT_ADD_FIELD (polygon,		(0, 0));
    SO_KIT_ADD_FIELD (divisions,	(4));
    SO_KIT_ADD_FIELD (smooth,		(TRUE));
    SO_KIT_ADD_FIELD (showLines,	(FALSE));
    SO_KIT_ADD_CATALOG_ENTRY (hints, SoShapeHints, FALSE, separator,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY (faces, SoIndexedFaceSet, FALSE, separator,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY (lines, SoIndexedLineSet, FALSE, separator,\x0, TRUE);
    
    SO_KIT_INIT_INSTANCE ();
    setUpConnections (true, true);

    SbVec2f defaultShape [] =
	{
	    SbVec2f (0, 0), 
	    SbVec2f (1, 1), 
	    SbVec2f (1, 0)
	}; 

    polygon.setValues (0, 3, defaultShape);
}

void 
IgSoRotSolid::cleanParts (void)
{
    setPart ("hints", NULL);
    setPart ("faces", NULL);
    setPart ("lines", NULL);
}

void
IgSoRotSolid::refresh (void)
{
    // If less than 3 points in the polygon being rotated, we have an
    // unclosed surface, therefore we just quit.
    if (polygon.getNum () < 3)
    {
	cleanParts ();
	return;
    }

    SoShapeHints	*hints = new SoShapeHints;
    SoIndexedFaceSet	*faces = new SoIndexedFaceSet;
    SoVertexProperty	*vtx = new SoVertexProperty;

    int			divisions = this->divisions.getValue ();
    float		phiStart = this->phiStart.getValue ();
    float		phiDelta = this->phiDelta.getValue ();
    bool		smooth = this->smooth.getValue ();
    int 		numEdges = polygon.getNum ();

    bool 		edgeVertical = false;
    bool 		closedShape = true;

    // If the angle is less than 2PI, then the shape is not closed.
    if (fabs (phiDelta) < 2 * M_PI) closedShape = false;

    if (smooth)	divisions = (int) (phiDelta / (M_PI / 36.0)) + 2;
    float delta = phiDelta / divisions;

    // Get the cross-section
    std::vector<SbVec3f> crossSection;

    for (int i = 0; i < numEdges; i++)
    {
        crossSection.push_back (SbVec3f (polygon [i][0], 0, polygon [i][1]));
    }
    crossSection.push_back (SbVec3f (polygon [0][0], 0, polygon [0][1]));
    
    int totalPoints = 0;
    
    std::vector<SbVec3f> vertexData;
    std::vector<int> indices;
    std::vector<int> lineIndices;
    std::vector<SbVec3f>::iterator it = crossSection.begin ();
    for (int i = 0; i < numEdges; i++, it++)
    {
	int nLeftPoints = 0;
	int nRightPoints = 0;
	
	SbVec3f leftPoint = *it;
	SbVec3f rightPoint = *(it + 1);
	
	(leftPoint [2] == rightPoint [2]) ? edgeVertical = true : edgeVertical = false;

	// Take one edge and rotate it.
	//
	if (leftPoint [0] != 0) // if left point is not on Z
	{
	    // rotate it
	    for (float phi = phiStart; phi < phiStart + phiDelta; phi += delta) // collect the rest
	    {
	        SbMatrix transform;
                transform.setRotate (SbRotation (SbVec3f (0, 0, 1), phi));
                SbVec3f dst;
                transform.multVecMatrix (leftPoint, dst);
		vertexData.push_back (dst);
		lineIndices.push_back (totalPoints);
		totalPoints++;
		nLeftPoints++;
	    }

	    // Repeate the last point to avoid rounding
	    SbMatrix transform;
	    transform.setRotate (SbRotation (SbVec3f (0, 0, 1), phiStart + phiDelta));
	    SbVec3f dst;
	    transform.multVecMatrix (leftPoint, dst);
	    vertexData.push_back (dst);
	    lineIndices.push_back (totalPoints);
	    totalPoints++;
	    nLeftPoints++;
	    lineIndices.push_back (SO_END_LINE_INDEX);
	} else 
	{
	    if (edgeVertical) 
	    { 
		if (!closedShape) 
		{
		    vertexData.push_back (leftPoint);		
		    totalPoints++;
		    nLeftPoints++;
		}
	    } else 
	    {
		vertexData.push_back (leftPoint);
		totalPoints++;
		nLeftPoints++;		
	    }
	}

	if (rightPoint [0] != 0) // if right point is not on Z
	{
	    // rotate it
	    for (float phi = phiStart; phi < phiStart + phiDelta; phi += delta) // collect the rest
	    {
	        SbMatrix transform;
                transform.setRotate (SbRotation (SbVec3f (0, 0, 1), phi));
                SbVec3f dst;
                transform.multVecMatrix (rightPoint, dst);
		vertexData.push_back (dst);
		lineIndices.push_back (totalPoints);
		totalPoints++;
		nRightPoints++;
	    }

	    // Repeate the last point to avoid rounding
	    SbMatrix transform;
	    transform.setRotate (SbRotation (SbVec3f (0, 0, 1), phiStart + phiDelta));
	    SbVec3f dst;
	    transform.multVecMatrix (rightPoint, dst);
	    vertexData.push_back (dst);
	    lineIndices.push_back (totalPoints);    
	    totalPoints++;
	    nRightPoints++;
	    lineIndices.push_back (SO_END_LINE_INDEX);
	} else
	{
	    if (edgeVertical)
	    {
		if (!closedShape)
		{
		    vertexData.push_back (rightPoint);
		    totalPoints++;
		    nRightPoints++;
		}
	    } else 
	    {	    
		vertexData.push_back (rightPoint);
		totalPoints++;
		nRightPoints++;
	    }
	}

	int edgePoints = nRightPoints + nLeftPoints;
	int startPoint = totalPoints - edgePoints;
	int nEdgePoints = 0;
	(nLeftPoints >= nRightPoints) ? nEdgePoints = nLeftPoints : nEdgePoints = nRightPoints;

	for (int npe = 0; npe < nEdgePoints - 1; npe++)
	{
	    switch (nLeftPoints)
	    {
	    case 0:
		break;
	    case 1:
		indices.push_back (startPoint);
		break;
	    default:
		if (nRightPoints != 0) 
		{
		    indices.push_back (startPoint + npe);
		    indices.push_back (startPoint + npe + 1);
		} else
		{
		    indices.push_back (startPoint + npe);
		}
		break;
	    }
	    
	    switch (nRightPoints)
	    {
	    case 0:
		break;
	    case 1:
		indices.push_back (totalPoints);
		indices.push_back (SO_END_FACE_INDEX);
		break;
	    default:
		if (nLeftPoints != 0) 
		{
		    indices.push_back (totalPoints - nRightPoints + npe + 1);
		    indices.push_back (totalPoints - nRightPoints + npe);
		    indices.push_back (SO_END_FACE_INDEX);
		    
		    if (!smooth) 
		    {
			if (nLeftPoints == 1) 
			    lineIndices.push_back (startPoint);
			else
			    lineIndices.push_back (startPoint + npe);
			lineIndices.push_back (totalPoints - nRightPoints + npe);
			lineIndices.push_back (SO_END_LINE_INDEX);
		    }
		} else
		{
		    indices.push_back (totalPoints - npe - 1);
		}
		break;
	    }
	}
	if (!indices.empty () && (indices.back () != SO_END_FACE_INDEX))
	    indices.push_back (SO_END_FACE_INDEX);	    
    }

    if (!closedShape)
    {
	std::vector<SbVec3f> edgeVertexData;
	std::vector<int> edgeIndices;
	
	for (std::vector<SbVec3f>::iterator it = crossSection.begin (); it != crossSection.end (); it++)
	{
	    SbMatrix transform;
	    transform.setRotate (SbRotation (SbVec3f (0, 0, 1), phiStart));
	    SbVec3f src = *it;
	    SbVec3f dst;
	    transform.multVecMatrix (src, dst);
	    vertexData.push_back (dst);
	    indices.push_back (totalPoints);
	    totalPoints++;
	}
	indices.push_back (SO_END_FACE_INDEX); // end the shape

	float phi = phiDelta + phiStart;
 	for (std::vector<SbVec3f>::reverse_iterator it = crossSection.rbegin (); it != crossSection.rend (); it++)
 	{
            SbMatrix transform;
            transform.setRotate (SbRotation (SbVec3f (0, 0, 1), phi));
            SbVec3f src = *it;
            SbVec3f dst;
            transform.multVecMatrix (src, dst);
	    vertexData.push_back (dst);
	    indices.push_back (totalPoints);
	    totalPoints++;
 	}
 	indices.push_back (SO_END_FACE_INDEX); // end the shape
    }

    vtx->vertex.setValues (0, vertexData.size (), &vertexData [0]);
    faces->vertexProperty = vtx;
    faces->coordIndex.setValues (0, indices.size (), &indices [0]);

    // Set the hints for the endcaps
    hints->vertexOrdering 	= SoShapeHints::COUNTERCLOCKWISE;   
    hints->shapeType 		= SoShapeHints::SOLID;
    if (smooth)	
	hints->creaseAngle 	= 10.0;

    setPart ("hints", hints);
    setPart ("faces", faces);

    if (showLines.getValue () == true) 
    {
	SoIndexedLineSet	*lines = new SoIndexedLineSet;
	lines->coordIndex.setValues (0, lineIndices.size (), &lineIndices [0]);
	lines->vertexProperty = vtx;
	setPart ("lines", lines);
    }
    else
    {
	setPart ("lines", NULL);
    }
}

void
IgSoRotSolid::makePgon (const std::vector<float> &zvals,
			const std::vector<float> &rmin,
			const std::vector<float> &rmax,
			float phiStart,
			float phiDelta,
			int divisions)
{
    this->divisions.setValue (divisions);
    this->phiStart.setValue (phiStart);
    this->phiDelta.setValue (phiDelta);
    if (divisions < 0)
	this->smooth = TRUE;
    else
	this->smooth = FALSE;

    std::vector<SbVec2f> contour (2 * zvals.size ());
    int k = 0;

    for (int i = 0; i < (int) zvals.size (); i++)
    {
	contour [k][0] = rmax [i];
	contour [k][1] = zvals [i];
	k++;
    }

    for (int i = (int) zvals.size () - 1; i >= 0 ; i--)
    {
	contour [k][0] = rmin [i];
	contour [k][1] = zvals [i];
	k++;
    }
    this->polygon.setValues (0, 2 * zvals.size (), &contour [0]);
}

void
IgSoRotSolid::makePcon (const std::vector<float> &zvals,
			const std::vector<float> &rmin,
			const std::vector<float> &rmax,
			float phiStart,
			float phiDelta,
			int divisions)
{
    makePgon (zvals, rmin, rmax, phiStart, phiDelta, divisions);
}

void
IgSoRotSolid::makeCons (float rmin1,
			float rmax1,
			float rmin2,
			float rmax2,
			float dz,
			float phiStart,
			float phiDelta,
			int divisions)
{
    std::vector<float> zvals (2);
    std::vector<float> rminTemp (2);
    std::vector<float> rmaxTemp (2);
    
    zvals [0] = -dz;
    zvals [1] = dz;
    rminTemp [0] = rmin1;
    rminTemp [1] = rmin2;
    rmaxTemp [0] = rmax1;
    rmaxTemp [1] = rmax2;

    makePcon (zvals, rminTemp, rmaxTemp, phiStart, phiDelta, divisions);
}

void
IgSoRotSolid::makeTubs (float rmin,
			float rmax,
			float dz,
			float phiStart,
			float phiDelta,
			int divisions)
{
    std::vector<float> zvals (2);
    std::vector<float> rminTemp (2);
    std::vector<float> rmaxTemp (2);
    
    zvals [0] = -dz;
    zvals [1] = dz;
    rminTemp [0] = rmin;
    rminTemp [1] = rmin;
    rmaxTemp [0] = rmax;
    rmaxTemp [1] = rmax;
    
    makePcon (zvals, rminTemp, rmaxTemp, phiStart, phiDelta, divisions);    
}
