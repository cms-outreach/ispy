//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Framework/interface/IgDocumentDataRoot.h"
#include "classlib/utils/DebugAids.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>

std::string IgDocumentDataRoot::m_currentRoot = "Untitled Document Data";

//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

const char*
IgDocumentDataRoot::getCurrentRoot (void)
{ return m_currentRoot.c_str (); }

void
IgDocumentDataRoot::currentRoot (const char* root)
{ ASSERT (root); m_currentRoot = root; }
