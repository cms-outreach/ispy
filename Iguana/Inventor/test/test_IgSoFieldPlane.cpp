//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Inventor/interface/IgSoFieldPlane.h"
#include "Iguana/Inventor/interface/IgSoFieldPlaneMap.h"
#include "Iguana/Inventor/interface/IgSoPlaneManip.h"
#include "Iguana/Inventor/interface/IgSbField.h"
#include "Iguana/Inventor/interface/IgSbColorMap.h"
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

static const double WORLD_RADIUS = 10.;
static const double FIELD_SIZE	 = 3.;
static const double FIELD_MAX	 = 3;

class TestField : public IgSbField
{
public:
    virtual void evaluate (const double pt [3], double field []) const {
	// sqrt in distance from origin
	double d = sqrt (pt[0]*pt[0] + pt[1]*pt[1] + pt[2]*pt[2]);
	if (d > FIELD_SIZE)
	    field [0] = field [1] = 0;
	else
	    field [0] = field [1] = FIELD_MAX * sqrt (1 - d / FIELD_SIZE);

	field [2] = 0;
    }
};


QString makeTest (SoSeparator *root)
{
    IgSoFieldPlane::initClass ();
    IgSoFieldPlaneMap::initClass ();
    IgSoPlaneManip::initClass ();

    SbBox3f world (-WORLD_RADIUS, -WORLD_RADIUS, -WORLD_RADIUS,
		   WORLD_RADIUS,   WORLD_RADIUS,  WORLD_RADIUS);
    IgSoFieldPlane *plane = new IgSoFieldPlane;
    plane->colorMap (&IgSbColorMap::jet);
    plane->field (new TestField, world);
    plane->show = true;

    root->addChild (plane);

    return "IGUANA: IgSoFieldPlane Test";
}
