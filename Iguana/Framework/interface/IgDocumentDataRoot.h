#ifndef IGUANA_FRAMEWORK_IG_DOCUMENT_DATA_ROOT_H
# define IGUANA_FRAMEWORK_IG_DOCUMENT_DATA_ROOT_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Framework/interface/config.h"
# include <string>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IGUANA_FRAMEWORK_API IgDocumentDataRoot
{
public:
    // implicit constructor
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    static void			currentRoot (const char* root);
    static const char* 		getCurrentRoot (void);
        
private:
    static std::string m_currentRoot;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_FRAMEWORK_IG_DOCUMENT_DATA_ROOT_H
