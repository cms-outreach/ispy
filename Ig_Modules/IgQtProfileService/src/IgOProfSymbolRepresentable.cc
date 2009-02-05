//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgQtProfileService/src/IgOProfSymbolRepresentable.h"
#include <classlib/utils/DebugAids.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgOProfSymbolRepresentable::IgOProfSymbolRepresentable (std::string name,
							std::string library)
    :m_name (name),
     m_library (library),
     m_hits (0),
     m_offset (0)	
{
    ASSERT (name != "");    
}

std::string
IgOProfSymbolRepresentable::name (void)
{ return m_name; }

std::string
IgOProfSymbolRepresentable::library (void)
{ return m_library; }

int
IgOProfSymbolRepresentable::offset (void)
{ return m_offset; }

int
IgOProfSymbolRepresentable::hits (void)
{ return m_hits; }

void
IgOProfSymbolRepresentable::setOffset (int offset)
{ m_offset = offset; }

void
IgOProfSymbolRepresentable::setHits (int hits)
{ m_hits = hits; }
