//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgControlCentre/interface/IgQtRangeMControlInt.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgQtRangeMControlInt::IgQtRangeMControlInt (QWidget *parent, int parts,
                                               int value /*= 0*/,
		                               const char *name  /*= 0*/)

    : inherited  (parent, parts, value, name)
{ }

IgQtRangeMControlInt::IgQtRangeMControlInt (QWidget *parent, int parts,
                                               unsigned int size,
		                               const int* values,
		                               const char *name  /*= 0*/)
    : inherited  (parent, parts, size, values, name)
{ }

IgQtRangeMControlInt::IgQtRangeMControlInt (QWidget *parent, int parts,
                          const rangeMValues& values,
		          const char *name  /*= 0*/)
    : inherited  (parent, parts, values, name)
{ }

void
IgQtRangeMControlInt::emitSignals (void)
{
    emit newValue (value ());
    emit valueChanged (valueChange ());
}
