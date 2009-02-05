//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/Ig3DControls/interface/Ig3DCameraRotControl.h"
#include "Ig_Modules/IgControlCentre/interface/IgQtRangeControlFloat.h"
#include <qwhatsthis.h>
#include <classlib/utils/DebugAids.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

Ig3DCameraRotControl::Ig3DCameraRotControl (IgControlCategory *pane,
					    QString label)
    : Ig3DRotationControl (pane, label)
{
    QWhatsThis::add (xControl ()->widget (), 
                   "Rotate camera around X-axis");
    QWhatsThis::add (yControl ()->widget (), 
                   "Rotate camera around Y-axis");
    QWhatsThis::add (zControl ()->widget (), 
                   "Rotate camera around Z-axis");
}

void
Ig3DCameraRotControl::applyValue (void)
{
    SbVec3f fp = getFocalPoint ();
    Ig3DRotationControl::applyValue ();
    setFocalPoint (fp);
}
