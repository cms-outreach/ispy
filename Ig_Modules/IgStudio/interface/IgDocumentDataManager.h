#ifndef IG_STUDIO_IG_DOCUMENT_DATA_MANAGER_H
# define IG_STUDIO_IG_DOCUMENT_DATA_MANAGER_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgStudio/interface/config.h"
# include "Ig_Framework/IgObjectBrowser/interface/IgStateElement.h"
# include <map>
# include <vector>
# include <string>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_STUDIO_API IgDocumentDataManager : public IgStateElement
{
    IG_DECLARE_STATE_ELEMENT (IgDocumentDataManager);
public:
    IgDocumentDataManager (IgState *m_state);
    ~IgDocumentDataManager (void);
    
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    typedef std::map<std::string, IgState*> DocDataMap;
    
    IgState*			state (void);
    IgState*			lookup (IgState* state = 0,
					std::string root = "");
    std::vector<std::string> 	roots (void);

private:
    IgState	*m_state;
    DocDataMap	m_docData;    
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_STUDIO_IG_DOCUMENT_DATA_MANAGER_H
