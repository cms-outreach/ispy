#include "qapplication.h"
#include "qmainwindow.h"
#include "qworkspace.h"
#include "qvbox.h"
#include "qfiledialog.h"
#include "qfontdialog.h"
#include "qpopupmenu.h"
#include "qmenubar.h"
#include "qmessagebox.h"
#include "qtextstream.h"
#include "qdom.h"
#include "qtoolbox.h"
#include "qobjectlist.h"
#include "qdockarea.h"
#include "qtextstream.h"
#include <iostream>
#include <algorithm>
#include <functional>

#include <Inventor/Qt/SoQt.h>

#include "Ig_Modules/IgVis/interface/IgQtMDI.h"
#include "Ig_Modules/IgVis/interface/IgQtMDIAbsShell.h"
#include "Ig_Modules/IgVis/interface/IgQtMDIBaseShell.h"
#include "Ig_Modules/IgVis/interface/IgQtMDIAbsShellBuilder.h"
#include "Ig_Modules/IgVis/interface/IgQtMDITwigs.h"
#include "Ig_Modules/IgVis/interface/IgQtResourceMgr.h"
#include "Ig_Modules/IgVis/interface/IgQtMDIShellFactory.h"
#include "Ig_Modules/IgVis/interface/IgQtTextStream.h"

#include "Ig_Modules/IgVis/interface/stars.xpm"

// internal class
namespace
{
	class testBrowserNames 
	{
	public:
		bool operator () (IgQtMDIAbsShell* obj) 
		{
			status |= (obj->shellName() == testName);
			return status;
		}
		testBrowserNames ( const QString name ) : testName (name),
			status(false)
		{}
		operator bool ()
		{
			return status;
		}
	private:
		const QString testName;
		bool status;
	};
}

