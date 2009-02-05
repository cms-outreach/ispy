#ifndef IG_IV_VIEW_IG_IV_VIEW_H
# define IG_IV_VIEW_IG_IV_VIEW_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgIVView/interface/config.h"
# include "Ig_Modules/IgStudio/interface/IgView.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgPage;
class IgState;
class IgBrowser;
class IgTwig;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_IV_VIEW_API IgIVView : public IgView
{
public:
    IgIVView (IgPage *page);
    ~IgIVView ();
    
    // implicit copy constructor
    // implicit assignment operator
    void dump (unsigned level, IgTwig *twig);
    
    IgState*		state (void);

private:
    IgState			*m_state;
    IgBrowser			*m_3dBrowser;
    IgBrowser 			*m_twigBrowser;    
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_IV_VIEW_IG_IV_VIEW_H
