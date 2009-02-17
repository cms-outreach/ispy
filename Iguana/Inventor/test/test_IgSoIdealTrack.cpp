//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Inventor/interface/IgSoIdealTrack.h"
#include "Iguana/Inventor/interface/IgParticleChar.h"
#include <Inventor/engines/SoTimeCounter.h>
#include <Inventor/nodes/SoMaterial.h>
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
    IgSoIdealTrack::initClass ();

    SoTimeCounter *counter = new SoTimeCounter;
    counter->max =  1000;
    counter->min = -1000;
    counter->frequency = 0.5;

    IgSoIdealTrack *obj = new IgSoIdealTrack;
    obj->initialise (0.0, 0.0, 0.0,
		     0.0, 0.5, 0.2,
		     1.0,
		     IgParticleChar::getByName ("electron"));
    obj->dt = 1000;
    obj->dt.connectFrom (&counter->output);
    root->addChild (obj);

    return "IGUANA: IgSoIdealTrack Test";
}
