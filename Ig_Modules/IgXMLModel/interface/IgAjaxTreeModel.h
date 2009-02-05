#ifndef IG_XML_MODEL_IG_AJAX_TREE_MODEL_H
# define IG_XML_MODEL_IG_AJAX_TREE_MODEL_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgXMLModel/interface/config.h"
# include "Ig_Modules/IgXMLModel/interface/IgXMLModel.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
class IgUIDModel;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_XML_MODEL_API IgAjaxTreeModel : public IgXMLModel
{
public:
    IgAjaxTreeModel (IgUIDModel *uidModel);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    IgUIDModel * uidModel (void);
private:
    IgUIDModel * 	m_uidModel;    
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_XML_MODEL_IG_AJAX_TREE_MODEL_H
