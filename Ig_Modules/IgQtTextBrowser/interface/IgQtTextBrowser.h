#ifndef IG_QT_TEXT_BROWSER_IG_QT_TEXT_BROWSER_H
# define IG_QT_TEXT_BROWSER_IG_QT_TEXT_BROWSER_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgQtTextBrowser/interface/config.h"
# include "Ig_Framework/IgObjectBrowser/interface/IgBrowser.h"
# include <qtextbrowser.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgState;
class IgTextRep;
class IgSimpleTextModel;
class IgTextModelEvent;
class IgSite;
class IgSelectionMessage;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_QT_TEXT_BROWSER_API IgQtTextBrowser : public QTextBrowser,
					       public IgBrowser
{
public:
    IgQtTextBrowser (IgState *state, IgSite *site);
    ~IgQtTextBrowser (void);

    virtual void	browse (IgRepresentable *object);

    static const char *	catalogLabel (void);

protected:
    virtual void	textChanged (IgTextModelEvent event);
    virtual void	selectMessage (IgSelectionMessage message);
    virtual void	select (IgTextRep *rep);

private:
    IgState		*m_state;
    IgSite		*m_site;
    IgSimpleTextModel	*m_model;

    // undefined semantics
    IgQtTextBrowser (const IgQtTextBrowser &);
    IgQtTextBrowser &operator= (const IgQtTextBrowser &);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_QT_TEXT_BROWSER_IG_QT_TEXT_BROWSER_H
