//<<<<<< INCLUDES                       >>>>>>

#include "Iguana/Inventor/interface/IgSoCircularHist.h"	
#include <Inventor/nodes/SoText2.h>
#include <Inventor/nodes/SoFont.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoRotation.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoIndexedFaceSet.h>
#include <Inventor/nodes/SoShapeHints.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoMaterialBinding.h>
#include <Inventor/nodes/SoIndexedLineSet.h>
#include <Inventor/nodes/SoVertexProperty.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoFaceSet.h>
#include <vector>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

SO_KIT_SOURCE (IgSoCircularHist);

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>

// returns x, y, z coordinates given polar coordinates
// @ param radialCoords = ( radius, angle, z)
// angle supposed to be in rad

inline SbVec3f 
convertCoordinates (SbVec3f radialCoords)
{
    SbVec3f cartesianCoords;
    cartesianCoords [0] = radialCoords [0] * cos (radialCoords [1]);
    cartesianCoords [1] = radialCoords [0] * sin (radialCoords [1]);
    cartesianCoords [2] = radialCoords [2];
    return cartesianCoords;
}

//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

void 
IgSoCircularHist::initClass (void)
{ SO_KIT_INIT_CLASS (IgSoCircularHist, IgSoShapeKit, "IgSoShapeKit"); }

