//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgTemplateModel/interface/IgTemplateItem.h"
#include "Ig_Modules/IgTemplateModel/interface/IgTemplateEngine.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgTemplateItem::IgTemplateItem (IgTemplateEngine *engine)
    : m_engine (engine)
{
}


IgTemplateItem::~IgTemplateItem (void)
{
}

IgTemplateEngine *
IgTemplateItem::engine (void)
{
    return m_engine;    
}
