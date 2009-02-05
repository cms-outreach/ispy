#ifndef IG_GEANT_4_SETUP_IG_WIZARD_PAGE_H
# define IG_GEANT_4_SETUP_IG_WIZARD_PAGE_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgStudio/interface/config.h"
# include <qvbox.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgWizard;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_STUDIO_API IgWizardPage : public QVBox
{
public:
    IgWizardPage (IgWizard *owner, const char *name = 0, WFlags f = 0);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    virtual void	enter (void);
    virtual void	leave (void);
    virtual void	commit (void);

    virtual void	help (void);

    virtual QString	title (void) const;
    virtual bool	isAppropriate (void) const;
    virtual bool	isBackAvailable (void) const;
    virtual bool	isNextAvailable (void) const;
    virtual bool	isFinishAvailable (void) const;
    virtual bool	isHelpAvailable (void) const;

protected:
    IgWizard		*m_wizard;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_GEANT_4_SETUP_IG_WIZARD_PAGE_H
