#ifndef IG_QT_BROWSER_IG_QT_APP_DEBUG_H
# define IG_QT_BROWSER_IG_QT_APP_DEBUG_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgQtBrowser/interface/config.h"
# include <qobject.h>
# include <qdialog.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class QListView;
class QListViewItem;
class QTimer;
class IgState;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_QT_BROWSER_API IgQtAppDebugInfo : public QDialog
{
    Q_OBJECT
public:
    IgQtAppDebugInfo (QWidget *parent = 0, bool autoRefresh = true);
    ~IgQtAppDebugInfo (void);

public slots:
    void		info (void);
    void		exportList (void);
    virtual void	update (void) = 0;

protected:
    QListView *		list (void);
    void                hideEvent (QHideEvent*);

private:
    void		start (void);

    bool		m_autoRefresh;
    QListView		*m_data;
    QTimer		*m_timer;

    // undefined semantics
    IgQtAppDebugInfo (const IgQtAppDebugInfo &);
    IgQtAppDebugInfo &operator= (const IgQtAppDebugInfo &);
};

class IG_QT_BROWSER_API IgQtAppDebugResourceInfo : public IgQtAppDebugInfo
{
public:
    IgQtAppDebugResourceInfo (QWidget *parent);

protected:
    virtual void	update (void);
};

class IG_QT_BROWSER_API IgQtAppDebugLibrariesInfo : public IgQtAppDebugInfo
{
public:
    IgQtAppDebugLibrariesInfo (QWidget *parent);

protected:
    virtual void	update (void);
};

class IG_QT_BROWSER_API IgQtAppDebugPluginsInfo : public IgQtAppDebugInfo
{
public:
    IgQtAppDebugPluginsInfo (IgState *state, QWidget *parent);

protected:
    virtual void	update (void);

private:
    IgState		*m_state;
};

class IG_QT_BROWSER_API IgQtAppDumpStateInfo : public IgQtAppDebugInfo
{
public:
    IgQtAppDumpStateInfo (IgState *state, QWidget *parent);

protected:
    virtual void	update (void);
    virtual void 	populate (IgState* state, QListViewItem *item);
    
private:
    IgState		*m_state;
};

class IG_QT_BROWSER_API IgQtAppDebug : public QObject
{
    Q_OBJECT
public:
    IgQtAppDebug (IgState *state, QWidget *parent = 0);
    ~IgQtAppDebug (void);

public slots:
    void		resources (void);
    void		libraries (void);
    void		plugins (void);
    void		debugme (void);
    void		dumpState (void);
    void		rundebug (void);
    void		showProfiler (void);

private:
    static bool		m_profiler;

    IgState		*m_state;
    QWidget		*m_parent;
    IgQtAppDebugInfo	*m_info [4];

    // undefined semantics
    IgQtAppDebug (const IgQtAppDebug &);
    IgQtAppDebug &operator= (const IgQtAppDebug &);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_QT_BROWSER_IG_QT_APP_DEBUG_H







