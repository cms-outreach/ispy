#include "Iguana/QtGUI/interface/ISpySplashScreen.h"
#include <QtGui>

ISpySplashScreen::ISpySplashScreen (QWidget *parent)
    : QWidget (parent)
{
    setupUi (this);
}

ISpySplashScreen::~ISpySplashScreen (void)
{
    if (! isHidden ())
    {
	hide ();
	splashDone ();
    }
}

void
ISpySplashScreen::closeEvent (QCloseEvent *event)
{
    hide ();
    splashDone ();
    event->accept();
}
