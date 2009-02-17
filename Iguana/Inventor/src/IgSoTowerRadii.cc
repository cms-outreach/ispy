//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Inventor/interface/IgSoTowerRadii.h"
#include "Inventor/nodes/SoMarkerSet.h"
#include "Inventor/nodes/SoLineSet.h"
#include "Inventor/nodes/SoVertexProperty.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

SO_KIT_SOURCE (IgSoTowerRadii);

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

void
IgSoTowerRadii::initClass (void)
{ SO_KIT_INIT_CLASS (IgSoTowerRadii, IgSoShapeKit, "IgSoShapeKit"); }

IgSoTowerRadii::IgSoTowerRadii (void)
{
    SO_KIT_CONSTRUCTOR (IgSoTowerRadii);
    SO_KIT_ADD_FIELD (center, (0.0f,0.0f));
    SO_KIT_ADD_FIELD (radius, (1.0f));
    SO_KIT_ADD_FIELD (displayCenter, (false));
    SO_KIT_ADD_CATALOG_ENTRY (line,  SoLineSet, FALSE, separator,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY (marker,  SoMarkerSet, FALSE, separator,\x0, TRUE);
    SO_KIT_INIT_INSTANCE ();
    setUpConnections (true, true);
}

void
IgSoTowerRadii::refresh (void)
{
    SoVertexProperty	*vtx = new SoVertexProperty;
    SoLineSet		*line = new SoLineSet;
    int			segments = 60;

    double		segAngle = 2 * M_PI / segments;
    double		r = radius.getValue ();
    float		cx, cz;
    int i = 0;

    center.getValue ().getValue (cx, cz);
    for (i = 0; i < segments; i++)
	vtx->vertex.set1Value (i, SbVec3f (r * cos (i * segAngle) + cx,
					   0,
					   r * sin (i * segAngle) + cz));

    vtx->vertex.set1Value (i, SbVec3f (r * cos (0) + cx,
					   0,
					   r * sin (0) + cz));
    vtx->normal = SbVec3f (0, 0, 1);
    vtx->normalBinding = SoVertexProperty::OVERALL;
    vtx->materialBinding = SoVertexProperty::OVERALL;
    line->numVertices = segments + 1;
    line->vertexProperty = vtx;

    if (displayCenter.getValue ())
    {
	SoMFInt32 markerIndex;
	markerIndex.setValue (SoMarkerSet::CROSS_5_5);
    
	SoMarkerSet *marker = new SoMarkerSet;
	SoVertexProperty *mvtx = new SoVertexProperty;
	mvtx->vertex.set1Value (0, SbVec3f (cx, 0, cz));
	marker->vertexProperty = mvtx;
	marker->markerIndex = markerIndex;
	marker->numPoints = 1;
	marker->startIndex = 0;

	setPart ("marker", marker);
    } 
    else
    {	
	setPart ("marker", NULL);
    }
    
    setPart ("line", line);
}
