//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Inventor/test/test_main.h"
#include "Iguana/Inventor/interface/IgSoTower.h"

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
    IgSoTower::initClass ();    
    
    SoMaterial *mat = new SoMaterial;

    IgSoTower *towerP = new IgSoTower;
    towerP->energy = 3;
    towerP->emFraction = 0.5;
    
    IgSoTower *towerN = new IgSoTower;
    towerN->energy = -3;
    towerN->emFraction = 0.5;
    
    root->addChild (mat);
    
    root->addChild (towerP);
    root->addChild (towerN);

    return "IGUANA: IgSoTower Test";
}
