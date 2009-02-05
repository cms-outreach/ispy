//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Extensions/IgOpenInventor/test/test_main.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSoTower.h"

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

    IgSoTower *tower = new IgSoTower;
    tower->energy = 3;
    tower->emFraction = 0.5;

    root->addChild (tower);

    return "IGUANA: IgSoTower Test";
}
