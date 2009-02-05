#ifndef VIEW_IVIEW_QWINDOW_SERVICE_H
# define VIEW_IVIEW_QWINDOW_SERVICE_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Framework/interface/config.h"
# include "Iguana/Framework/interface/IgStateElement.h"

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
    IViewQWindowService (IgState *state, QWidget *mainWindow);
    ~IViewQWindowService (void);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    virtual QWidget *		mainWindow (void);

private:
    IgState			*m_state;
    QWidget			*m_mainWindow;

    // undefined semantics
    IViewQWindowService (const IViewQWindowService &);
    IViewQWindowService &operator= (const IViewQWindowService &);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIEW_IVIEW_QWINDOW_SERVICE_H
