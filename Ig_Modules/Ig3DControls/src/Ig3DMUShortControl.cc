//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/Ig3DControls/interface/Ig3DMUShortControl.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

Ig3DMUShortControl::Ig3DMUShortControl (IgControlCategory *pane, QString label,
				    int parts /* = IgQtRangeMControl::ValuePart */)
    : inherited (pane,label, parts)
{ }

void
Ig3DMUShortControl::setValue (unsigned int size, const unsigned short* newvals)
{ inherited::setValue(size, newvals); }

void
Ig3DMUShortControl::setValue (const std::vector<unsigned short>& newvals)
{ inherited::setValue(newvals); }
