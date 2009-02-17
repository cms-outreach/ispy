//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Inventor/interface/IgSoCircularHist.h"
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
    IgSoCircularHist::initClass ();
    
    float maxEn = 5;
    int  bins = 72;
    
    std::vector<float> energies (bins);
    for (int i = 0; i < bins; i++)
    {
        energies [i] = maxEn * (float) rand () / RAND_MAX;	
    }
    energies [0] = 0.0;
    energies [1] = 50;
    energies [30] = 30.0;
    
    // test configuration linear constrained positiv
    IgSoCircularHist* small = new IgSoCircularHist;
    small->minRadius = 1.806;
    small->maxRadius = -1;
    small->numberOfBins = bins;
    small->energies.setValues (0, bins, &energies [0]);
    small->logScale = 0;
    small->layer = 10.0;
    small->scaleFactor = 0.1;
    small->showAnnotations = true;

    SoMaterial* mat = new SoMaterial;
    mat->diffuseColor.setValue (0.0, 0.0, 1.0);

    IgSoCircularHist* neg  = new IgSoCircularHist;
    neg->minRadius = 1.806;
    neg->maxRadius = 1.5;
    neg->numberOfBins = bins;
    neg->energies.setValues (0, bins, &energies [0]);
    neg->logScale = 0;
    neg->layer = 10.0;
    neg->scaleFactor = 0.1;
    neg->showAnnotations = 0;

    SoMaterial *mat2 = new SoMaterial;
    mat2->diffuseColor.setValue (1.0, 1.0, 0.0); 
    
    root->addChild (mat);
    root->addChild (small);
    root->addChild (mat2);
    root->addChild (neg);

    return "IGUANA: IgSoCircularHist Test";
}
