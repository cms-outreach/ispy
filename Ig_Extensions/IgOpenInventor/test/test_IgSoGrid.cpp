//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Extensions/IgOpenInventor/interface/IgSoGrid.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSoCoordinateAxis.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSoAxis.h"
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
    IgSoGrid::initClass ();
    IgSoGrid *obj = new IgSoGrid;
    obj->maxX = 4;
    obj->maxZ = 6;
    
    root->addChild (obj);

    return "IGUANA: IgSoGrid Test";
}
