//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/Ig3DControls/interface/Ig3DMShortControl.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

Ig3DMShortControl::Ig3DMShortControl (IgControlCategory *pane, QString label,
				    int parts /* = IgQtRangeMControl::ValuePart */)
    : inherited (pane,label, parts)
{ }

void
Ig3DMShortControl::setValue (unsigned int size, const short* newvals)
{ inherited::setValue(size, newvals); }

void
Ig3DMShortControl::setValue (const std::vector<short>& newvals)
{ inherited::setValue(newvals); }
