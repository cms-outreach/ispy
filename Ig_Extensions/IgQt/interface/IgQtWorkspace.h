#ifndef IG_QT_IG_QT_WORK_SPACE_H
# define IG_QT_IG_QT_WORK_SPACE_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Extensions/IgQt/interface/config.h"
# include <qworkspace.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_QT_API IgQtWorkspace : public QWorkspace
{
    Q_OBJECT
public:
    IgQtWorkspace (QWidget *parent, const char *name = 0);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

protected:
    void 		windowActivationChange  (bool oldActive);

signals:
    void		workspaceActivated (void);
    void		workspaceDeactivated (void);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_QT_IG_QT_WORK_SPACE_H
