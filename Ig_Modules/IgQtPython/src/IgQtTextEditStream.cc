//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgQtPython/interface/IgQtTextEditStream.h"
#include "Ig_Modules/IgQtPython/interface/IgQtTextEditStreambuf.h"
//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgQtTextEditStream::IgQtTextEditStream (QWidget * parent, const int subStyle, const bool waitToFlush)
    :QTextEdit(parent),
     std::ostream(&m_streamBuf),
     m_parent(parent),
     m_streamBuf(this,subStyle,waitToFlush),
     m_waitToFlush(waitToFlush)
{
}

void IgQtTextEditStream::realFlush(void)
{
    m_streamBuf.realSync();
}

