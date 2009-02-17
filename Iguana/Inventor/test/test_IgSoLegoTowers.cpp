//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Inventor/interface/IgSoLegoTowers.h"
#include "Iguana/Inventor/interface/IgSoTowerRadii.h"
#include "Iguana/Inventor/interface/IgSoTower.h"
#include "test_main.h"

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
    IgSoLegoTowers::initClass ();
    IgSoTowerRadii::initClass ();
    IgSoTower::initClass ();

    SoMaterial *pmat = new SoMaterial;
    pmat->diffuseColor.setValue (1.0, 1.0, 1.0);

    IgSoLegoTowers *plot = new IgSoLegoTowers;
    plot->heightScale = 20;

    IgSoTowerRadii *circ = new IgSoTowerRadii;
    circ->displayCenter = true;
    circ->center = SbVec2f (1, 1);
    circ->radius = 0.75;

    SoMaterial *mat = new SoMaterial;
    mat->diffuseColor.setValue (0.0, 1.0, 0.0);
    
    IgSoTower *tower = new IgSoTower;
    tower->position = SbVec2f (1, 1); // eta, phi
    tower->scaleFactor = 20;
    tower->energy = 20.0;
    tower->emFraction = 0.1; // EM/Total 10%

    IgSoTower *tower1 = new IgSoTower;
    tower1->position = SbVec2f (3, 3); // eta, phi
    tower1->scaleFactor = 20;
    tower1->energy = 20.0;
    tower1->emFraction = 1.0; // EM/Total 100%

    IgSoTower *negTower = new IgSoTower;
    negTower->position = SbVec2f (2, 2); // eta, phi
    negTower->scaleFactor = 20;
    negTower->emFraction = 0.0; // EM/Total 0%
    negTower->energy = -20.0;

    root->addChild (pmat);
    root->addChild (plot);
    root->addChild (mat);
    root->addChild (circ);
    
    root->addChild (tower);
    root->addChild (tower1);
    root->addChild (negTower);

    return "IGUANA: IgSoLegoTowers Test";
}
