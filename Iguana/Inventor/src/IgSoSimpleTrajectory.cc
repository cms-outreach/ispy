//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Inventor/interface/IgSoSimpleTrajectory.h"
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoLineSet.h>
//#include <Inventor/nodes/SoNurbsCurve.h>
#include <Inventor/nodes/SoPointSet.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

SO_KIT_SOURCE (IgSoSimpleTrajectory);

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

void
IgSoSimpleTrajectory::initClass (void)
{ SO_KIT_INIT_CLASS (IgSoSimpleTrajectory, IgSoShapeKit, "IgSoShapeKit"); }

IgSoSimpleTrajectory::IgSoSimpleTrajectory (void)
{
    SO_KIT_CONSTRUCTOR (IgSoSimpleTrajectory);
    SO_KIT_ADD_FIELD (controlPoints, (0.0, 0.0, 0.0));
    SO_KIT_ADD_FIELD (markerPoints,  (0.0, 0.0, 0.0));
    SO_KIT_ADD_FIELD (lineWidth,     (1.0));
    SO_KIT_ADD_FIELD (pointSize,     (3.0));
    SO_KIT_ADD_FIELD (linePattern,   (0xffff));
    SO_KIT_ADD_CATALOG_ENTRY (style, SoDrawStyle, FALSE, separator,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY (line, SoLineSet, FALSE, separator,\x0, TRUE); // FIXME: SoNurbsCurve
    SO_KIT_ADD_CATALOG_ENTRY (markers, SoPointSet, FALSE, separator,\x0, TRUE);
    SO_KIT_INIT_INSTANCE ();
    setUpConnections (true, true);
}

void
IgSoSimpleTrajectory::refresh (void)
{
    SoDrawStyle		*style = new SoDrawStyle;
    SoLineSet		*line = new SoLineSet; // FIXME: SoNurbsCurve
    SoPointSet		*markers = new SoPointSet;
    SoVertexProperty	*lineVtx = new SoVertexProperty;
    SoVertexProperty	*markerVtx = new SoVertexProperty;

    style->lineWidth = lineWidth;
    style->pointSize = pointSize;
    style->linePattern = linePattern;

#if 0	// Old NURBS version
    if (controlPoints.getNum () >= 2)
    {
	static const int repeat = 2; // to force curve really through the points, repeat them
	int points = controlPoints.getNum ();
	for (int i = 0, n = 0; i < points; ++i)
	    for (int j = 0; j < repeat; ++j, ++n)
		lineVtx->vertex.set1Value (n, controlPoints [i]);

	// Define the knot vector
	for (int i = 0; i < points * repeat + 3; ++i)
	    line->knotVector.set1Value (i, i);

	line->numControlPoints = points * repeat;
    }
#endif

    lineVtx->vertex = controlPoints;
    line->vertexProperty = lineVtx;

    markerVtx->vertex = markerPoints;
    markers->vertexProperty = markerVtx;

    setPart ("style",   style);
    setPart ("line",    line);
    setPart ("markers", markers);
}
