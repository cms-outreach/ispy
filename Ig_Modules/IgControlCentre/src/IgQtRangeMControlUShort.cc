//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgControlCentre/interface/IgQtRangeMControlUShort.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgQtRangeMControlUShort::IgQtRangeMControlUShort (QWidget *parent, int parts,
                                               unsigned short value /*= 0*/,
		                               const char *name /*= 0*/)

    : inherited  (parent, parts, value, name)
{ }

IgQtRangeMControlUShort::IgQtRangeMControlUShort (QWidget *parent, int parts,
                                               unsigned int size,
		                               const unsigned short* values,
		                               const char *name /*= 0*/)
    : inherited  (parent, parts, size, values, name)
{ }

IgQtRangeMControlUShort::IgQtRangeMControlUShort (QWidget *parent, int parts,
                          const rangeMValues& values,
		          const char *name /*= 0*/)
    : inherited  (parent, parts, values, name)
{ }

void
IgQtRangeMControlUShort::emitSignals (void)
{
    emit newValue (value ());
    emit valueChanged (valueChange ());
}
