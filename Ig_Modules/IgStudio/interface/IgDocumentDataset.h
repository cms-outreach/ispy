#ifndef IG_STUDIO_IG_DOCUMENT_DATASET_H
# define IG_STUDIO_IG_DOCUMENT_DATASET_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgStudio/interface/config.h"
# include <list>
# include <string>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
class IgRepresentable;
class IgBrowser;
class IgView;
class IgModel;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_STUDIO_API IgDocumentDataset
{
public:
    IgDocumentDataset (IgRepresentable *root, const std::string &name);

    typedef std::list<IgModel *> ModelsList;    
    typedef std::list<IgBrowser *> BrowsersList;
    typedef std::list<IgView *> ViewsList;
    
    IgRepresentable *root (void);
    ModelsList &models (void);
    ViewsList &views (void);
    BrowsersList &browsers (void);
private:
    IgRepresentable 	*m_root;
    std::string		m_name;    
    ModelsList 		m_models;    
    BrowsersList 	m_browsers;
    ViewsList 		m_views;
    
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_STUDIO_IG_DOCUMENT_DATASET_H
