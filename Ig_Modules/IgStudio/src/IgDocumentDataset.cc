//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgStudio/interface/IgDocumentDataset.h"
#include "Ig_Modules/IgStudio/interface/IgView.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgRepresentable.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgModel.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgBrowser.h"

#include <string>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgDocumentDataset::IgDocumentDataset (IgRepresentable *root,
				      const std::string &name)
    :m_root (root),
     m_name (name)
{    
}


IgRepresentable *
IgDocumentDataset::root (void)
{
    return m_root;    
}

IgDocumentDataset::ModelsList &
IgDocumentDataset::models (void)
{
    return m_models;    
}

IgDocumentDataset::ViewsList &
IgDocumentDataset::views (void)
{
    return m_views;    
}

IgDocumentDataset::BrowsersList &
IgDocumentDataset::browsers (void)
{
    return m_browsers;    
}
