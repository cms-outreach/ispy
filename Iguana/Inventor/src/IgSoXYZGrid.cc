//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Inventor/interface/IgSoXYZGrid.h"
#include "Iguana/Inventor/interface/IgSoCoordinateAxis.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

SO_KIT_SOURCE (IgSoXYZGrid);

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

void
IgSoXYZGrid::initClass ()
{ SO_KIT_INIT_CLASS (IgSoXYZGrid, IgSoShapeKit, "IgSoShapeKit"); }

IgSoXYZGrid::IgSoXYZGrid ()
{
    SO_KIT_CONSTRUCTOR (IgSoXYZGrid);
    SO_KIT_ADD_FIELD (divisions,      (3));
    SO_KIT_ADD_FIELD (divisionLength, (0.5));
    SO_KIT_ADD_CATALOG_ENTRY (axes, IgSoCoordinateAxis, FALSE, separator,\x0, TRUE);
    SO_KIT_INIT_INSTANCE ();
    setUpConnections (true, true);
}

void
IgSoXYZGrid::refresh ()
{
    IgSoCoordinateAxis *axes = new IgSoCoordinateAxis;
    axes->divisions = divisions;
    axes->divisionLength = divisionLength;

    setPart ("axes", axes);
}
