//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Inventor/interface/IgSoRotSolid.h"
#include "test_main.h"
#include <vector>
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
    IgSoRotSolid::initClass ();
    
    std::vector<float> zvals(4);
    std::vector<float> rmin(4);
    std::vector<float> rmax(4);

    zvals[0] = -1; zvals[1] = -.8; zvals[2] = .8; zvals[3] = 1;
    rmin[0] = .2; rmin[1] = 1.; rmin[2] = 1.; rmin[3] = .2;
    rmax[0] = .2; rmax[1] = .2; rmax[2] = .2; rmax[3] = .2;
    
    IgSoRotSolid *obj = new IgSoRotSolid;
    obj->makePcon (zvals, rmin, rmax);
    
    root->addChild (obj);

    return "IGUANA: IgSoPcon Test";
}
