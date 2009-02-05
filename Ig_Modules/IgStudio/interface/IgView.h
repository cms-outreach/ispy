#ifndef IG_STUDIO_IG_VIEW_H
# define IG_STUDIO_IG_VIEW_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgStudio/interface/config.h"
# include <string>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgPage;
class IgState;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_STUDIO_API IgView
{
public:
    IgView (IgPage *page);
    virtual ~IgView (void);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    
    virtual IgState	*state (void);    
    IgPage		*page (void);    

    std::string 	title (void) const;
    void	 	setTitle (const char *name);
    
private:
    IgPage		*m_page;
    std::string		m_title;    
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_STUDIO_IG_VIEW_H
