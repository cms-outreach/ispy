//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgControlCentre/interface/IgQtRangeMControlLong.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgQtRangeMControlLong::IgQtRangeMControlLong (QWidget *parent, int parts,
                                               long value /*= 0*/,
		                               const char *name  /*= 0*/)

    : inherited  (parent, parts, value, name)
{ }

IgQtRangeMControlLong::IgQtRangeMControlLong (QWidget *parent, int parts,
                                               unsigned int size,
		                               const long* values,
		                               const char *name  /*= 0*/)
    : inherited  (parent, parts, size, values, name)
{ }

IgQtRangeMControlLong::IgQtRangeMControlLong (QWidget *parent, int parts,
                          const rangeMValues& values,
		          const char *name  /*= 0*/)
    : inherited  (parent, parts, values, name)
{ }

void
IgQtRangeMControlLong::emitSignals (void)
{
    emit newValue (value ());
    emit valueChanged (valueChange ());
}
