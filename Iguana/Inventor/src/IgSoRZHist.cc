//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Inventor/interface/IgSoRZHist.h"
#include <Inventor/nodes/SoText2.h>
#include <Inventor/nodes/SoFont.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoFaceSet.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoRotation.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoIndexedFaceSet.h>
#include <Inventor/nodes/SoShapeHints.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoMaterialBinding.h>
#include <Inventor/nodes/SoIndexedLineSet.h>
#include <Inventor/nodes/SoVertexProperty.h>
#include <vector>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

SO_KIT_SOURCE (IgSoRZHist);

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>

inline double 
theta_from_eta (double eta) 
{return (2.0 * atan (exp (-eta)));}

// returns x, y, z coordinates given polar coordinates
// @ param radialCoords = ( y, eta, x)
// angle supposed to be in rad
// returning  (x, y, z)

inline SbVec3f
convertCoordinates (SbVec3f yEtaXCoords)
{
    SbVec3f cartesianCoords;
    if (yEtaXCoords [1] == 0)
    {
	cartesianCoords [0] =  yEtaXCoords [2];
	cartesianCoords [1] =  yEtaXCoords [0];
	cartesianCoords [2] =  0.0;
    }
    else if (yEtaXCoords [1] > 0)
    {
	cartesianCoords [0] =  yEtaXCoords [2];
	cartesianCoords [1] =  yEtaXCoords [0];
	cartesianCoords [2] =  yEtaXCoords [0] * tan (M_PI / 2.0 - theta_from_eta (yEtaXCoords [1]));
    }
    else
    {
	cartesianCoords [0] =  yEtaXCoords [2];
	cartesianCoords [1] =  yEtaXCoords [0];
	cartesianCoords [2] =  - yEtaXCoords [0] * tan (M_PI / 2.0 - theta_from_eta (-1.0 * yEtaXCoords [1]));
    }
    return cartesianCoords;
}

std::vector <SbVec3f>
generateUpperVertexData (std::vector <float> energies, float rMin, float rMax, 
			 float deltaEta, float xLayer, float maxEn, 
			 bool logScale, float factor)
{
    float etaRight;
    float etaLeft;
    int nbrOfBins = energies.size ();
    float etaMax = deltaEta * nbrOfBins / 2;
    
    std::vector<SbVec3f> vertexData (3 * nbrOfBins + 1);

    vertexData [0] = convertCoordinates (SbVec3f (rMin, -etaMax, xLayer));
    for (unsigned int i = 0; i < energies.size (); i++)
    {
	etaRight = - (etaMax - (etaMax / nbrOfBins * 2.0) * (i  + 1));
	etaLeft  = - (etaMax - (etaMax / nbrOfBins * 2.0) * i);
	// for liner scale
        if (! logScale)
	{
	    // if rMax == -1, print energy towers according to energy amount in bin
	    if (rMax == -1)
	    {
		vertexData [i * 3 + 1] = convertCoordinates (SbVec3f (rMin, etaRight, xLayer));
		vertexData [i * 3 + 2] = convertCoordinates (SbVec3f (rMin + factor * energies [i], etaRight, xLayer));
		vertexData [i * 3 + 3] = convertCoordinates (SbVec3f (rMin + factor * energies [i], etaLeft, xLayer));
	    }
	    // otherwise scale the towers to [rMin, rMax] such that the maximum energy results in a tower of height rMax
	    else
	    {
		vertexData [i * 3 + 1] = convertCoordinates (SbVec3f (rMin, etaRight , xLayer));
		vertexData [i * 3 + 2] = convertCoordinates (SbVec3f (rMin + energies [i] / maxEn * (rMax - rMin), etaRight, xLayer));
		vertexData [i * 3 + 3] = convertCoordinates (SbVec3f (rMin + energies [i] / maxEn * (rMax - rMin), etaLeft, xLayer));
	    }
        }
	// if logScale == true, use energies2 = log(energies)
        else
        {
	    if ((rMax >= rMin) || (rMax == -1))
	    {
		vertexData [i * 3 + 1] = convertCoordinates (SbVec3f (rMin, etaRight,  xLayer));
		vertexData [i * 3 + 2] = convertCoordinates (SbVec3f (rMin + energies [i], etaRight, xLayer));
		vertexData [i * 3 + 3] = convertCoordinates (SbVec3f (rMin + energies [i], etaLeft, xLayer));
	    }
	    else
	    {
		vertexData [i * 3 + 1] = convertCoordinates (SbVec3f (rMin, etaRight, xLayer));
		vertexData [i * 3 + 2] = convertCoordinates (SbVec3f (rMin - energies [i], etaRight, xLayer));
		vertexData [i * 3 + 3] = convertCoordinates (SbVec3f (rMin - energies [i], etaLeft, xLayer));
	    }
        }
    }
    return vertexData;
}

