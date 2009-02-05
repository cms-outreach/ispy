//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/Ig3DControls/interface/Ig3DUShortControl.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

Ig3DUShortControl::Ig3DUShortControl (IgControlCategory *pane, QString label,
				    int parts /* = IgQtRangeControl::ValuePart */)
    : inherited (pane,label, parts)
{ }

void
Ig3DUShortControl::setValue (unsigned short newval)
{ inherited::setValue(newval); }
