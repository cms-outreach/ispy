//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/Ig3DControls/interface/Ig3DMUInt32Control.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

Ig3DMUInt32Control::Ig3DMUInt32Control (IgControlCategory *pane, QString label,
				    int parts /* = IgQtRangeMControl::ValuePart */)
    : inherited (pane,label, parts)
{ }

void
Ig3DMUInt32Control::setValue (unsigned int size, const unsigned int* newvals)
{ inherited::setValue(size, newvals); }

void
Ig3DMUInt32Control::setValue (const std::vector<unsigned int>& newvals)
{ inherited::setValue(newvals); }