std::vector<SbVec3f>  
generateLowerVertexData (std::vector<float> energies, float rMin, float rMax, 
			 float deltaEta, float xLayer, float maxEn, 
			 bool logScale, float factor)
{
    float etaRight;
    float etaLeft;
    int nbrOfBins = energies.size ();
    float etaMax = deltaEta * nbrOfBins / 2;
    
    std::vector <SbVec3f> vertexData (3 * nbrOfBins + 1);

    vertexData [0] = convertCoordinates (SbVec3f (-rMin, etaMax, xLayer));
    for (unsigned int i = 0; i < energies.size (); i++)
    {
	etaRight = (etaMax - deltaEta * (i + 1));
	etaLeft = (etaMax - deltaEta * (i));	// for liner scale
        if (! logScale)
	{
	    // if rMax == -1, print energy towers according to energy amount in bin
	    if (rMax == -1)
	    {
		vertexData [i * 3 + 1] = convertCoordinates (SbVec3f (-rMin, etaRight, xLayer));
		vertexData [i * 3 + 2] = convertCoordinates (SbVec3f (-rMin -  factor * energies [i], etaRight, xLayer));
		vertexData [i * 3 + 3] = convertCoordinates (SbVec3f (-rMin -  factor * energies [i], etaLeft, xLayer));
	    }
	    // otherwise scale the towers to [rMin, rMax] such that the maximum energy results in a tower of height rMax
	    else
	    {
		vertexData [i * 3 + 1] = convertCoordinates (SbVec3f (-rMin, etaRight, xLayer));
		vertexData [i * 3 + 2] = convertCoordinates (SbVec3f (-rMin - energies [i] / maxEn * (rMax - rMin), etaRight, xLayer));
		vertexData [i * 3 + 3] = convertCoordinates (SbVec3f (-rMin - energies [i] / maxEn * (rMax - rMin), etaLeft, xLayer));
	    }
        }
	// if logScale == true, use energies2 = log(energies)
        else
        {
	    if ((rMax >= rMin) || (rMax == -1))
	    {
		vertexData [i * 3 + 1] = convertCoordinates (SbVec3f (-rMin, etaRight,  xLayer));
		vertexData [i * 3 + 2] = convertCoordinates (SbVec3f (-rMin - energies [i], etaRight, xLayer));
		vertexData [i * 3 + 3] = convertCoordinates (SbVec3f (-rMin - energies [i], etaLeft, xLayer));
	    }
	    else
	    {
		vertexData [i * 3 + 1] = convertCoordinates (SbVec3f (-rMin, etaRight, xLayer));
		vertexData [i * 3 + 2] = convertCoordinates (SbVec3f (-rMin + energies [i], etaRight, xLayer));
		vertexData [i * 3 + 3] = convertCoordinates (SbVec3f (-rMin + energies [i], etaLeft, xLayer));
	    }
        }
    }
    return vertexData;
}

//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>
void
IgSoRZHist::initClass (void)
{ SO_KIT_INIT_CLASS (IgSoRZHist, IgSoShapeKit, "IgSoShapeKit"); }

