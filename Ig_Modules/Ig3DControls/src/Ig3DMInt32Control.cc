//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/Ig3DControls/interface/Ig3DMInt32Control.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

Ig3DMInt32Control::Ig3DMInt32Control (IgControlCategory *pane, QString label,
				    int parts /* = IgQtRangeMControl::ValuePart */)
    : inherited (pane,label, parts)
{ }

void
Ig3DMInt32Control::setValue (unsigned int size, const int* newvals)
{ inherited::setValue(size, newvals); }

void
Ig3DMInt32Control::setValue (const std::vector<int>& newvals)
{ inherited::setValue(newvals); }
