//<<<<<< INCLUDES                                                       >>>>>>

#include "IgPluginParserError.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgPluginParserError::IgPluginParserError (const std::string &reason)
    : m_reason (reason)
{}

std::string
IgPluginParserError::explainSelf (void) const
{ return m_reason; }

/** @reimp */
lat::Error *
IgPluginParserError::clone (void) const
{ return new IgPluginParserError (*this); }

/** @reimp */
void
IgPluginParserError::rethrow (void)
{ throw *this; }
