//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Inventor/interface/IgSo3DErrorBar.h"
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoIndexedLineSet.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

SO_KIT_SOURCE (IgSo3DErrorBar);

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgSo3DErrorBar::IgSo3DErrorBar (void)
{
    SO_KIT_CONSTRUCTOR (IgSo3DErrorBar);

    SO_KIT_ADD_FIELD (lineWidth, (1.0));
    SO_KIT_ADD_FIELD (u1,	 (0.5,  0.0,  0.0));
    SO_KIT_ADD_FIELD (u2,	(-0.5,  0.0,  0.0));
    SO_KIT_ADD_FIELD (v1,	 (0.0,  0.5,  0.0));
    SO_KIT_ADD_FIELD (v2,	 (0.0, -0.5,  0.0));
    SO_KIT_ADD_FIELD (w1,	 (0.0,  0.0,  0.5));
    SO_KIT_ADD_FIELD (w2,	 (0.0,  0.0, -0.5));

    SO_KIT_ADD_CATALOG_ENTRY (drawStyle, SoDrawStyle, FALSE, separator,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY (lineCoords, SoCoordinate3, FALSE, separator,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY (lineSet, SoIndexedLineSet, FALSE, separator,\x0, TRUE);

    SO_KIT_INIT_INSTANCE ();

    setUpConnections (true, true);
}

void
IgSo3DErrorBar::initClass (void)
{ SO_KIT_INIT_CLASS (IgSo3DErrorBar, IgSoShapeKit, "IgSoShapeKit"); }

void
IgSo3DErrorBar::refresh (void)
{
    SoDrawStyle		*drawStyle  = new SoDrawStyle;
    SoCoordinate3	*lineCoords = new SoCoordinate3;
    SoIndexedLineSet	*lineSet    = new SoIndexedLineSet;

    SbVec3f		lineVertices [6] = {
	u1.getValue (),	u2.getValue (),
	v1.getValue (), v2.getValue (),
	w1.getValue (), w2.getValue ()
    };

    static const int	linesIndices [3*3] = {
	0, 1, SO_END_LINE_INDEX,
	2, 3, SO_END_LINE_INDEX,
	4, 5, SO_END_LINE_INDEX
    };

    lineCoords->point.setValues (0, 6, lineVertices); 
    lineSet->coordIndex.setValues (0, 9, linesIndices);

    drawStyle->lineWidth   = lineWidth.getValue ();
    drawStyle->linePattern = 0xffff;    // 0xffff = solid

    setPart ("drawStyle", drawStyle);
    setPart ("lineCoords", lineCoords);
    setPart ("lineSet", lineSet);
}
