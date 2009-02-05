//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/Ig3DControls/interface/Ig3DMFloatControl.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

Ig3DMFloatControl::Ig3DMFloatControl (IgControlCategory *pane, QString label,
				    int parts /* = IgQtRangeMControl::ValuePart */)
    : inherited (pane,label, parts)
{ }

void
Ig3DMFloatControl::setValue (unsigned int size, const float* newvals)
{ inherited::setValue(size, newvals); }

void
Ig3DMFloatControl::setValue (const std::vector<float>& newvals)
{ inherited::setValue(newvals); }