IgSoCircularHist::IgSoCircularHist (void)
{
    SO_KIT_CONSTRUCTOR (IgSoCircularHist);
    // parameters of the constructor
    SO_KIT_ADD_FIELD (minRadius,		(1.3));
    SO_KIT_ADD_FIELD (maxRadius,		(1.5));
    SO_KIT_ADD_FIELD (numberOfBins,     	(180));    
    SO_KIT_ADD_FIELD (energies,     	     	(0.0)); // vector of positive energies in GeV
    SO_KIT_ADD_FIELD (logScale,               	(false));
    SO_KIT_ADD_FIELD (layer,                  	(0.0)); // determines the z position of the histogram
    SO_KIT_ADD_FIELD (scaleFactor,    	      	(0.1)); // for unconstrained linear and log scale the histograms can be scaled with this factor
    SO_KIT_ADD_FIELD (showAnnotations,        	(false));
    SO_KIT_ADD_CATALOG_ENTRY (shapeHints, SoShapeHints, FALSE, separator,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY (faceSet, SoIndexedFaceSet, FALSE, separator,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY (lines, SoIndexedLineSet, FALSE, separator,\x0, TRUE);    
    SO_KIT_ADD_CATALOG_ENTRY (ruler, SoSeparator, FALSE, separator,\x0, TRUE);
    
    SO_KIT_INIT_INSTANCE ();
    setUpConnections (true, true);
}

void
IgSoCircularHist::refresh (void)
{
    if (energies.getNum () == 0)
    {
        setPart ("shapeHints", NULL);
        setPart ("faceSet",    NULL);
        setPart ("lines",      NULL);
        setPart ("ruler",      NULL);
        return;
    }

    // ingredients for the circular histogram
    SoIndexedFaceSet* faceSet = new SoIndexedFaceSet;
    SoIndexedLineSet* lineSet = new SoIndexedLineSet;
    SoShapeHints* shapeHints = new SoShapeHints;    
    SoVertexProperty* vtx = new SoVertexProperty;   
    
    // make a local copy of SO_KIT fields
    float		rMin = minRadius.getValue ();
    float		rMax = maxRadius.getValue ();
    int                 nbrOfBins =  numberOfBins.getValue ();    
    float               zLayer = layer.getValue ();
    float		factor = scaleFactor.getValue ();
    bool 		annotations = showAnnotations.getValue ();
    bool		logarithmic = logScale.getValue ();
    
    // enabling rendering of front and back faces   
    shapeHints->vertexOrdering = SoShapeHints::COUNTERCLOCKWISE;
    shapeHints->faceType = SoShapeHints::CONVEX;

    // prepare space for  a local copy of the energy vector
    std::vector<float> energies2 (nbrOfBins);
    
    // look for bin with maximum energy sum
    float               maxEn  = 0.0;
    int			maxEnIndex = 0;
    for (int i = 0; i < nbrOfBins; i++)
    {
	if (energies [i] > maxEn)
	{
	    maxEn = energies [i];
	    maxEnIndex = i;		
	}
    }
    
    // if constrained linear scale is used find the maximum energy value
    // for logscale  apply  log to contents of the energy vector
    if (logarithmic)
    {
	for (int i = 0; i < nbrOfBins; i++)
	{
	    energies2 [i] = factor  * log (energies [i] + 1.0);	  
	}		    
    }
   
    /* building  vector containing the vertices for the faces of the shape*/
    std::vector<SbVec3f> vertexData (3 * nbrOfBins + 1);	

    vertexData [0] = convertCoordinates (SbVec3f (rMin, 0.0, zLayer));
    for (int i = 0; i < nbrOfBins ; i++)
    {
	float phi = 2.0 * M_PI / nbrOfBins * (i + 1);
	// for liner scale
        if (! logarithmic)
	{
	    // if rMax == -1, print energy towers according to energy amount in bin
	    if (rMax == -1) 
	    {
		vertexData [i * 3 + 1] = convertCoordinates (SbVec3f (rMin, phi, zLayer));
		vertexData [i * 3 + 2] = convertCoordinates (SbVec3f (rMin + factor * energies [i], phi, zLayer));
		vertexData [i * 3 + 3] = convertCoordinates (SbVec3f (rMin + factor * energies [i], 2 * M_PI / nbrOfBins * i, zLayer));      	
	    }
	    // otherwise scale the towers to [rMin, rMax] such that the maximum energy results in a tower of height rMax
	    else
	    {
		vertexData [i * 3 + 1] = convertCoordinates (SbVec3f (rMin, phi, zLayer));
		vertexData [i * 3 + 2] = convertCoordinates (SbVec3f (rMin + energies [i] / maxEn * (rMax - rMin), phi, zLayer));
		vertexData [i * 3 + 3] = convertCoordinates (SbVec3f (rMin + energies [i] / maxEn * (rMax - rMin), 2 * M_PI / nbrOfBins * i, zLayer)); 
	    }
        }
	// if logScale == true, use energies2 = log(energies)
        else
        {
	    if ((rMax >= rMin) || (rMax == -1))
	    {
		vertexData [i * 3 + 1] = convertCoordinates (SbVec3f (rMin, phi, zLayer));
		vertexData [i * 3 + 2] = convertCoordinates (SbVec3f (rMin + energies2 [i], phi, zLayer));
		vertexData [i * 3 + 3] = convertCoordinates (SbVec3f (rMin + energies2 [i], 2 * M_PI / nbrOfBins * i, zLayer));      
	    }
	    else
	    {
		vertexData [i * 3 + 1] = convertCoordinates (SbVec3f (rMin, phi, zLayer));
		vertexData [i * 3 + 2] = convertCoordinates (SbVec3f (rMin - energies2 [i], phi, zLayer));
		vertexData [i * 3 + 3] = convertCoordinates (SbVec3f (rMin - energies2 [i], 2 * M_PI / nbrOfBins * i, zLayer));  
	    }
        }
    }
   
    /* building  vector indicating the vertices belonging to each face*/
    std::vector<int> faceIndices (nbrOfBins * 5);
    faceIndices [0] = 0;
    faceIndices [1] = 1;
    faceIndices [2] = 2;
    faceIndices [3] = 3;
    faceIndices [4] = SO_END_FACE_INDEX;

    for (int i = 1; i < nbrOfBins; i++)
    { 
        faceIndices [i * 5] = (i - 1) * 3 + 1;
        faceIndices [i * 5 + 1] = (i - 1) * 3 + 4;
        faceIndices [i * 5 + 2] = (i - 1) * 3 + 5;
        faceIndices [i * 5 + 3] = (i - 1) * 3 + 6;
        faceIndices [i * 5 + 4] = SO_END_FACE_INDEX; 
    }
    
    vtx->vertex.setValues (0, nbrOfBins * 3 + 1 ,&vertexData [0]);
    vtx->materialBinding = SoMaterialBinding::PER_FACE_INDEXED;
    
    faceSet->coordIndex.setValues (0, nbrOfBins * 5, &faceIndices [0]);
    faceSet->vertexProperty = vtx;

    // building  vector indication the vertices belonging to each line
    std::vector<int>  lineIndices (2 * nbrOfBins + 1);
    
    for (int i = 0; i < nbrOfBins; i++)
    {
        lineIndices [2 * i] = 3 * i + 3;
        lineIndices [2 * i + 1] = 3 * i + 2;      
    }
    lineIndices [2 * nbrOfBins] = 3;
  
    SoVertexProperty* lineVtx = new SoVertexProperty;
    lineVtx->vertex.setValues (0, nbrOfBins * 3 + 1, &vertexData [0]);
  
    lineSet->coordIndex.setValues (0, 2 * nbrOfBins + 1, &lineIndices [0]);
    lineSet->vertexProperty = lineVtx;

    /////////////////////////////////////////////////////////////////////////////////       
    /// buiding the scene graph including faces and lines of bins ///////////////////
    setPart ("shapeHints", shapeHints);
    setPart ("faceSet", faceSet);
    setPart ("lines", lineSet); 	    

    ///////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////// constructing a ruler indicating the maximum energy//////////////
    ///////////////////////////////////////////////////////////////////////////////////////////
    // compute all the stuff only if annotations are requested ///////////////////////////////
    if (annotations)
    {	
	float offset; // fraction or ruler length equivalent to remainder of (maximum energy div 10 GeV)
	int nbrOfDivisions;   
	float divisionLength; 
	float rulerLength; //ruler length =  nbrOfDivisions*divisionLength + offset 

	// find the appropriate length for the scale axis
	// in case of negativ energies the ruler length is negative
	if (! logarithmic)
	{
	    // if unconstrained linear scale is used, the length equals the maximum found energy sum otherwise the rMax - rMin	   
	    rulerLength = (rMax == -1 ) ? factor  *  maxEn : rMax - rMin;
	}		
	// for log scale the length equals log( maxEn)
	else
	{
	    rulerLength = ((rMax >= rMin) || (rMax == -1)) ? energies2 [maxEnIndex] : (-energies2 [maxEnIndex]);
	}	

	// if the axis is not too small, prepare drawing
	if ((rulerLength > 0.1) || (rulerLength < -0.1))
	{
	    // ingredients for the ruler
	    SoSeparator* ruler = new  SoSeparator;
	    SoMaterial* axisMaterial = new SoMaterial;    	  
	    SoRotation* zrot = new SoRotation;
	    //SoTranslation* ytrans = new SoTranslation;
	    
	    // define the material for the axis
	    axisMaterial->diffuseColor.setValue(0.0, 0.0, 0.0); // orange	   
	    	
	    // the offset has to be computed in order to compute the correct division length
	    offset = (rulerLength >= 0) ? (rulerLength * (1.0 -  floor (0.1 * maxEn) / (maxEn * 0.1))) : (-rulerLength * (1.0 -  floor (0.1 * maxEn) / (maxEn * 0.1)));
	   
	    if (! logarithmic)
	    {
		nbrOfDivisions = static_cast<int>(floor (maxEn * 0.1));

		if (nbrOfDivisions == 0)
		{
		    nbrOfDivisions = 1;
		    divisionLength = rulerLength;
		}
		else
		{
		    divisionLength = (rulerLength - offset) / nbrOfDivisions;
		    // if (offset - divisionLength) is larger then the arrowhead draw one marker more
		    if (! (offset == 0) && ((offset - divisionLength) > 0.1  * rulerLength))
		    {
			nbrOfDivisions += 1;
		    }
		}		
	    }
	    //our axis is not able to display logarithmic scale, so in logScale case we do not draw division markers
	    else 
	    {
		nbrOfDivisions = 1;
		divisionLength = rulerLength;
	    }
	    
	    float w = 2 * M_PI / nbrOfBins; // bin width in rad 
	    float rulerAngle =  2 * M_PI / nbrOfBins * (float (maxEnIndex) + 0.5) ; // angle of the bin with highest energy
	    float zDist = 0.001; // put the ruler slightly above the Histogram by adding zDist to z coordinate to make it visible

	    // construct the ruler vertices, first a quad and an triangle for the arrow, than the devision markers
	    std::vector<SbVec3f>  rulerVtx (7 + 4 * nbrOfDivisions);         
	    
	    rulerVtx [0] = convertCoordinates (SbVec3f (rMin,	 			rulerAngle - w / 4.0, 	zLayer + zDist));	    
	    rulerVtx [1] = convertCoordinates (SbVec3f (rMin + 9.0/10.0*rulerLength,	rulerAngle - w / 8.0, 	zLayer + zDist));	    
	    rulerVtx [2] = convertCoordinates (SbVec3f (rMin + 9.0/10.0*rulerLength,	rulerAngle + w / 8.0, 	zLayer + zDist));	    
	    rulerVtx [3] = convertCoordinates (SbVec3f (rMin, 		   		rulerAngle + w / 4.0, 	zLayer + zDist));	    
	    rulerVtx [4] = convertCoordinates (SbVec3f (rMin + 9.0/10.0*rulerLength, 	rulerAngle - w / 4.0, 	zLayer + zDist));	    
	    rulerVtx [5] = convertCoordinates (SbVec3f (rMin + rulerLength, 		rulerAngle, 		zLayer + zDist));	    
	    rulerVtx [6] = convertCoordinates (SbVec3f (rMin + 9.0/10.0*rulerLength, 	rulerAngle + w / 4.0, 	zLayer + zDist));    
	    
	    for (int i = 1; i < nbrOfDivisions; i++)
	    {
		rulerVtx [3+ i * 4]     = convertCoordinates (SbVec3f (rMin + divisionLength * i - 0.01, rulerAngle - w / 3.0, 	zLayer + zDist));  
		rulerVtx [3+ i * 4 + 1] = convertCoordinates (SbVec3f (rMin + divisionLength * i + 0.01, rulerAngle - w / 3.0, 	zLayer + zDist));
		rulerVtx [3+ i * 4 + 2] = convertCoordinates (SbVec3f (rMin + divisionLength * i + 0.01, rulerAngle + w / 3.0, 	zLayer + zDist));
		rulerVtx [3+ i * 4 + 3] = convertCoordinates (SbVec3f (rMin + divisionLength * i - 0.01, rulerAngle + w / 3.0, 	zLayer + zDist));
	    }
	    
	    SoCoordinate3* rulerCoords = new SoCoordinate3;
	    rulerCoords->point.setValues (0, 7 + 4 * (nbrOfDivisions - 1), &rulerVtx [0]);

	    std::vector<int> numVtx (1 + nbrOfDivisions);
	    numVtx [0] = 4;
	    numVtx [1] = 3;
	    for (int i = 0; i < (nbrOfDivisions - 1); i++)
	    {
		numVtx [2 + i] = 4;
	    }
	    
	    SoFaceSet* rulerFaceSet = new SoFaceSet;
	    rulerFaceSet->numVertices.setValues (0, 2 + nbrOfDivisions - 1 , &numVtx [0]);
	    
	    // prepare the label of the ruler and translate it to the proper place on the arrowhead
	    // label the axis with appropriate unit, if maxEn > 100 GeV, change to TeV
	    char label [50];
	    if (maxEn >= 100.0)
	    {
		sprintf (label, "Emax = %.1f TeV", maxEn / 1000.0);
	    }
	    else
	    {
		sprintf (label, "Emax = %.1f GeV", maxEn);
	    }
	    
	    SoFont* labelFont = new SoFont;
	    labelFont->size.setValue (16.0);
	    labelFont->name.setValue ("Times-Roman");

	    SoText2* labelText = new SoText2;
	    labelText->string = label;
	    labelText->justification = SoText2::CENTER;

	    SoTranslation* labelTrans = new SoTranslation;

	    // compute the rotation and translation to align the label with the bin with the maximum energy sum
	    // if unconstrained linear scale or linear / logarithmic scale for positive energies  is used , axis has always to point outside
	    // rotate to middle of wanted bin which has  phi = 2 * M_PI / nbrOfBins *( float(maxEnIndex) + 0.5)
	    if ((rMax == -1) || (rMax >= rMin))
	    {
		zrot->rotation.setValue (SbVec3f (0.0, 0.0, 1.0), - (M_PI / 2 - 2 * M_PI / nbrOfBins * (float (maxEnIndex) + 0.5)));
		labelTrans->translation.setValue (0.0, 1.1 * (rMin + rulerLength), zLayer + zDist);
	    }
	    // else hawe have constrained linear for negative energies or logarithmic scale for negative energies, hence ruler  points inwards
	    else
	    {
		zrot->rotation.setValue (SbVec3f (0.0, 0.0, 1.0), - (M_PI / 2 - 2 * M_PI / nbrOfBins * (float (maxEnIndex) + 0.5)) + M_PI);
		labelTrans->translation.setValue (0.0, -0.9 * (rMin + rulerLength), zLayer + zDist);
	    }
	    
	    // now construct the ruler and add it to the scene graph
	    ruler->addChild (axisMaterial);
	    ruler->addChild (rulerCoords);
	    ruler->addChild (rulerFaceSet);
	    ruler->addChild (labelFont);
	    ruler->addChild (zrot);
	    ruler->addChild (labelTrans);
	    ruler->addChild (labelText);
	    
	    setPart ("ruler",   ruler);
	}
    }
}
