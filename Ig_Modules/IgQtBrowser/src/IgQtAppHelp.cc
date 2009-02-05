#include "Ig_Modules/IgQtBrowser/interface/IgQtAppHelp.h"
#include "Ig_Extensions/IgIcons/interface/iguana-medium.xpm"
#include <qmessagebox.h>
#include <qwhatsthis.h>
#include <qpixmap.h>

IgQtAppHelp::IgQtAppHelp (IgState *state, QWidget *parent /* = 0 */)
    : m_state (state),
      m_parent (parent)
{
    init ();
}

IgQtAppHelp::~IgQtAppHelp (void)
{
}

static QString * translatedTextAboutIguana = 0;

void
IgQtAppHelp::init (void) 
{    
    if (!translatedTextAboutIguana) 
    {
	translatedTextAboutIguana = new QString;
        *translatedTextAboutIguana = 
            tr ("<h3>About IGUANA</h3>"
	        "<p>Version %1.</p>"
	        "<p>See <tt>http://iguana.cern.ch</tt> "
	        "for more information.</p>"
		).arg (IGUANA_VERSION);
    }
}

void
IgQtAppHelp::about (void)
{    
    QMessageBox *mb = new QMessageBox (m_parent, "About IGUANA");
    mb->setCaption (QString::fromLatin1 ("About IGUANA"));
    mb->setText (*translatedTextAboutIguana);
    mb->setIconPixmap (QPixmap (iguana_medium_xpm));
    mb->exec ();
}

void
IgQtAppHelp::aboutQt (void)
{    
    QMessageBox::aboutQt (m_parent, "IGUANA");
}

void
IgQtAppHelp::whatsThis (void)
{
    QWhatsThis::enterWhatsThisMode ();
}

void
IgQtAppHelp::buttons (void) 
{
    netscape ("http://iguana.web.cern.ch/iguana/snapshot/help/buttons.html");
}

void
IgQtAppHelp::iguanaWebSite (void) 
{
    netscape ("http://iguana.cern.ch");
}

void
IgQtAppHelp::reportBugDialog (void)
{
    netscape ("http://savannah.cern.ch/bugs/?func=addbug&group=iguana");
}

void
IgQtAppHelp::netscape (const QString & url) 
{
#ifdef _WS_WIN_

    //FIXME replace with less buggy ShellExecuteEx?

    if ((unsigned int)::ShellExecute (qApp->mainWidget ()->winId (), 
                                      NULL, url, NULL, NULL, SW_SHOW) <= 32)
    {
        QMessageBox::critical (0, "IGUANA",
                              tr ("Unable to display a web browser. "
                                  "Ensure that you have a web browser installed."),
                              tr ("&OK"));
    }
    
#else

    // on Linux: It tries to open the url in running netscape
    // or starts netscape if there is no netscape running.
    // If netscape is not installed, pops up a message box. 
    //
    // system( "netscape -rise -remote 'openURL(http://iguana.web.cern.ch/iguana/)'"
    //         " || netscape 'http://iguana.web.cern.ch/iguana/'");
    
    QString aCommand ("mozilla -rise -remote 'openURL(");
    aCommand += url;
    aCommand += ")' || mozilla '";
    aCommand += url;
    aCommand += "' &";
    if (system (aCommand) != 0)
    {
        QMessageBox::critical (0, "IGUANA",
                              tr ("Unable to display a mozilla browser. "
                                  "You need to have mozilla installed and in the path."),
                              tr ("&OK"));
    }

#endif
}
