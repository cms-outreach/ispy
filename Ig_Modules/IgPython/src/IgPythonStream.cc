//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgPython/interface/IgPythonStream.h"
#include "Ig_Modules/IgPython/interface/IgPythonInterpreter.h"
#include <ostream>
#include <streambuf>
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
using namespace std;

IgPythonStreambuf::IgPythonStreambuf (void)
    :m_pythonInterpreter(new IgPythonInterpreter())
{
}

int IgPythonStreambuf::sync(void)
{
    m_pythonInterpreter->interpret(str().c_str());
    str(std::string(""));
    
    return 0;
}

void IgPythonStreambuf::redirectError(std::ostream &stream)
{
    m_pythonInterpreter->redirectError(stream);
}

void IgPythonStreambuf::redirectOutput(std::ostream &stream)
{
    m_pythonInterpreter->redirectOutput(stream);
}

std::string IgPythonStreambuf::completeName(const char *objName)
{
    return m_pythonInterpreter->completeName(objName);    
}

IgPythonStream::IgPythonStream(void)
    :std::ostream(&m_streambuf),m_currentObjectName("")
{
}

IgPythonStream &IgPythonStream::operator[](const char *objectName)
{
    m_currentObjectName = objectName;
    return *this;
}

void IgPythonStream::redirectError(std::ostream &stream)
{
    m_streambuf.redirectError(stream);
}

void IgPythonStream::redirectOutput(std::ostream &stream)
{
    m_streambuf.redirectOutput(stream);
}

std::string IgPythonStream::completeName(const char *objName)
{
    return m_streambuf.completeName(objName);    
}

