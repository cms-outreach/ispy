//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Inventor/interface/IgSo2DArrow.h"
#include <Inventor/nodes/SoVertexProperty.h>
#include <Inventor/nodes/SoLineSet.h>
#include <Inventor/nodes/SoFaceSet.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

SO_KIT_SOURCE (IgSo2DArrow);

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

void
IgSo2DArrow::initClass (void)
{ SO_KIT_INIT_CLASS (IgSo2DArrow, IgSoShapeKit, "IgSoShapeKit"); }

IgSo2DArrow::IgSo2DArrow (void)
{
    SO_KIT_CONSTRUCTOR (IgSo2DArrow);
    SO_KIT_ADD_FIELD (length, (1.0f));
    SO_KIT_ADD_FIELD (lengthRatio, (0.1f));
    SO_KIT_ADD_FIELD (headHeight, (0.05f));
    SO_KIT_ADD_CATALOG_ENTRY (line, SoLineSet, FALSE, separator,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY (head, SoFaceSet, FALSE, separator,\x0, TRUE);
    SO_KIT_INIT_INSTANCE ();
    setUpConnections (true, true);
}

void
IgSo2DArrow::refresh (void)
{
    SoLineSet		*line = new SoLineSet;
    SoFaceSet		*head = new SoFaceSet;
    SoVertexProperty	*vtx;

    float		lRatio = lengthRatio.getValue ();
    float		hh = headHeight.getValue ();
    float		l = length.getValue ();

    // line
    vtx = new SoVertexProperty;
    vtx->vertex.set1Value (0, 0.f, 0.f, 0.f);
    vtx->vertex.set1Value (1, (1.f-lRatio)*l, 0.f, 0.f);
    vtx->normal = SbVec3f (0.f,0.f,1.f);
    line->numVertices = 2;
    line->vertexProperty = vtx;

    // head
    vtx = new SoVertexProperty;
    vtx->vertex.set1Value (0, (1.f-lRatio)*l, -.5f*hh, 0.f);
    vtx->vertex.set1Value (1, l, 0.f, 0.f);
    vtx->vertex.set1Value (2, (1.f-lRatio)*l, .5f*hh, 0.f);
    vtx->normal = SbVec3f (0.f,0.f,1.f);
    head->numVertices = 3;
    head->vertexProperty = vtx;

    setPart ("line", line);
    setPart ("head", head);
}
