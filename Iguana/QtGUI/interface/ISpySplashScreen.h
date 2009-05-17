#ifndef QT_GUI_ISPY_SPLASH_SCREEN_H
# define QT_GUI_ISPY_SPLASH_SCREEN_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "ui_ISpySplashScreen.h"
# include <QObject>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class ISpySplashScreen : public QWidget, private Ui::ISpySplashScreen
{
    Q_OBJECT
public:
    ISpySplashScreen (QWidget *parent = 0);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    ~ISpySplashScreen (void);

signals:
    void		splashDone (void);

protected:
    virtual void	closeEvent (QCloseEvent *event);    
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // QT_GUI_ISPY_SPLASH_SCREEN_H
