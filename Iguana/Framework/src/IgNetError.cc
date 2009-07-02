//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/Framework/interface/IgNetError.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgNetError::IgNetError(const std::string &reason, lat::Error *next)
  : Error(next),
    m_reason(reason)
{
}

std::string
IgNetError::explainSelf(void) const
{ return m_reason; }

/** @reimp */
lat::Error *
IgNetError::clone(void) const
{ return new IgNetError(*this); }

/** @reimp */
void
IgNetError::rethrow(void)
{ throw *this; }
