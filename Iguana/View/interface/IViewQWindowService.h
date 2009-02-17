#ifndef VIEW_IVIEW_QWINDOW_SERVICE_H
# define VIEW_IVIEW_QWINDOW_SERVICE_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Framework/interface/config.h"
# include "Iguana/Framework/interface/IgStateElement.h"
# include <Quarter/Quarter.h>
# include <Quarter/QuarterWidget.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class QWidget;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IGUANA_FRAMEWORK_API IViewQWindowService : public IgStateElement
{
    IG_DECLARE_STATE_ELEMENT (IViewQWindowService);
public:
    IViewQWindowService (IgState *state, QWidget *mainWindow, SIM::Coin3D::Quarter::QuarterWidget *viewer, SoRenderManager::Superimposition * super);
    ~IViewQWindowService (void);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    virtual QWidget *		mainWindow (void);
    virtual SIM::Coin3D::Quarter::QuarterWidget * viewer (void);
    virtual SoRenderManager::Superimposition * super (void);
    virtual void super (SoRenderManager::Superimposition *);

private:
    IgState			*m_state;
    QWidget			*m_mainWindow;
    SIM::Coin3D::Quarter::QuarterWidget *m_viewer;
    SoRenderManager::Superimposition 	*m_super;
    
    // undefined semantics
    IViewQWindowService (const IViewQWindowService &);
    IViewQWindowService &operator= (const IViewQWindowService &);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIEW_IVIEW_QWINDOW_SERVICE_H
