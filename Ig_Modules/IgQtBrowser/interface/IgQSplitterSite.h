#ifndef IG_QT_BROWSER_IG_Q_SPLITTER_SITE_H
# define IG_QT_BROWSER_IG_Q_SPLITTER_SITE_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgQtBrowser/interface/IgQtSite.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgState;
class QSplitter;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_QT_BROWSER_API IgQSplitterSite : public IgQtSite
{
public:
    // FIXME: get orientation from browser configuration!
    IgQSplitterSite (IgState *state, IgSite *parent);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    static const char *	catalogLabel (void);

protected:
    IgQSplitterSite (IgSite *parent, QSplitter *widget);

    virtual QWidget *	self (void);
    virtual QWidget *	host (void);
    virtual void	host (QWidget *child);

private:
    QSplitter		*m_widget;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_QT_BROWSER_IG_Q_SPLITTER_SITE_H
