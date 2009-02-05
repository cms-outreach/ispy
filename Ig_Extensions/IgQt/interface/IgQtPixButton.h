#ifndef IG_QT_IG_QT_PIX_BUTTON_H
# define IG_QT_IG_QT_PIX_BUTTON_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Extensions/IgQt/interface/config.h"
# include <qtoolbutton.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_QT_API IgQtPixButton : public QToolButton
{
    Q_OBJECT
public:
    IgQtPixButton (QWidget *parent, const char *name = 0);
    IgQtPixButton (QPixmap pixmap, QWidget *parent, const char *name = 0);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

protected slots:
    void	showWhatsThisHelp (void);
    
protected:
    void	mousePressEvent ( QMouseEvent * event);
    void	initialise (QPixmap pixmap);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_QT_IG_QT_PIX_BUTTON_H
