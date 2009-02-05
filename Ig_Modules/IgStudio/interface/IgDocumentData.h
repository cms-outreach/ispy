#ifndef IG_STUDIO_IG_DOCUMENT_DATA_H
# define IG_STUDIO_IG_DOCUMENT_DATA_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgStudio/interface/config.h"
# include "Ig_Framework/IgObjectBrowser/interface/IgStateElement.h"
# include <map>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgDocument;
class IgSimpleTwig;
class IgTwig;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_STUDIO_API IgDocumentData : public IgStateElement
{
    IG_DECLARE_STATE_ELEMENT (IgDocumentData);
public:
    IgDocumentData (IgState *state, IgDocument *owner);
    ~IgDocumentData (void);
    // implicit copy constructor
    // implicit assignment operator

    IgDocument *	document (void);
    IgSimpleTwig *	root (const std::string &name = "", 
			      bool create = false);
    void		root (IgSimpleTwig *twig, 
			     const std::string &rootName = "");
    IgSimpleTwig *	add (const std::string &name, 
			     const std::string &rootName = "");
    IgTwig *		find (const std::string &name,
			      const std::string &rootName = "");

    typedef std::map<std::string, IgSimpleTwig *> TwigMap;
    TwigMap::const_iterator 	begin (void);
    TwigMap::const_iterator	end (void);
    
private:
    IgState		*m_state;
    IgDocument		*m_document;
    TwigMap m_rootMap;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_STUDIO_IG_DOCUMENT_DATA_H
