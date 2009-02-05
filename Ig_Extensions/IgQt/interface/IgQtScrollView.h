#ifndef IG_QT_IG_QT_SCROLL_VIEW_H
# define IG_QT_IG_QT_SCROLL_VIEW_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Extensions/IgQt/interface/config.h"
# include <qscrollview.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_QT_API IgQtScrollView : public QScrollView
{
    Q_OBJECT
public:
    IgQtScrollView (QWidget * parent = 0, const char * name = 0, WFlags f = 0);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

protected:
    void		resizeEvent (QResizeEvent *e);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_QT_IG_QT_SCROLL_VIEW_H
