//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Inventor/interface/IgSoLegoTowers.h"
#include <Inventor/nodes/SoText2.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoFont.h>
#include <Inventor/nodes/SoIndexedLineSet.h>
#include <Inventor/nodes/SoSeparator.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

SO_KIT_SOURCE (IgSoLegoTowers);

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

void
IgSoLegoTowers::initClass (void)
{ SO_KIT_INIT_CLASS (IgSoLegoTowers, IgSoShapeKit, "IgSoShapeKit"); }

IgSoLegoTowers::IgSoLegoTowers (void)
{
    SO_KIT_CONSTRUCTOR (IgSoLegoTowers);
    SO_KIT_ADD_FIELD (heightScale,	(1.0f));

    SO_KIT_ADD_CATALOG_ENTRY (font,		SoFont,		FALSE, separator,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY (phiLabels,	SoSeparator,	FALSE, separator,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY (etaLabels,	SoSeparator,	FALSE, separator,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY (labels,           SoSeparator,	FALSE, separator,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY (labelScale,	SoSeparator,	FALSE, labels,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY (labelX,		SoSeparator,	FALSE, separator,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY (labelZ,		SoSeparator,	FALSE, separator,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY (grid,		SoIndexedLineSet, FALSE, separator,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY (minorGridSep,	SoSeparator,	FALSE, separator,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY (legos,		SoSeparator,	FALSE, separator,\x0, TRUE);
    SO_KIT_INIT_INSTANCE ();
    setUpConnections (true, true);
}

void
IgSoLegoTowers::refresh (void)
{
    static const float LABEL_OFFSET = 0.3F;
    float maxEta = 5.0F;

    SoFont		*font		= new SoFont;
    font->name.setValue ("Times-Roman");
    font->size.setValue (14.0);
    
    SoSeparator		*phiLabels	= new SoSeparator;
    SoSeparator		*etaLabels	= new SoSeparator;
    SoIndexedLineSet	*grid		= new SoIndexedLineSet;
    SoSeparator 	*minorGridSep 	= new SoSeparator;
    SoSeparator		*labelScale	= new SoSeparator;
    SoFont		*labelScaleFont	= new SoFont;
    labelScaleFont->name.setValue ("Times-Roman");
    labelScaleFont->size.setValue (18.0);
   
    SoSeparator		*labelX		= new SoSeparator;
    SoSeparator		*labelZ		= new SoSeparator;
    SoTranslation	*labelScaleOffset = new SoTranslation;
    SoText2		*labelScaleText	= new SoText2;
    SoTranslation	*labelXOffset	= new SoTranslation;
    SoText2		*labelXText	= new SoText2;
    SoTranslation	*labelZOffset	= new SoTranslation;
    SoText2		*labelZText	= new SoText2;
    SoVertexProperty	*vtx		= new SoVertexProperty;
    SoVertexProperty	*minorVtx	= new SoVertexProperty;
    SoText2		*text;
    SoMFInt32		coords;

    SoIndexedLineSet	*minorGrid	= new SoIndexedLineSet;
    SoMFInt32		minorCoords;
    
    //  set up co-ords for an overall outline plus a one unit vertical scale mark
    float		x = float (2 * M_PI);
    float		z = 5.0;
    int			vertex = 6;
    int			coord = 0;
    int 		minorVertex = 0;
    int		 	minorCoord = 0;
    
    vtx->vertex.set1Value (0, SbVec3f (0, 0, -z));
    vtx->vertex.set1Value (1, SbVec3f (0, 0,  z));
    vtx->vertex.set1Value (2, SbVec3f (x, 0,  z));
    vtx->vertex.set1Value (3, SbVec3f (x, 0, -z));
    vtx->vertex.set1Value (4, SbVec3f (0, 0, -z));
    vtx->vertex.set1Value (5, SbVec3f (0, 1, -z));
    for (; coord < vertex; coord++)
	coords.set1Value (coord, coord);
    coords.set1Value (coord++, SO_END_LINE_INDEX);
	
    x = 0;
    z = 5.0;

    SoText2		*phiLabelText [19];
    SoTranslation	*phiStart = new SoTranslation;
    SoTranslation	*phiTranslation = new SoTranslation;
    int			ticks = 18;
    int			tick;

    phiStart->translation = SbVec3f (M_PI / 4, 0, -z-2*LABEL_OFFSET);
    phiTranslation->translation = SbVec3f (M_PI / 4, 0, 0);
    phiLabels->addChild (phiStart);

    for (tick = 0; tick < ticks; tick++, x += (2 * M_PI) / 18.0)
    {
	vtx->vertex.set1Value (vertex, SbVec3f (x, 0, -z));
	coords.set1Value (coord++, vertex++);
	vtx->vertex.set1Value (vertex, SbVec3f (x, 0, z));
	coords.set1Value (coord++, vertex++);
	coords.set1Value (coord++, SO_END_LINE_INDEX);

	for (int ttow = 1; ttow < 4; ttow++)
	{
	    minorVtx->vertex.set1Value (minorVertex, SbVec3f (x + ttow * ((2 * M_PI) / 18.0) / 4.0, 0, -3.0));
	    minorCoords.set1Value (minorCoord++, minorVertex++);
	    minorVtx->vertex.set1Value (minorVertex, SbVec3f (x + ttow * ((2 * M_PI) / 18.0) / 4.0, 0, 3.0));
	    minorCoords.set1Value (minorCoord++, minorVertex++);
	    minorCoords.set1Value (minorCoord++, SO_END_LINE_INDEX);
	}

	text = phiLabelText [tick] = new SoText2;
	phiLabels->addChild (text);
	phiLabels->addChild (phiTranslation);
    }

    text = phiLabelText [tick] = new SoText2;
    phiLabels->addChild (text);

    phiLabelText [0]->string = "PI/4";
    phiLabelText [1]->string = "PI/2";
    phiLabelText [2]->string = "3PI/4";
    phiLabelText [3]->string = "PI";
    phiLabelText [4]->string = "5PI/4";
    phiLabelText [5]->string = "3PI/2";
    phiLabelText [6]->string = "7PI/4";
    phiLabelText [7]->string = "2PI";

    // now eta
    x = 2*M_PI;
    z = ceil (-5.0);
    if (z == -5.0)
 	z += 1; // remove those end regions
    
    z = -5.0;
    for (tick = 1; tick < 5; tick++)
    {
	vtx->vertex.set1Value (vertex, SbVec3f (0, 0, z + tick * 0.5));
	coords.set1Value (coord++, vertex++);
	vtx->vertex.set1Value (vertex, SbVec3f (x, 0, z + tick * 0.5));
	coords.set1Value (coord++, vertex++);
	coords.set1Value (coord++, SO_END_LINE_INDEX);

	vtx->vertex.set1Value (vertex, SbVec3f (0, 0, -z - tick * 0.5));
	coords.set1Value (coord++, vertex++);
	vtx->vertex.set1Value (vertex, SbVec3f (x, 0, -z - tick * 0.5));
	coords.set1Value (coord++, vertex++);
	coords.set1Value (coord++, SO_END_LINE_INDEX);
    }

    ticks = 11;
    z = -1.74;
    
    for (tick = 0; tick < ticks; tick++, z += 4 * 0.087)
    {
	vtx->vertex.set1Value (vertex, SbVec3f (0, 0, z));
	coords.set1Value (coord++, vertex++);
	vtx->vertex.set1Value (vertex, SbVec3f (x, 0, z));
	coords.set1Value (coord++, vertex++);
	coords.set1Value (coord++, SO_END_LINE_INDEX);
	
	for (int ttow = 0; ttow < 4 && z < 1.74; ttow++)
	{
	    minorVtx->vertex.set1Value (minorVertex, SbVec3f (0, 0, z + ttow * ((2 * M_PI) / 18.0) / 4.0));
	    minorCoords.set1Value (minorCoord++, minorVertex++);
	    minorVtx->vertex.set1Value (minorVertex, SbVec3f (x, 0, z + ttow * ((2 * M_PI) / 18.0) / 4.0));
	    minorCoords.set1Value (minorCoord++, minorVertex++);
	    minorCoords.set1Value (minorCoord++, SO_END_LINE_INDEX);
	}
    }

    z = -2.172;
    
    vtx->vertex.set1Value (vertex, SbVec3f (0, 0, z));
    coords.set1Value (coord++, vertex++);
    vtx->vertex.set1Value (vertex, SbVec3f (x, 0, z));
    coords.set1Value (coord++, vertex++);
    coords.set1Value (coord++, SO_END_LINE_INDEX);

    vtx->vertex.set1Value (vertex, SbVec3f (0, 0, -z));
    coords.set1Value (coord++, vertex++);
    vtx->vertex.set1Value (vertex, SbVec3f (x, 0, -z));
    coords.set1Value (coord++, vertex++);
    coords.set1Value (coord++, SO_END_LINE_INDEX);
    
    z = 1.74;
    float dz [8] = { 1.83, 1.93, 2.043, 2.172, 2.322, 2.50, 2.65, 3.0};

    for (tick = 0; tick < 8; tick++)
    {
	minorVtx->vertex.set1Value (minorVertex, SbVec3f (0, 0, dz [tick]));
	minorCoords.set1Value (minorCoord++, minorVertex++);
	minorVtx->vertex.set1Value (minorVertex, SbVec3f (x, 0, dz [tick]));
	minorCoords.set1Value (minorCoord++, minorVertex++);
	minorCoords.set1Value (minorCoord++, SO_END_LINE_INDEX);
 
	minorVtx->vertex.set1Value (minorVertex, SbVec3f (0, 0, -dz[tick]));
	minorCoords.set1Value (minorCoord++, minorVertex++);
	minorVtx->vertex.set1Value (minorVertex, SbVec3f (x, 0, -dz[tick]));
	minorCoords.set1Value (minorCoord++, minorVertex++);
	minorCoords.set1Value (minorCoord++, SO_END_LINE_INDEX);
    }
    
    z = -5.0;

    SoTranslation	*etaStart = new SoTranslation;
    SoTranslation	*etaTranslation = new SoTranslation;

    etaStart->translation = SbVec3f (-2 * LABEL_OFFSET, 0, z);
    etaTranslation->translation = SbVec3f (0, 0, 1);
    etaLabels->addChild (etaStart);

    ticks = int (2 * abs (int (z)) + 1);
    for (tick = 0; tick < ticks; tick++, z ++)
    {
	text = new SoText2;
	char textLabel [10];
	sprintf (textLabel, "%.0f", z);
	text->string = textLabel;
	etaLabels->addChild (text);
	etaLabels->addChild (etaTranslation);
    }

    grid->vertexProperty = vtx;
    grid->coordIndex = coords;
    minorGrid->vertexProperty = minorVtx;
    minorGrid->coordIndex = minorCoords;
	
    // scale
    char scaleChars[12] = "1.0 GeV";
    sprintf (scaleChars, "%.2G GeV", heightScale.getValue ());
    labelScaleText->string = scaleChars;
    labelScaleOffset->translation
	= SbVec3f (-LABEL_OFFSET, 1, -maxEta - LABEL_OFFSET);

    // axis labels
    labelXOffset->translation = SbVec3f (M_PI, 0, -maxEta - 8 * LABEL_OFFSET);
    labelXText->string = "Phi";
    labelZOffset->translation = SbVec3f (-8 * LABEL_OFFSET, 0, 0);
    labelZText->string = "Eta";

    labelScale->addChild (labelScaleFont);
    labelScale->addChild (labelScaleOffset);
    labelScale->addChild (labelScaleText);

    labelX->addChild (labelXOffset);
    labelX->addChild (labelXText);
    labelZ->addChild (labelZOffset);
    labelZ->addChild (labelZText);

    // set parts
    setPart ("font",		font);
    setPart ("phiLabels",	phiLabels);
    setPart ("etaLabels",	etaLabels);
    setPart ("labelScale",	labelScale);
    setPart ("labelX",		labelX);
    setPart ("labelZ",		labelZ);
    setPart ("grid",		grid);

    SoDrawStyle		*minorDrawStyle = new SoDrawStyle;
    minorDrawStyle->linePattern = 0x5555;
    minorGridSep->addChild (minorDrawStyle);
    minorGridSep->addChild (minorGrid);

    setPart ("minorGridSep", minorGridSep);
}
