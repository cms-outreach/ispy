//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgXMLModel/interface/IgAjaxTreeModel.h"
#include "Ig_Modules/IgXMLModel/interface/IgUIDModel.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgAjaxTreeModel::IgAjaxTreeModel (IgUIDModel *uidModel)
    : IgXMLModel ("", ""),
      m_uidModel (uidModel)
{
}

IgUIDModel *
IgAjaxTreeModel::uidModel (void)
{
    return m_uidModel;    
}
