//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgControlCentre/interface/IgQtRangeMControlULong.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgQtRangeMControlULong::IgQtRangeMControlULong (QWidget *parent, int parts,
                                               unsigned long value /*= 0*/,
		                               const char *name  /*= 0*/)

    : inherited  (parent, parts, value, name)
{ }

IgQtRangeMControlULong::IgQtRangeMControlULong (QWidget *parent, int parts,
                                               unsigned int size,
		                               const unsigned long* values,
		                               const char *name  /*= 0*/)
    : inherited  (parent, parts, size, values, name)
{ }

IgQtRangeMControlULong::IgQtRangeMControlULong (QWidget *parent, int parts,
                          const rangeMValues& values,
		          const char *name  /*= 0*/)
    : inherited  (parent, parts, values, name)
{ }

void
IgQtRangeMControlULong::emitSignals (void)
{
    emit newValue (value ());
    emit valueChanged (valueChange ());
}
