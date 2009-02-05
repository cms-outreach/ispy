#include "Ig_Extensions/IgQt/interface/IgNetscapeRunner.h"
#include <qmessagebox.h>
#include <qapplication.h>

#ifdef _WS_WIN_
#include <comdef.h>
#include <shellapi.h>
#endif

void IgNetscapeRunner::netscape( const QString & url )
{

#ifdef _WS_WIN_

    //FIXME replace with less buggy ShellExecuteEx?

    if(( unsigned int )::ShellExecute( qApp->mainWidget()->winId(), NULL, url, NULL, NULL, SW_SHOW ) <= 32 )
    {
        QMessageBox::critical(( QWidget* ) this, "IGUANA",
			      tr( "Unable to display a web browser. "
				  "Ensure that you have a web browser installed." ),
			      tr( "&OK" ) );
    }
    
#else

    // on Linux: It tries to open the url in running netscape
    // or starts netscape if there is no netscape running.
    // If netscape is not installed, pops up a message box. 
    //
    // system( "netscape -rise -remote 'openURL(http://iguana.web.cern.ch/iguana/)'"
    //         " || netscape 'http://iguana.web.cern.ch/iguana/'");
    
    QString aCommand( "netscape -rise -remote 'openURL(" );
    aCommand += url;
    aCommand += ")' || netscape '";
    aCommand += url;
    aCommand += "' &";
    if( system( aCommand ) != 0 )
    {
        QMessageBox::critical(( QWidget* ) this, "IGUANA",
			      tr( "Unable to display a netscape browser. "
				  "You need to have netscape installed and in the path." ),
			      tr( "&OK" ) );
    }

#endif

}





