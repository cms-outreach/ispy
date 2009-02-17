//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Inventor/interface/IgSbColorMap.h"
#include "Inventor/nodes/SoVertexProperty.h"
#include "Inventor/nodes/SoQuadMesh.h"
#include "Inventor/sensors/SoTimerSensor.h"
#include "test_main.h"
#include <iostream>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

static const int CELLS = 20;
static void switchMap (void *vtxData, SoSensor *)
{
    static IgSbColorMap *cmaps [] = {
	&IgSbColorMap::autumn,
	&IgSbColorMap::bone,
	&IgSbColorMap::cool,
	&IgSbColorMap::copper,
	// &IgSbColorMap::cube,
	&IgSbColorMap::flag,
	&IgSbColorMap::gray,
	&IgSbColorMap::hot,
	&IgSbColorMap::hsv,
	&IgSbColorMap::jet,
	&IgSbColorMap::pink,
	&IgSbColorMap::prism,
	&IgSbColorMap::spring,
	&IgSbColorMap::summer,
	&IgSbColorMap::vga,
	&IgSbColorMap::winter,
	0
    };
    static IgSbColorMap **cmap = 0;

    if (cmap)
	++cmap;

    if (! cmap || ! *cmap)
	cmap = &cmaps [0];

    SoVertexProperty *vtx = (SoVertexProperty *) vtxData;
    vtx->materialBinding = SoVertexProperty::PER_VERTEX;
    vtx->orderedRGBA.setNum ((CELLS + 1) * (CELLS + 1));
    unsigned *rgba = vtx->orderedRGBA.startEditing ();
    for (int i = 0; i <= CELLS; ++i)
	for (int j = 0; j <= CELLS; ++j)
	{
	    float color [4] = { 0, 0, 0, 1 };
	    (*cmap)->color (i * 1. / CELLS, color);

	    unsigned r = int (color [0] * 255 + .5);
	    unsigned g = int (color [1] * 255 + .5);
	    unsigned b = int (color [2] * 255 + .5);
	    unsigned a = int (color [3] * 255 + .5);

	    rgba [i * (CELLS + 1) + j] = r << 24 | g << 16 | b << 8 | a;
	}

    vtx->orderedRGBA.finishEditing ();
}

QString makeTest (SoSeparator *root)
{
    SoVertexProperty *vtx = new SoVertexProperty;
    vtx->normal = SbVec3f (0, 0, 1);
    vtx->normalBinding = SoVertexProperty::OVERALL;
    switchMap (vtx, 0);
    vtx->vertex.setNum ((CELLS+1) * (CELLS+1));
    SbVec3f *pts = vtx->vertex.startEditing ();
    for (int i = 0; i <= CELLS; ++i)
	for (int j = 0; j <= CELLS; ++j)
	    pts [i * (CELLS + 1) + j] = SbVec3f (i, j, 0);
    vtx->vertex.finishEditing ();
    switchMap (vtx, 0);

    SoQuadMesh *cells = new SoQuadMesh;
    cells->verticesPerRow = CELLS + 1;
    cells->verticesPerColumn = CELLS + 1;
    cells->vertexProperty = vtx;

    SoTimerSensor *timeOutSensor = new SoTimerSensor (&switchMap, vtx);
    timeOutSensor->setInterval (SbTime (1.0));
    timeOutSensor->schedule ();

    root->removeAllChildren ();
    root->addChild (cells);

    return "IGUANA: IgSbColorMap Test";
}
