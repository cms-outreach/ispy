//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Inventor/interface/IgSoCircleArc.h"
#include "Inventor/nodes/SoLineSet.h"
#include "Inventor/nodes/SoVertexProperty.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

SO_KIT_SOURCE (IgSoCircleArc);

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

void
IgSoCircleArc::initClass (void)
{ SO_KIT_INIT_CLASS (IgSoCircleArc, IgSoShapeKit, "IgSoShapeKit"); }

IgSoCircleArc::IgSoCircleArc (void)
{
    SO_KIT_CONSTRUCTOR (IgSoCircleArc);
    SO_KIT_ADD_FIELD (center, (0.f,0.f));
    SO_KIT_ADD_FIELD (radius, (1.f));
    SO_KIT_ADD_FIELD (sweepAngle, (360.f));
    SO_KIT_ADD_CATALOG_ENTRY (line,  SoLineSet, FALSE, separator,\x0, TRUE);
    SO_KIT_INIT_INSTANCE ();
    setUpConnections (true, true);
}

void
IgSoCircleArc::refresh (void)
{
    static const int	CIRCLE_SEGMENTS = 60;
    SoVertexProperty	*vtx = new SoVertexProperty;
    SoLineSet		*line = new SoLineSet;
    int			segments = int(CIRCLE_SEGMENTS * sweepAngle.getValue () / 360 + .5);

    if (segments < 3)
	segments = 3;

    double		segAngle = sweepAngle.getValue () * M_PI / 180 / segments;
    double		r = radius.getValue ();
    float		cx, cy;

    center.getValue ().getValue (cx, cy);
    for (int i = 0; i < segments; i++)
	vtx->vertex.set1Value (i, SbVec3f (r * cos (i * segAngle) + cx,
					   r * sin (i * segAngle) + cy,
					   0));

    vtx->normal = SbVec3f (0, 0, 1);
    vtx->normalBinding = SoVertexProperty::OVERALL;
    vtx->materialBinding = SoVertexProperty::OVERALL;
    line->numVertices = segments;
    line->vertexProperty = vtx;

    setPart ("line", line);
}
