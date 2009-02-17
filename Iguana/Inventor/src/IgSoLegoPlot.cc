//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Inventor/interface/IgSoLegoPlot.h"
#include <Inventor/nodes/SoText2.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoFont.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoIndexedLineSet.h>
#include <Inventor/nodes/SoSeparator.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

SO_KIT_SOURCE (IgSoLegoPlot);

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

void
IgSoLegoPlot::initClass (void)
{ SO_KIT_INIT_CLASS (IgSoLegoPlot, IgSoShapeKit, "IgSoShapeKit"); }

IgSoLegoPlot::IgSoLegoPlot (void)
{
    SO_KIT_CONSTRUCTOR (IgSoLegoPlot);
    SO_KIT_ADD_FIELD (divsPhi,		(1));
    SO_KIT_ADD_FIELD (divsEta,		(1));
    SO_KIT_ADD_FIELD (heightScale,	(1.0f));
    SO_KIT_ADD_FIELD (maxEta, 		(4.5f));
    SO_KIT_ADD_FIELD (minEnergy,	(0.5f));
    SO_KIT_ADD_FIELD (displayNegTowers,	(FALSE));

    SO_KIT_ADD_CATALOG_ENTRY (phiLabels,	SoSeparator,	FALSE, separator,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY (etaLabels,	SoSeparator,	FALSE, separator,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY (grid,		SoIndexedLineSet, FALSE, separator,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY (font,		SoFont,		FALSE, separator,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY (labels,           SoSeparator,	FALSE, separator,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY (labelScale,	SoSeparator,	FALSE, labels,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY (labelScaleFont,	SoFont,		FALSE, labelScale,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY (labelScaleOffset,	SoTranslation,	FALSE, labelScale,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY (labelScaleText,	SoText2,	FALSE, labelScale,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY (labelX,		SoSeparator,	FALSE, labels,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY (labelXOffset,	SoTranslation,	FALSE, labelX,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY (labelXText,	SoText2,	FALSE, labelX,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY (labelZ,		SoSeparator,	FALSE, labels,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY (labelZOffset,	SoTranslation,	FALSE, labelZ,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY (labelZText,	SoText2,	FALSE, labelZ,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY (legos,		SoSeparator,	FALSE, separator,\x0, TRUE);
    SO_KIT_INIT_INSTANCE ();
    setUpConnections (true, true);
}

void
IgSoLegoPlot::refresh (void)
{
    static const float LABEL_OFFSET = 0.3F;

    SoFont		*font		= new SoFont;
    SoSeparator		*phiLabels	= new SoSeparator;
    SoSeparator		*etaLabels	= new SoSeparator;
    SoIndexedLineSet	*grid		= new SoIndexedLineSet;
    SoFont		*labelScaleFont	= new SoFont;
    SoTranslation	*labelScaleOffset = new SoTranslation;
    SoText2		*labelScaleText	= new SoText2;
    SoTranslation	*labelXOffset	= new SoTranslation;
    SoText2		*labelXText	= new SoText2;
    SoTranslation	*labelZOffset	= new SoTranslation;
    SoText2		*labelZText	= new SoText2;
    SoVertexProperty	*vtx		= new SoVertexProperty;
    SoText2		*text;
    SoMFInt32		coords;

    //  set up co-ords for an overall outline plus a one unit vertical scale mark
    float		x = float (2 * M_PI);
    float		z = maxEta.getValue ();
    int			vertex = 6;
    int			coord = 0;

    vtx->vertex.set1Value (0, SbVec3f (0, 0, -z));
    vtx->vertex.set1Value (1, SbVec3f (0, 0,  z));
    vtx->vertex.set1Value (2, SbVec3f (x, 0,  z));
    vtx->vertex.set1Value (3, SbVec3f (x, 0, -z));
    vtx->vertex.set1Value (4, SbVec3f (0, 0, -z));
    vtx->vertex.set1Value (5, SbVec3f (0, 1, -z));
    for (; coord < vertex; coord++)
	coords.set1Value (coord, coord);
    coords.set1Value (coord++, SO_END_LINE_INDEX);
	
    //  set up co-ords for grid lines at unit intervals in eta, pi/4 in phi
    //  phi lines, exclude 0 and 2pi
    x = M_PI/4;
    z = maxEta.getValue ();

    SoText2		*phiLabelText [8];
    SoTranslation	*phiStart = new SoTranslation;
    SoTranslation	*phiTranslation = new SoTranslation;
    int			ticks = 7;
    int			tick;

    phiStart->translation = SbVec3f (M_PI/4, 0, -z-2*LABEL_OFFSET);
    phiTranslation->translation = SbVec3f (M_PI/4, 0, 0);
    phiLabels->addChild (phiStart);

    for (tick = 0; tick < ticks; tick++, x += M_PI/4)
    {
	vtx->vertex.set1Value (vertex, SbVec3f (x, 0, -z));
	coords.set1Value (coord++, vertex++);
	vtx->vertex.set1Value (vertex, SbVec3f (x, 0, z));
	coords.set1Value (coord++, vertex++);
	coords.set1Value (coord++, SO_END_LINE_INDEX);

	text = phiLabelText [tick] = new SoText2;
	text->justification = SoText2::CENTER;
	phiLabels->addChild (text);
	phiLabels->addChild (phiTranslation);
    }

    text = phiLabelText [tick] = new SoText2;
    text->justification = SoText2::CENTER;
    phiLabels->addChild (text);

    phiLabelText [0]->string = "p/4";
    phiLabelText [1]->string = "p/2";
    phiLabelText [2]->string = "3p/4";
    phiLabelText [3]->string = "p";
    phiLabelText [4]->string = "5p/4";
    phiLabelText [5]->string = "3p/2";
    phiLabelText [6]->string = "7p/4";
    phiLabelText [7]->string = "2p";

    // now eta
    x = 2*M_PI;
    z = ceil (-maxEta.getValue ());
    if (z == -maxEta.getValue ())
	z += 1; // remove those end regions

    SoTranslation	*etaStart = new SoTranslation;
    SoTranslation	*etaTranslation = new SoTranslation;

    etaStart->translation = SbVec3f (-2 * LABEL_OFFSET, 0, z);
    etaTranslation->translation = SbVec3f (0, 0, 1);
    etaLabels->addChild (etaStart);

    ticks = int (2 * abs (int (z)) + 1);
    for (tick = 0; tick < ticks; tick++, z++)
    {
	vtx->vertex.set1Value (vertex, SbVec3f (0, 0, z));
	coords.set1Value (coord++, vertex++);
	vtx->vertex.set1Value (vertex, SbVec3f (x, 0, z));
	coords.set1Value (coord++, vertex++);
	coords.set1Value (coord++, SO_END_LINE_INDEX);

	text = new SoText2;
	text->justification = SoText2::CENTER;
	char textLabel [10];
	sprintf (textLabel, "%.0f", z);
	text->string = textLabel;
	etaLabels->addChild (text);
	etaLabels->addChild (etaTranslation);
    }

    grid->vertexProperty = vtx;
    grid->coordIndex = coords;
	
    // labels
#if defined WIN32
    labelScaleFont->name = "Times New Roman;Regular";
#elif defined __APPLE__
    labelScaleFont->name = "Times New Roman";
#else
    labelScaleFont->name = "Times-Roman";
#endif

    // scale
    char scaleChars[12] = "1.0 GeV";
    sprintf (scaleChars, "%.2G GeV", heightScale.getValue ());
    labelScaleText->string = scaleChars;
    labelScaleOffset->translation
	= SbVec3f (-LABEL_OFFSET, 1, -maxEta.getValue()-LABEL_OFFSET);

    // axis labels
    // font->size = 14.0; //in points-- CHANGE THE SIZE WITH EXTREME CARE
    font->name = "Symbol";
    labelXOffset->translation = SbVec3f (M_PI, 0, -maxEta.getValue() - 8 * LABEL_OFFSET);
    labelXText->string = "f";
    labelZOffset->translation = SbVec3f (-8 * LABEL_OFFSET, 0, 0);
    labelZText->string = "h";

    // set parts
    setPart ("phiLabels",	phiLabels);
    setPart ("etaLabels",	etaLabels);
    setPart ("grid",		grid);
    setPart ("font",		font);
    setPart ("labelScaleFont",	labelScaleFont);
    setPart ("labelScaleOffset",labelScaleOffset);
    setPart ("labelScaleText",	labelScaleText);
    setPart ("labelXOffset",	labelXOffset);
    setPart ("labelXText",	labelXText);
    setPart ("labelZOffset",	labelZOffset);
    setPart ("labelZText",	labelZText);
}

/// Load pre-binned data into the lego plot.
void
IgSoLegoPlot::setData (const IgSbLegoHistogram &hist)
{
    if (hist.empty ())
    {
	divsEta = divsPhi = 1;
	setPart ("legos", 0);
	return;
    }

    // data is arranged cycles first in phi, then in eta; lowest to highest
    // em then hadronic
    SoMaterial	*emMaterial = new SoMaterial;
    SoMaterial	*hadMaterial = new SoMaterial;
    SoSeparator	*legos = new SoSeparator;

    emMaterial->emissiveColor.setValue (0.2F, 0.F, 0.F);
    emMaterial->diffuseColor.setValue (0.F, 1.F, 0.F);
    hadMaterial->emissiveColor.setValue (0.2F, 0.F, 0.F);
    hadMaterial->diffuseColor.setValue (0.F, 0.F, 1.F);

    IgSbLegoBin largest = hist.rbegin ()->first;
    divsEta = largest.ix + 1;
    divsPhi = largest.iy + 1;

    // x is phi; z is eta
    float phiSize = 2 * M_PI / divsPhi.getValue ();
    float etaSize = 2 * maxEta.getValue () / divsEta.getValue ();

    for (IgSbLegoHistogram::const_iterator it = hist.begin (); it != hist.end (); it++)
    {
	float emE = it->second.emE;
	float E = it->second.E;
	if (emE > minEnergy.getValue () || E > minEnergy.getValue ())
	{
	    int iPhi = it->first.ix;
	    int iEta = it->first.iy;
	    float zEta = iEta * etaSize + etaSize / 2;
	    float xPhi = iPhi * phiSize + phiSize / 2;

	    SoSeparator		*sep = new SoSeparator;
	    SoTranslation	*trans = new SoTranslation;

	    trans->translation
		= SbVec3f (xPhi, emE / 2 / heightScale.getValue (),
			   zEta - maxEta.getValue ());
	    sep->addChild (trans);

	    float firstOffset = 0;
	    if (emE > minEnergy.getValue ())
	    {
		SoCube *cube = new SoCube;
		cube->width = phiSize;
		cube->depth = etaSize;
		cube->height = emE / heightScale.getValue ();
		sep->addChild (emMaterial);
		sep->addChild (cube);
		firstOffset = emE;
	    }

	    if (E > minEnergy.getValue ())
	    {
		trans = new SoTranslation;
		trans->translation
		    = SbVec3f (0.F, (firstOffset+E) / 2 / heightScale.getValue (), 0.F);
		sep->addChild (trans);

		SoCube *cube = new SoCube;
		cube->width = phiSize;
		cube->depth = etaSize;
		cube->height = E / heightScale.getValue ();
		sep->addChild (hadMaterial);
		sep->addChild (cube);
	    }

	    legos->addChild (sep);
	}
    }

    setPart ("legos", legos);
}
