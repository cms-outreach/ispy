//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Extensions/IgOpenInventor/interface/IgSoXYZGrid.h"
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
    IgSoAxis::initClass ();
    IgSoCoordinateAxis::initClass ();
    IgSoXYZGrid::initClass ();
    IgSoXYZGrid *obj = new IgSoXYZGrid;
    root->addChild (obj);

    return "IGUANA: IgSoXYZGrid Test";
}
