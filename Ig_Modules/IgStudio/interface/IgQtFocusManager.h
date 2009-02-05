#ifndef IG_STUDIO_IG_QT_FOCUSMANAGER_H
# define IG_STUDIO_IG_QT_FOCUSMANAGER_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgStudio/interface/config.h"
# include <qobject.h>
# include <map>
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
class QWidget;
class IgState;
class IgView;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

// FIXME: maybe we should just have IgPage derive from QObject?

class IG_STUDIO_API IgQtFocusManager : public QObject
{
    Q_OBJECT
public:
    IgQtFocusManager (IgState *state, std::map<QWidget *, IgView *> *views);

public slots:
void windowActivated (QWidget *widget);
    
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
private:
    IgState 				*m_state;
    std::map<QWidget *, IgView *> 	*m_views;    
    IgView				*m_oldView;    
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_STUDIO_IG_QT_FOCUSMANAGER_H
