//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgTemplateModel/interface/IgTemplateItemDB.h"
#include "Ig_Modules/IgTemplateModel/interface/IgTemplateEngine.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

IgTemplateItemDB IgTemplateItemDB::s_instance;

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgTemplateItemDB::IgTemplateItemDB (void)
    : IgPluginFactory <IgTemplateItem * (IgTemplateEngine *)> ("IGUANA Template Items")
{
}

IgTemplateItemDB *
IgTemplateItemDB::get (void)
{ return &s_instance; }