IgQtMDI::IgQtMDI(QWidget *parent, QString name) :
QMainWindow(parent,name), theWorkspace(0), theInfobox(0), theTwigbox(0){

	setIcon(QPixmap(( const char** ) stars_xpm));

	QVBox* vb = new QVBox( this );
	vb->setFrameStyle( QFrame::StyledPanel | QFrame::Sunken );
	theWorkspace = new QWorkspace(vb);
	this->setCentralWidget( vb );
	vb->resize(450,600);

	//	add dock windows
	QDockWindow *dw = new QDockWindow( QDockWindow::InDock, this, "InfoDW");
	dw->setResizeEnabled( true );
	dw->setCloseMode( QDockWindow::Always );
	addDockWindow( dw, QDockWindow::DockRight );
	theInfobox = new QToolBox( dw, "IgInfoTB" );
	dw->setCaption( tr("Info") );
	dw->setWidget( theInfobox );
	dw->setFixedExtentWidth( 100 );
	dw->show();
	setDockEnabled( dw, QDockWindow::DockLeft, true );
	setDockEnabled( dw, QDockWindow::DockRight, true );
	setDockEnabled( dw, QDockWindow::DockBottom, true );

	/* QDockWindow * */ dw = new QDockWindow( QDockWindow::InDock, this, "TwigsDW" );
	dw->setResizeEnabled( true );
	dw->setCloseMode( QDockWindow::Always );
	addDockWindow( dw, QDockWindow::DockLeft );
	theTwigbox = new QToolBox( dw, "IgTwigsTB" ); //name is used to find later
	dw->setCaption( tr("Twig Controllers") );
	dw->setWidget( theTwigbox );
	dw->setFixedExtentWidth( 100 );
	dw->show();
	setDockEnabled( dw, QDockWindow::DockLeft, true );
	setDockEnabled( dw, QDockWindow::DockRight, true );
	setDockEnabled( dw, QDockWindow::DockBottom, true );

	// setup file menu
	topFile = new QPopupMenu( this );
	CHECK_PTR( topFile );     
	//	IgQtGeneralPrefsImpl *prefs = new IgQtGeneralPrefsImpl( this );
	//	topFile->insertItem("Prefs...", prefs, SLOT(show()));
	topFile->insertItem(tr("Default Font"),this,SLOT(fontDialogSlot()));
	topFile->insertItem( "E&xit",  this, SLOT(quitSlot()));
	this->menuBar()->insertItem("&File",topFile);

	QPopupMenu *newScanMenu = new QPopupMenu( this, "New Scan" );
	newScanMenu->insertTearOffHandle();
	IgQtMDIShellFactory &f = IgQtMDIShellFactory::getInstance();
	IgQtMDIShellFactory::registry_map &rMap = f.getMap();
	for ( IgQtMDIShellFactory::registry_map::const_iterator it =  rMap.begin(); it != rMap.end(); it++)
	{
		int id = newScanMenu->insertItem( f.getDisplayName(it->first), this, SLOT(newScanSlot(int)) );
		browserMenuItems.insert( std::pair< int, IgQtMDIAbsShellBuilder* >(id, it->second ) );
	}
	this->menuBar()->insertItem("Scan",newScanMenu);

	windowsMenu = new QPopupMenu( this );
	windowsMenu->insertTearOffHandle();
	windowsMenu->setCheckable( TRUE );
	connect( windowsMenu, SIGNAL( aboutToShow() ),
		this, SLOT( windowsMenuAboutToShow() ) );
	menuBar()->insertItem( "&Windows", windowsMenu );

	menuBar()->insertSeparator();
	QPopupMenu * help = new QPopupMenu( this );
	menuBar()->insertItem( "&Help", help );

	help->insertItem( "&About", this, SLOT(about()), Key_F1);
	help->insertSeparator();
	help->insertItem( "What's &This", this, SLOT(whatsThis()), SHIFT+Key_F1);

	// restore any previous settings
	IgQtResourceMgr& theMgr = IgQtResourceMgr::getInstance();
	if ( theMgr.isValid() )
	{
		// restore font settings
		QDomDocument &theDoc = theMgr.getDoc( false );
		QDomElement session = theDoc.namedItem( "session" ).toElement();
		QDomNode fontList = session.namedItem( "font" );
		if ( !fontList.isNull() )
		{
			QApplication::setFont( theMgr.xmlToFont( fontList.toElement() ) );
		}

	}
}
QWorkspace* IgQtMDI::workspace(void) {
	return theWorkspace;
}
QToolBox* IgQtMDI::infobox(void) {
	return theInfobox;
}
QToolBox* IgQtMDI::twigbox(void) {
	return theTwigbox;
}
void IgQtMDI::quitSlot() {

	saveResources();

	//	emit 
	quit();
}
void IgQtMDI::about()
{
	QString message("IgVis \nVersion 2.0.0\n");
	message += QChar(static_cast<ushort>(0x00a9)); //copyright symbol
	message += QString("2000, 2001, 2002, 2003");
	QMessageBox::about( this, "About IgVis", message);
}
void IgQtMDI::windowsMenuAboutToShow()
{
	windowsMenu->clear();
	int cascadeId = windowsMenu->insertItem("&Cascade", theWorkspace, SLOT(cascade() ) );
	int tileId = windowsMenu->insertItem("&Tile", theWorkspace, SLOT(tile() ) );
	if ( theWorkspace->windowList().isEmpty() ) {
		windowsMenu->setItemEnabled( cascadeId, FALSE );
		windowsMenu->setItemEnabled( tileId, FALSE );
	}
	windowsMenu->insertSeparator();
	windowsMenu->insertItem( tr( "Tool&bars" ), createDockWindowMenu( AllDockWindows ) );
	windowsMenu->insertSeparator();
	QWidgetList windows = theWorkspace->windowList();
	for ( int i = 0; i < int(windows.count()); ++i ) {
		int id = windowsMenu->insertItem(windows.at(i)->caption(),
			this, SLOT( windowsMenuActivated( int ) ) );
		windowsMenu->setItemParameter( id, i );
		windowsMenu->setItemChecked( id, theWorkspace->activeWindow() == windows.at(i) );
	}
}

