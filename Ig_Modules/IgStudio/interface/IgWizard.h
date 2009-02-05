#ifndef IG_GEANT_4_SETUP_IG_WIZARD_H
# define IG_GEANT_4_SETUP_IG_WIZARD_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgStudio/interface/config.h"
# include "Ig_Modules/IgStudio/interface/IgWizardBase.h"
# include <vector>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgWizardPage;
class QWidgetStack;
class QAccel;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_STUDIO_API IgWizard : public IgWizardBase
{
    Q_OBJECT
public:
    IgWizard (QWidget		*parent = 0,
	      const char	*name = 0,
	      bool		modal = false,
	      WFlags		f = 0);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    // title area
    virtual QString	title (void) const;
    virtual void	setTitle (QString title);

    // page manipulation
    virtual void	addPage (IgWizardPage *page);
    virtual void	insertPage (IgWizardPage *page, int index);
    virtual void	removePage (IgWizardPage *page);

    virtual void	showPage (IgWizardPage *page);
    virtual IgWizardPage *currentPage (void) const;
    virtual IgWizardPage *page (int index) const;
    virtual int		pageCount (void) const;
    virtual int		indexOf (IgWizardPage *page) const;

    // overrides
    virtual void	show (void);
    virtual bool	eventFilter (QObject *o, QEvent *e);

    // slot overrides from base class
    virtual void	back (void);
    virtual void	next (void);
    virtual void	help (void);

public slots:
    virtual void	updateButtons (void);

signals:
    virtual void	selected (IgWizardPage *page);

private:
    void		updateBack (void);
    void		updateNext (void);
    void		updateFinish (void);
    void		updateHelp (void);

    typedef std::vector<IgWizardPage *> PageList;
    QWidgetStack	*m_stack;
    PageList		m_pages;
    IgWizardPage	*m_current;
    QAccel		*m_accels;
    int			m_backAccel;
    int			m_nextAccel;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_GEANT_4_SETUP_IG_WIZARD_H
