#ifndef IG_STUDIO_IG_DOCUMENT_H
# define IG_STUDIO_IG_DOCUMENT_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgStudio/interface/config.h"
# include <string>
# include <list>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgStudioDriver;
class IgDocumentData;
class IgState;
class IgQtSite;
class IgPage;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_STUDIO_API IgDocument
{
public:
    IgDocument (IgStudioDriver *parent);
    virtual ~IgDocument (void);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    virtual IgStudioDriver *	owner (void) const;
    virtual IgState *		state (void) const;
    virtual IgDocumentData *	data (void) const;

    // FIXME: Windows, sites?
    virtual IgQtSite *		window (void) const;

    // FIXME: file name? document file handling
    virtual void		setName (const std::string &name);
    virtual void		addContentProxy (const std::string &name);
    virtual void		load (const std::string &name,
				      bool prefix = false);
    void			addPage (IgPage *page);
    IgPage			*currentPage (void);
private:
    IgStudioDriver		*m_owner;
    IgState			*m_state;
    IgQtSite			*m_window;
    IgDocumentData		*m_data;

    typedef std::list <IgPage *> PageList;
    PageList			m_pageMap;
    IgPage			*m_currentPage;    

    // undefined, no semantics
    IgDocument (const IgDocument &);
    IgDocument &operator= (const IgDocument &);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_STUDIO_IG_DOCUMENT_H