IgSoRZHist::IgSoRZHist (void)
{
    SO_KIT_CONSTRUCTOR (IgSoRZHist);
    // parameters of the constructor
    SO_KIT_ADD_FIELD (minRadius,		(1.806));
    SO_KIT_ADD_FIELD (maxRadius,		(2.864));
    SO_KIT_ADD_FIELD (upper,        		(true));
    SO_KIT_ADD_FIELD (energies,     	        (0.0));
    SO_KIT_ADD_FIELD (logScale,                 (false));
    SO_KIT_ADD_FIELD (layer,                    (0.0));
    SO_KIT_ADD_FIELD (scaleFactor,              (0.1));
    SO_KIT_ADD_FIELD (showAnnotations,          (false));
    SO_KIT_ADD_FIELD (deltaEta,              	(0.087));
    SO_KIT_ADD_CATALOG_ENTRY (shapeHints, SoShapeHints, FALSE, separator,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY (faceSet, SoIndexedFaceSet, FALSE, separator,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY (lines, SoIndexedLineSet, FALSE, separator,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY (ruler, SoSeparator, FALSE, separator,\x0, TRUE);

    SO_KIT_INIT_INSTANCE ();
    setUpConnections (true, true);
}

void
IgSoRZHist::refresh (void)
{
    int		nbrOfBins  = energies.getNum ();
    if (nbrOfBins == 0)
    {
        setPart ("shapeHints", NULL);
        setPart ("faceSet",    NULL);
        setPart ("lines",      NULL);
        setPart ("ruler",      NULL);
	return;
    }

    float       maxEn  	   = 0.0;
    int		maxEnIndex = -1;
    for (int i = 0; i < nbrOfBins; i++)
    {
	if (energies [i] > maxEn)
	{
	    maxEn = energies [i];
	    maxEnIndex = i;
	}
    }
    
    SoIndexedFaceSet* 	faceSet 	= new SoIndexedFaceSet;
    SoIndexedLineSet* 	lineSet 	= new SoIndexedLineSet;
    SoShapeHints* 	shapeHints 	= new SoShapeHints;
    SoVertexProperty* 	vtx 		= new SoVertexProperty;
    
    float		rMin 		= minRadius.getValue ();
    float		rMax 		= maxRadius.getValue ();
    float               zLayer 		= layer.getValue ();
    float		factor 		= scaleFactor.getValue ();
    bool		logarithmic 	= logScale.getValue ();
    bool		up 		= upper.getValue ();
    float		dEta 		= deltaEta.getValue ();
   
    shapeHints->faceType = SoShapeHints::CONVEX;

    // prepare space for  a local copy of the energy vector
    std::vector <float> energy (nbrOfBins);
    
    // for logscale  apply  log to contents of the energy vector
    if (logarithmic)
    {
	for (int i = 0; i < nbrOfBins; i++)
	{
	    energy [i] = factor  * log (energies [i] + 1.0);
	}
    }
    else
    {
	for (int i = 0; i < nbrOfBins; i++)
	{
	    energy [i] = energies[i];
	}
    }
    
    /* building  vector containing the vertices for the faces of the shape*/
    std::vector<SbVec3f> vertexData (3 * nbrOfBins + 1);

    if (up)
    {
	vertexData = generateUpperVertexData (energy, rMin, rMax, dEta, zLayer, maxEn, logarithmic, factor);
	shapeHints->vertexOrdering = SoShapeHints::COUNTERCLOCKWISE;
    }
    else
    {
	vertexData = generateLowerVertexData (energy, rMin, rMax, dEta, zLayer, maxEn, logarithmic, factor);
	shapeHints->vertexOrdering = SoShapeHints::CLOCKWISE;
    }
    
    /* building  vector indicating the vertices belonging to each face, however care for faces with energy = 0 , do not draw these*/
    int nbrOfDrawnFaces = 0;
    std::vector<int> faceIndices (nbrOfBins * 5);
    
    if (! (energy [0] == 0))
    {
	faceIndices [0] = 0;
	faceIndices [1] = 1;
	faceIndices [2] = 2;
	faceIndices [3] = 3;
	faceIndices [4] = SO_END_FACE_INDEX;
	nbrOfDrawnFaces++;
    }
    
    for (int i = 1; i < nbrOfBins; i++)
    {
	if (! (energy [i] == 0))
	{
	    faceIndices [nbrOfDrawnFaces * 5] = (i - 1) * 3 + 1;
	    faceIndices [nbrOfDrawnFaces * 5 + 1] = (i - 1) * 3 + 4;
	    faceIndices [nbrOfDrawnFaces * 5 + 2] = (i - 1) * 3 + 5;
	    faceIndices [nbrOfDrawnFaces * 5 + 3] = (i - 1) * 3 + 6;
	    faceIndices [nbrOfDrawnFaces * 5 + 4] = SO_END_FACE_INDEX;
	    nbrOfDrawnFaces++;
      	}
    }
    
    vtx->vertex.setValues (0, nbrOfBins * 3 + 1 , &vertexData [0]);
    vtx->materialBinding = SoMaterialBinding::PER_FACE_INDEXED;
    
    faceSet->coordIndex.setValues (0, nbrOfDrawnFaces * 5, &faceIndices [0]);
    faceSet->vertexProperty = vtx;

    // building  vector indication the vertices belonging to each line
    std::vector<int>  lineIndices (2 * nbrOfBins + 1 + 1);

    for (int i = 0; i < nbrOfBins; i++)
    {
        lineIndices [2 * i] = 3 * i + 3;
        lineIndices [2 * i + 1] = 3 * i + 2;
    }

    lineIndices [2 * nbrOfBins] = 3 * nbrOfBins - 2;
    lineIndices [2 * nbrOfBins + 1] = 0;
    
    SoVertexProperty* lineVtx = new SoVertexProperty;
    lineVtx->vertex.setValues (0, nbrOfBins * 3 + 1, &vertexData [0]);
    
    lineSet->coordIndex.setValues (0, 2 * nbrOfBins + 2, &lineIndices [0]);
    lineSet->vertexProperty = lineVtx;

    // building the scene graph for the histogram faces and outline
    if (nbrOfDrawnFaces)
    {
	setPart ("shapeHints", shapeHints);
	setPart ("faceSet", faceSet);
	setPart ("lines", lineSet);
    }

    ///////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////// constructing a ruler indicating the maximum energy//////////////
    ///////////////////////////////////////////////////////////////////////////////////////////

    // if there is at least one bin with energy > 0 and  annotations are requested
    if (nbrOfDrawnFaces && showAnnotations.getValue ())
    {
	float offset;
	int nbrOfDivisions;
	float divisionLength;
	float  rulerAngle = (dEta * nbrOfBins / 2 - dEta * (maxEnIndex + 0.5));
	float  rulerHeight;

	if (up)
	{
	    rulerAngle = -rulerAngle;
	}
	
	// find the appropriate length for the ruler
	if (! logarithmic)
	{
	    // if unconstrained linear scale is used, the length is proportional to the maximum found energy sum otherwise to (rMax - rMin)
	    rulerHeight = (rMax == -1 ) ? factor * maxEn : rulerHeight = rMax - rMin;
	    if (! up)
	    {
		rulerHeight = -rulerHeight;
	    }
	}
	else
	{
	    if ((rMax > rMin) || (rMax == -1))
	    {
		rulerHeight = (up) ? factor * log (maxEn + 1.0) : -factor * log (maxEn + 1.0);
	    }
	    else
	    {
		rulerHeight = (up) ? -factor * log (maxEn + 1.0) : factor * log (maxEn + 1.0);
	    }
	}

	// if the axis is not too small, prepare drawing
	if ((rulerHeight > 0.1)|| (rulerHeight < -0.1))
	{
	    // ingredients for the ruler
	    SoSeparator* ruler = new  SoSeparator;
	    SoMaterial* axisMaterial = new SoMaterial;

	    // define the material for the axis
	    axisMaterial->diffuseColor.setValue (0.0, 0.0, 0.0); // orange
	    
	    // the offset has to be computed in order to compute the correct division length
	    offset = (rulerHeight >= 0.0) ? (rulerHeight * (1.0 -  floor (0.1 * maxEn) / (maxEn * 0.1))) : (-rulerHeight * (1.0 -  floor (0.1 * maxEn) / (maxEn * 0.1)));
	    
	    //determin number of division markers and length of a single division
	    if (! logarithmic)
	    {
		nbrOfDivisions = (int) floor (maxEn * 0.1);// factor removed
		if (nbrOfDivisions == 0)
		{
		    nbrOfDivisions = 1;
		    divisionLength = rulerHeight;
		}
		else
		{
		    divisionLength = (rulerHeight - offset) / nbrOfDivisions;
		    if (! (offset == 0) && (offset > 0.1 * rulerHeight * rulerHeight))
		    {
			nbrOfDivisions += 1;
		    }
		}
	    }
	    //our axis is not able to display logarithmic scale, so in logScale case we do not draw division markers
	    else
	    {
		nbrOfDivisions = 1;
		divisionLength = rulerHeight;
	    }
	    
	    if ((divisionLength * divisionLength) < 0.01)
	    {
		nbrOfDivisions = 1;
		divisionLength = rulerHeight;
	    }
	    
	    float zDist = -0.05;
	    float w = dEta;

	    std::vector<SbVec3f>  rulerVtx (7 + 4 * nbrOfDivisions );
	    float rMinSigned = (up) ? rMin : -rMin;
	    rulerVtx [0] = convertCoordinates (SbVec3f (rMinSigned,                            rulerAngle - w /  8.0, zLayer + zDist));
	    rulerVtx [1] = convertCoordinates (SbVec3f (rMinSigned + 9.0 / 10.0 * rulerHeight, rulerAngle - w / 16.0, zLayer + zDist));
	    rulerVtx [2] = convertCoordinates (SbVec3f (rMinSigned + 9.0 / 10.0 * rulerHeight, rulerAngle + w / 16.0, zLayer + zDist));
	    rulerVtx [3] = convertCoordinates (SbVec3f (rMinSigned,                            rulerAngle + w /  8.0, zLayer + zDist));
	    rulerVtx [4] = convertCoordinates (SbVec3f (rMinSigned + 9.0 / 10.0 * rulerHeight, rulerAngle - w /  8.0, zLayer + zDist));
	    rulerVtx [5] = convertCoordinates (SbVec3f (rMinSigned + rulerHeight,              rulerAngle,            zLayer + zDist));
	    rulerVtx [6] = convertCoordinates (SbVec3f (rMinSigned + 9.0 / 10.0 * rulerHeight, rulerAngle + w /  8.0, zLayer + zDist));
	    
	    for (int i = 1; i < nbrOfDivisions; i++)
	    {
		rulerVtx [3 + i * 4]	 = convertCoordinates (SbVec3f (rMinSigned + divisionLength * i - 0.001, rulerAngle - w / 3.0, zLayer + zDist));
		rulerVtx [3 + i * 4 + 1] = convertCoordinates (SbVec3f (rMinSigned + divisionLength * i + 0.001, rulerAngle - w / 3.0, zLayer + zDist));
		rulerVtx [3 + i * 4 + 2] = convertCoordinates (SbVec3f (rMinSigned + divisionLength * i + 0.001, rulerAngle + w / 3.0, zLayer + zDist));
		rulerVtx [3 + i * 4 + 3] = convertCoordinates (SbVec3f (rMinSigned + divisionLength * i - 0.001, rulerAngle + w / 3.0, zLayer + zDist));
	    }
	    
	    SoCoordinate3* rulerCoords = new SoCoordinate3;
	    rulerCoords->point.setValues (0, 7 + 4 * (nbrOfDivisions - 1) , &rulerVtx [0]);
	    
	    std::vector <int>  numVtx (1 + nbrOfDivisions);
	    numVtx [0] = 4;
	    numVtx [1] = 3;
	    for (int i = 0; i < (nbrOfDivisions - 1); i++)
	    {
		numVtx [2 + i] = 4;
	    }
	    
	    SoFaceSet* rulerFaceSet = new SoFaceSet;
	    rulerFaceSet->numVertices.setValues (0, 2 + nbrOfDivisions - 1 , &numVtx [0]);

	    // finally label the axis with appropriate unit
	    char label [50];
	    if (maxEn >= 100.0)
	    {
		sprintf (label, "Emax = %.1f TeV ", maxEn / 1000.0);
	    }
	    else
	    {
		sprintf (label, "Emax = %.1f GeV ", maxEn);
	    }

	    SoText2* labelText = new SoText2;
	    labelText->string = label;
	    labelText->justification = SoText2::CENTER;
	    
	    SoFont* labelFont = new SoFont;
	    labelFont->size.setValue (16.0);
	    labelFont->name.setValue ("Times-Roman");
	    SoTranslation* labelTrans = new SoTranslation;
	    
	    if ((rMax >= rMin)|| (rMax == -1))
	    {
		labelTrans->translation.setValue (convertCoordinates (SbVec3f (1.1 * (rMinSigned + rulerHeight), rulerAngle, zLayer + zDist)));
	    }
	    else
	    {
		labelTrans->translation.setValue (convertCoordinates (SbVec3f (0.9 * (rMinSigned + rulerHeight), rulerAngle, zLayer + zDist)));
	    }
	    
	    // build the ruler and add it to the scene graph
	    ruler->addChild (axisMaterial);
	    ruler->addChild (rulerCoords);
	    ruler->addChild (rulerFaceSet);
	    ruler->addChild (labelFont);
	    ruler->addChild (labelTrans);
	    ruler->addChild (labelText);
	    
	    setPart ("ruler",   ruler);
	}
    }
}
