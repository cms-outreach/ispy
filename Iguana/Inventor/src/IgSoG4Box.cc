//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Inventor/interface/IgSoG4Box.h"
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoScale.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

SO_KIT_SOURCE (IgSoG4Box);

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

void
IgSoG4Box::initClass (void)
{ SO_KIT_INIT_CLASS (IgSoG4Box, IgSoShapeKit, "IgSoShapeKit"); }

IgSoG4Box::IgSoG4Box (void)
{
    SO_KIT_CONSTRUCTOR (IgSoG4Box);
    SO_KIT_ADD_FIELD (dx, (1.0));
    SO_KIT_ADD_FIELD (dy, (1.0));
    SO_KIT_ADD_FIELD (dz, (1.0));
    SO_KIT_ADD_CATALOG_ENTRY (scale, SoScale, FALSE, separator,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY (cube, SoCube, FALSE, separator,\x0, TRUE);
    SO_KIT_INIT_INSTANCE ();
    setUpConnections (true, true);
}

void
IgSoG4Box::refresh (void)
{
    SoScale *scale = new SoScale;
    SoCube  *cube = new SoCube;

    scale->scaleFactor.setValue (dx.getValue (), dy.getValue (), dz.getValue ());

    setPart ("scale", scale);
    setPart ("cube", cube);
}
