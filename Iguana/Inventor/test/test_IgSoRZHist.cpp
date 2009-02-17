//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Inventor/interface/IgSoRZHist.h"
#include "Iguana/Inventor/interface/IgSoAxis.h"
#include <Inventor/nodes/SoRotationXYZ.h>
#include "test_main.h"
#include <vector>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

QString makeTest (SoSeparator *root)
{	
    IgSoShapeKit::initClass ();
    IgSoRZHist::initClass ();  
    
    
    float maxEta = 1.218;
    float deltaEta = 0.087;
    int nbrOfBins = (int) (maxEta / deltaEta * 2);

    std::vector<float> energies (nbrOfBins);
    for (int i = 0; i < nbrOfBins; i++)
    {
	energies [i] = 20 * (float) rand () / RAND_MAX;
    }
       
    energies [nbrOfBins / 2 + 10] = 50;

    // test configuration linear constrained positiv
    IgSoRZHist* upper = new IgSoRZHist;
    upper->minRadius = 1.5;
    upper->maxRadius = 1.806;
    upper->energies.setValues (0, nbrOfBins, &energies [0]);
    upper->logScale = 0;
    upper->layer = 0.0;
    upper->scaleFactor = 0.1;
    upper->showAnnotations = 1;
    upper->upper = true;

    IgSoRZHist* upperUnconst = new IgSoRZHist;
    upperUnconst->minRadius = 1.5;
    upperUnconst->maxRadius = -1;
    upperUnconst->energies.setValues (0, nbrOfBins, &energies [0]);
    upperUnconst->logScale = 0;
    upperUnconst->layer = 0.0;
    upperUnconst->scaleFactor = 0.1;
    upperUnconst->showAnnotations = 1;
    upperUnconst->upper = true;
    
    IgSoRZHist* upperL = new IgSoRZHist;
    upperL->minRadius = 2;
    upperL->maxRadius = 2.806;
    upperL->energies.setValues (0, nbrOfBins, &energies [0]);
    upperL->logScale = 1;
    upperL->layer = 0.0;
    upperL->scaleFactor = 0.1;
    upperL->showAnnotations = 0 ;
    upperL->upper = true;

    IgSoRZHist* upperN = new IgSoRZHist;
    upperN->minRadius = 1.5;
    upperN->maxRadius = 1.3;
    upperN->energies.setValues (0, nbrOfBins, &energies [0]);
    upperN->logScale = 0;
    upperN->layer = 0.0;
    upperN->scaleFactor = 0.1;
    upperN->showAnnotations = 1;
    upperN->upper = true;

    IgSoRZHist* upperNLog = new IgSoRZHist;
    upperNLog->minRadius = 2.0;
    upperNLog->maxRadius = 1.8;
    upperNLog->energies.setValues (0, nbrOfBins, &energies [0]);
    upperNLog->logScale = 1;
    upperNLog->layer = 0.0;
    upperNLog->scaleFactor = 0.1;
    upperNLog->showAnnotations = 0;
    upperNLog->upper = true;
    
    //////////////////////////////////////////////////
    for (int i = 0; i < nbrOfBins; i++)
    {
	energies [i] = 20 * (float) rand () / RAND_MAX;
    }
       
    energies [nbrOfBins / 2 + 10] = 10;
    energies [nbrOfBins / 2 - 10] = 50;
    SoMaterial *mat = new SoMaterial;
    mat->diffuseColor.setValue (0, 0, 1);

    IgSoRZHist* lower = new IgSoRZHist;
    lower->minRadius = 1.5;
    lower->maxRadius = -1;
    lower->energies.setValues (0, nbrOfBins, &energies [0]);
    lower->logScale = 0;
    lower->layer = 0.0;
    lower->scaleFactor = 0.1;
    lower->showAnnotations = 1;
    lower->upper = false;

    IgSoRZHist* lowerUnconst = new IgSoRZHist;
    lowerUnconst->minRadius = 5;
    lowerUnconst->maxRadius = -1;
    lowerUnconst->energies.setValues (0, nbrOfBins, &energies [0]);
    lowerUnconst->logScale = 0;
    lowerUnconst->layer = 0.0;
    lowerUnconst->scaleFactor = 0.1;
    lowerUnconst->showAnnotations = 1;
    lowerUnconst->upper = false;
    
    IgSoRZHist* lowerLog = new IgSoRZHist;
    lowerLog->minRadius = 2;
    lowerLog->maxRadius = 3;
    lowerLog->energies.setValues (0, nbrOfBins, &energies [0]);
    lowerLog->logScale = 1;
    lowerLog->layer = 0.0;
    lowerLog->scaleFactor = 0.1;
    lowerLog->showAnnotations = 0;
    lowerLog->upper = false;

    IgSoRZHist* lowerN = new IgSoRZHist;
    lowerN->minRadius = 1.5;
    lowerN->maxRadius =  1.3;
    lowerN->energies.setValues (0, nbrOfBins, &energies [0]);
    lowerN->logScale = 0;
    lowerN->layer = 0.0;
    lowerN->scaleFactor = 0.1;
    lowerN->showAnnotations = 1;
    lowerN->upper = false;

    IgSoRZHist* lowerNLog = new IgSoRZHist;
    lowerNLog->minRadius = 2.0;
    lowerNLog->maxRadius = 1.8;
    lowerNLog->energies.setValues (0, nbrOfBins, &energies [0]);
    lowerNLog->logScale = 1;
    lowerNLog->layer = 0.0;
    lowerNLog->scaleFactor = 0.1;
    lowerNLog->showAnnotations = 0;
    lowerNLog->upper = false;

    SoMaterial *mat2 = new SoMaterial;
    mat2->diffuseColor.setValue (1.0, 1.0, 0.0);
        
    root->addChild (mat);
    root->addChild (upperUnconst);
    root->addChild (lower);

    root->addChild (mat2);
    root->addChild (upperN);
    root->addChild (lowerN);

    return "IGUANA: IgSoRZHist Test";
}
