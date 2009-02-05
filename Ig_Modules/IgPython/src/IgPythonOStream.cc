//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgPython/src/IgPythonOStream.h"
#include <iostream>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgPythonOStreamOutput::IgPythonOStreamOutput (void)
    :m_ostream(&std::cout)
{
}


IgPythonOStreamOutput::IgPythonOStreamOutput (std::ostream &stream)
    :m_ostream(&stream)
{
}

void IgPythonOStreamOutput::write(const char *text)
{
    *m_ostream << text;
    m_ostream->flush();
}

std::ostream *&IgPythonOStreamOutput::stream(void)
{
    return m_ostream;
}

IgPythonOStreamError::IgPythonOStreamError (void)
    :m_ostream(&std::cerr)
{
}


IgPythonOStreamError::IgPythonOStreamError (std::ostream &stream)
    :m_ostream(&stream)
{
}

void IgPythonOStreamError::write(const char *text)
{
    *m_ostream << text;
    m_ostream->flush();
}

std::ostream *&IgPythonOStreamError::stream(void)
{
    return m_ostream;
}

