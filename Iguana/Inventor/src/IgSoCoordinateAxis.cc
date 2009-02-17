//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Inventor/interface/IgSoCoordinateAxis.h"
#include "Iguana/Inventor/interface/IgSoAxis.h"
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoRotation.h>
#include <Inventor/SbLinear.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

SO_KIT_SOURCE (IgSoCoordinateAxis);

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

void
IgSoCoordinateAxis::initClass ()
{ SO_KIT_INIT_CLASS (IgSoCoordinateAxis, IgSoShapeKit, "IgSoShapeKit"); }

IgSoCoordinateAxis::IgSoCoordinateAxis ()
{
    SO_KIT_CONSTRUCTOR (IgSoCoordinateAxis);
    SO_KIT_ADD_FIELD (divisions,      (3));
    SO_KIT_ADD_FIELD (divisionLength, (0.5));
    SO_KIT_ADD_CATALOG_ENTRY (x, SoSeparator, FALSE, separator,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY (xrot, SoRotation, FALSE, x,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY (xaxis, IgSoAxis, FALSE, x,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY (y, SoSeparator, FALSE, separator,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY (yrot, SoRotation, FALSE, y,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY (yaxis, IgSoAxis, FALSE, y,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY (z, SoSeparator, FALSE, separator,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY (zaxis, IgSoAxis, FALSE, z,\x0, TRUE);
    SO_KIT_INIT_INSTANCE ();
    setUpConnections (true, true);
}

void
IgSoCoordinateAxis::refresh ()
{
    SoRotation	*xrot = new SoRotation;
    IgSoAxis	*xaxis = new IgSoAxis;
    SoRotation	*yrot = new SoRotation;
    IgSoAxis	*yaxis = new IgSoAxis;
    IgSoAxis	*zaxis = new IgSoAxis;
    
    xaxis->divisions = divisions;
    xaxis->divisionLength = divisionLength;
    xaxis->label = "X";

    yaxis->divisions = divisions;
    yaxis->divisionLength = divisionLength;
    yaxis->label = "Y";

    zaxis->divisions = divisions;
    zaxis->divisionLength = divisionLength;
    zaxis->label = "Z";

    xrot->rotation.setValue (SbVec3f (0,1,0), M_PI/2);
    yrot->rotation.setValue (SbVec3f (1,0,0), -M_PI/2);

    setPart ("xrot",	xrot);
    setPart ("xaxis",	xaxis);
    setPart ("yrot",	yrot);
    setPart ("yaxis",	yaxis);
    setPart ("zaxis",	zaxis);
}
