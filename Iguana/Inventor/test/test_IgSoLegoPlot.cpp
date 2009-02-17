//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Inventor/interface/IgSoLegoPlot.h"
#include <Inventor/engines/SoElapsedTime.h>
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
    IgSoLegoPlot::initClass ();

    typedef IgSbLegoHistogram::value_type ValueType;
    IgSoLegoPlot *plot = new IgSoLegoPlot;
    plot->heightScale = 20;
    IgSbLegoHistogram histo;
    for (int bin = 0; bin < 20; bin++) 
	histo.insert (ValueType (IgSbLegoBin (bin, bin),
				 IgSbLegoValue (0.6 * bin, 0.4 * bin)));
    plot->setData (histo);
    root->addChild (plot);

    return "IGUANA: IgSoLegoPlot Test";
}
