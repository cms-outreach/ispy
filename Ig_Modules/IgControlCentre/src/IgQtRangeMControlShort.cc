//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgControlCentre/interface/IgQtRangeMControlShort.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgQtRangeMControlShort::IgQtRangeMControlShort (QWidget *parent, int parts,
                                               short value /*= 0*/,
		                               const char *name  /*= 0*/)

    : inherited  (parent, parts, value, name)
{ }

IgQtRangeMControlShort::IgQtRangeMControlShort (QWidget *parent, int parts,
                                               unsigned int size,
		                               const short* values,
		                               const char *name  /*= 0*/)
    : inherited  (parent, parts, size, values, name)
{ }

IgQtRangeMControlShort::IgQtRangeMControlShort (QWidget *parent, int parts,
                          const rangeMValues& values,
		          const char *name  /*= 0*/)
    : inherited  (parent, parts, values, name)
{ }

void
IgQtRangeMControlShort::emitSignals (void)
{
    emit newValue (value ());
    emit valueChanged (valueChange ());
}
