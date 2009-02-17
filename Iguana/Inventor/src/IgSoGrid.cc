//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Inventor/interface/IgSoGrid.h"
#include <Inventor/nodes/SoText2.h>
#include <Inventor/nodes/SoFont.h>
#include <Inventor/nodes/SoIndexedLineSet.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTranslation.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

SO_KIT_SOURCE (IgSoGrid);

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

void
IgSoGrid::initClass (void)
{ SO_KIT_INIT_CLASS (IgSoGrid, IgSoShapeKit, "IgSoShapeKit"); }


IgSoGrid::IgSoGrid (void)
{    
    SO_KIT_CONSTRUCTOR (IgSoGrid);
    SO_KIT_ADD_FIELD (maxX, 		(5.0f));
    SO_KIT_ADD_FIELD (maxZ, 		(5.0f));

    SO_KIT_ADD_CATALOG_ENTRY (xLabels,		SoSeparator,	FALSE, separator,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY (zLabels,		SoSeparator,	FALSE, separator,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY (grid,		SoIndexedLineSet, FALSE, separator,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY (font,		SoFont,		FALSE, separator,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY (labels,           SoSeparator,	FALSE, separator,\x0, TRUE);

    SO_KIT_INIT_INSTANCE ();
    setUpConnections (true, true);
}

void
IgSoGrid::refresh (void)
{
    SoSeparator		*xLabels	= new SoSeparator;
    SoSeparator		*zLabels	= new SoSeparator;
    SoIndexedLineSet	*grid		= new SoIndexedLineSet;
    SoFont		*font		= new SoFont;

    SoVertexProperty	*vtx		= new SoVertexProperty;
    SoText2		*text;
    SoMFInt32		coords;

    float		xMax = maxZ.getValue ();
    float		zMax = maxX.getValue ();
    
    int			vertex = 5;
    int			coord = 0;

    vtx->vertex.set1Value (0, SbVec3f (-zMax, 0, -xMax));
    vtx->vertex.set1Value (1, SbVec3f (-zMax, 0,  xMax));
    vtx->vertex.set1Value (2, SbVec3f ( zMax, 0,  xMax));
    vtx->vertex.set1Value (3, SbVec3f ( zMax, 0, -xMax));
    vtx->vertex.set1Value (4, SbVec3f (-zMax, 0, -xMax));

    for (; coord < vertex; coord++)
	coords.set1Value (coord, coord);
    coords.set1Value (coord++, SO_END_LINE_INDEX);
	
    //  set up co-ords for grid lines at unit intervals in x and z
    float x = -zMax;
    float z = ceil (-xMax);

    SoTranslation	*xStart = new SoTranslation;
    SoTranslation	*xTranslation = new SoTranslation;
    int			tick;
    
    int	ticks = int (2 * abs (int (x)) + 1);

    xStart->translation = SbVec3f (x, 0, -z);
    xTranslation->translation = SbVec3f (1.0, 0.0, 0.0);
    xLabels->addChild (xStart);

    for (tick = 0; tick < ticks; tick++, x++)
    {
	vtx->vertex.set1Value (vertex, SbVec3f (x, 0, -z));
	coords.set1Value (coord++, vertex++);
	vtx->vertex.set1Value (vertex, SbVec3f (x, 0, z));
	coords.set1Value (coord++, vertex++);
	coords.set1Value (coord++, SO_END_LINE_INDEX);

	text = new SoText2;
	text->justification = SoText2::CENTER;
	char textLabel [10];
	sprintf (textLabel, "%.0f", x);
	text->string = textLabel;
	xLabels->addChild (text);
	xLabels->addChild (xTranslation);
    }

    // now x
    x = -zMax;
    z = ceil (-xMax);
    if (z == -xMax)
	z += 1; // remove those end regions

    SoTranslation	*yStart = new SoTranslation;
    SoTranslation	*yTranslation = new SoTranslation;

    yStart->translation = SbVec3f (-x, 0, z);
    yTranslation->translation = SbVec3f (0, 0, 1);
    zLabels->addChild (yStart);

    ticks = int (2 * abs (int (z)) + 1);
    for (tick = 0; tick < ticks; tick++, z++)
    {
	vtx->vertex.set1Value (vertex, SbVec3f (-x, 0, z));
	coords.set1Value (coord++, vertex++);
	vtx->vertex.set1Value (vertex, SbVec3f (x, 0, z));
	coords.set1Value (coord++, vertex++);
	coords.set1Value (coord++, SO_END_LINE_INDEX);

	text = new SoText2;
	text->justification = SoText2::CENTER;
	char textLabel [10];
	sprintf (textLabel, "%.0f", z);
	text->string = textLabel;
	zLabels->addChild (text);
	zLabels->addChild (yTranslation);
    }

    grid->vertexProperty = vtx;
    grid->coordIndex = coords;
	
    // set parts
    setPart ("xLabels",		xLabels);
    setPart ("zLabels",		zLabels);
    setPart ("grid",		grid);
    setPart ("font",		font);
}