void IgQtMDI::windowsMenuActivated( int id )
{
	QWidget* w = theWorkspace->windowList().at( id );
	if ( w )
	{
		w->show();
		w->setFocus();
	}
}
void IgQtMDI::newScanSlot( int i)
{
	// insert a window for an event viewer 
	IgQtMDIAbsShellBuilder *b = browserMenuItems[i];
	// check that there are no maximized browsers: trying to create
	// an SoQtFullViewer will die whenever an adjustSize call is made
	QWidget *oldMax = NULL;
	QWidgetList l = theWorkspace->windowList(QWorkspace::CreationOrder);
	for (QWidget *w = l.first(); w; w = l.next())
	{
		if ( w->isMaximized() ) 
		{
			w->showNormal();
			oldMax = w;
		}
	}

	IgQtMDIAbsShell *gWindow = b->getShell( this );
	gWindow->show();
	// decided to just leave old maximized window un-maximized
	/*
	if (oldMax)
	{
	oldMax->showMaximized();
	gWindow->raise();
	}
	*/
}
void IgQtMDI::fontDialogSlot()
{
	bool ok;
	QFont f = QFontDialog::getFont( &ok, QApplication::font(), this );
	if ( ok ) {
		// the user selected a valid font
		QApplication::setFont( f, true ); //set default font and update all
	}
}
void IgQtMDI::addBrowser( IgQtMDIAbsShell* browser )
{
	// as the various base classes get initialised, we get called again with
	// the same browser
	if ( std::find(childBrowsers.begin(), childBrowsers.end(), browser ) == childBrowsers.end() )
	{
		childBrowsers.push_back( browser );
	}
	// try and restore any previously stored params
	IgQtResourceMgr& theMgr = IgQtResourceMgr::getInstance();
	if ( theMgr.isValid() )
	{
		QDomDocument &theDoc = theMgr.getDoc( false );
		QDomElement session = theDoc.namedItem( "session" ).toElement();
		QDomNode l = session.namedItem( "browsers" );
		if ( !l.isNull() )
		{
			QDomNodeList ll = l.toElement().elementsByTagName( "browser" );
			bool found = false;
			for ( unsigned int i=0; i < ll.length() && !found; i++)
			{
				QDomElement de = ll.item( i ).toElement();
				if ( browser->getUniqueName() == de.attribute("resourceID") )
				{
					// found exact match
					browser->xmlToShell( de );
					found = true;
				}
			}
		}
	}
}
void IgQtMDI::removeBrowser( IgQtMDIAbsShell* browser )
{
	std::vector<IgQtMDIAbsShell*>::iterator it = std::find(childBrowsers.begin(), childBrowsers.end(), browser );
	childBrowsers.erase(it);
}
bool IgQtMDI::restoreResources( void )
{
	bool result = false; 

	IgQtResourceMgr& theMgr = IgQtResourceMgr::getInstance();
	if ( theMgr.isValid() )
	{

		QDomDocument &theDoc = theMgr.getDoc( false );
		QDomElement session = theDoc.namedItem( "session" ).toElement();

		result = ! session.isNull();

		//top-level widget
		QDomNode wnode = session.namedItem( "widget" );
		if ( !wnode.isNull() )
		{
			theMgr.xmlToWidget( this, wnode.toElement() );
		}
		QDomNode fnode = session.namedItem( "font" );
		if ( !fnode.isNull() )
		{
			theMgr.xmlToFont( fnode.toElement() );
		}

		// dock settings
		QDomNode dn = session.namedItem( "#cdata-section" );
		QDomCDATASection cds = dn.toCDATASection();
		if ( !cds.isNull() )
		{
			QString dataString;
			QTextStream data( &dataString, IO_ReadWrite );
			data <<  cds.data();
			data.device()->at(0); // reset to beginning of buffer
			data >> *this;

		}

		// browser settings
		QDomNode node = session.namedItem( "browsers" );
		if ( !node.isNull() )
		{
			// keep track of the maximized one
			IgQtMDIAbsShell* maxedShell=NULL;

			QDomNodeList ll = node.toElement().elementsByTagName( "browser" );
			for (unsigned int i = 0; i < ll.length(); i++)
			{
				QDomElement qe = ll.item( i ).toElement();
				QString name = qe.attribute( "resourceID" ).section( '_', 0, 0 );

				// skip if one is already there as a rough check
				std::vector<IgQtMDIAbsShell*>::iterator it = std::find_if( childBrowsers.begin(), childBrowsers.end(), testBrowserNames( name ));
				if (  it != childBrowsers.end() )
				{
					(*it)->xmlToShell(qe);
					if ((*it)->isMaximized()) maxedShell = *it;
				}
				else
				{
					IgQtMDIAbsShell *shell = IgQtMDIShellFactory::getInstance().getShell( name, this );
					//		    if ( shell && shell->shellName() == name ) shell->xmlToShell(qe);
					if (shell->isMaximized()) maxedShell = shell;
				}
			}
			if (maxedShell) maxedShell->showMaximized();
		}
		// now set top toolbox for Twig DockWindow
		QDomNode dockWindowsNode = session.namedItem( "DockWindows" );
		QDomNodeList ll = dockWindowsNode.toElement().elementsByTagName( "DockWindow" );
		for (unsigned int i = 0; i < ll.length(); i++)
		{
			QDomElement qe = ll.item( i ).toElement();
			QDomNodeList l3 = qe.toElement().elementsByTagName( "ToolBox" );
			if (l3.length() == 1)
			{
				QDomElement qe3 = l3.item(0).toElement();
				int currentIndex = qe3.attribute( "currentIndex" ).toInt();
				QObjectList *dws = queryList( "QToolBox", "IgTwigsTB");
				if (dws && !dws->isEmpty())
				{
					// must be only one, and index should be available
					QToolBox *qtb = static_cast<QToolBox*>(dws->first());
					if ( currentIndex < qtb->count() ) qtb->setCurrentIndex(currentIndex);
				}
			}
		}
	}
	return result;
}
void IgQtMDI::saveResources( void )
{
	// save the current size and location of the MDI itself
	IgQtResourceMgr& theMgr = IgQtResourceMgr::getInstance();

	QDomDocument &theDoc = theMgr.getDoc( true );
	QDomElement session = theDoc.createElement( "session" );
	theDoc.appendChild( session );

	// save widget parameters
	session.appendChild( theMgr.widgetToXML( this ) ); 

	// save font parameters
	session.appendChild( theMgr.fontToXML( QApplication::font() ) );

	// save params for all child browsers
	QDomElement browsers = theDoc.createElement( "browsers" );
	for (std::vector<IgQtMDIAbsShell*>::iterator it = childBrowsers.begin(); it != childBrowsers.end(); it++ )
	{
		browsers.appendChild( (*it)->shellToXML() );
	}
	session.appendChild( browsers );

	// save params for dock windows: this section does it by hand
	// which we will combine later with the automatic method (used below)

	QDomElement docks = theDoc.createElement( "DockWindows" );
	session.appendChild( docks );
	QObjectList *l = queryList( "QDockWindow" );
	if ( l && !l->isEmpty() )
	{
		for ( QObject* o = l->first(); o; o = l->next() ) 
		{
			QDockWindow *dw = static_cast<QDockWindow*>( o );
			int index, extraOffset;
			bool nl;
			Qt::Dock area; 

			if ( getLocation( dw, area, index, nl, extraOffset) )
			{
				QDomElement dockw = theDoc.createElement( "DockWindow" );
				docks.appendChild( dockw );
				// use (unique, we hope) widget name as resourceID
				dockw.setAttribute( "resourceID", dw->name() );

				QObjectList *ll = dw->queryList( "QToolBox", "IgTwigsTB");
				if (ll && !ll->isEmpty())
				{
					QObjectListIt iit( *ll );
					QObject* oo;
					while ( (oo = iit.current()) != 0 )
					{
						++iit;
						QToolBox *tb = static_cast<QToolBox*>( oo );
						QDomElement tbw = theDoc.createElement( "ToolBox" );
						tbw.setAttribute( "currentIndex", tb->currentIndex() );
						dockw.appendChild( tbw );
					}
				}
				delete ll;
			}
		}
	}
	delete l;

	// using the extraction method for QMainWindow: get's most of what we need
	QString dataString;
	QTextStream data( &dataString, IO_ReadWrite );
	data << *this;
	QDomCDATASection settings = theDoc.createCDATASection( dataString );
	session.appendChild( settings );

	theMgr.save();
}

