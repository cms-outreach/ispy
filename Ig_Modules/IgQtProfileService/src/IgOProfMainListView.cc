//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgQtProfileService/src/IgOProfMainListView.h"
#include "Ig_Modules/IgQtProfileService/src/IgOProfProcessListViewItem.h"
#include "Ig_Modules/IgQtProfileService/src/IgOProfSymbolListViewItem.h"
#include "Ig_Modules/IgQtProfileService/src/IgOProfConfiguration.h"
#include "Ig_Modules/IgQtProfileService/src/IgOProfPopupMenu.h"
#include "Ig_Modules/IgQtProfileService/src/IgOProfShowTextWindow.h"
#include "Ig_Modules/IgQtProfileService/src/IgOProfRepresentable.h"
#include "Ig_Modules/IgQtProfileService/src/IgOProfProcessRepresentable.h"
#include "Ig_Modules/IgQtProfileService/src/IgOProfSymbolRepresentable.h"

#include <qlistview.h>
#include <qwidget.h>
#include <qstring.h>
#include <qfile.h>
#include <qdatastream.h>
#include <qfiledialog.h>
#include <qtextstream.h>
#include <qobject.h>
#include <iostream>
#include <list>
#include <map>
#include <unistd.h>
#include <qdom.h>
#include <qregexp.h>
#include <qtimer.h>

using namespace std;

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgOProfMainListView::IgOProfMainListView(QWidget *w) 
    :QListView(w),
     m_configuration( IgOProfConfigurationSingleton::instance() ),
     m_representable( IgOProfRepresentableSingleton::instance() ),
     m_timer( new QTimer()),
     m_outOfDate(true)
{	
    addColumn("n of samples / %");
    addColumn("Program name/Symbols");
    addColumn("Library");
    
    setAllColumnsShowFocus(true);
    setResizeMode(LastColumn);
    resize(800,300);
    setRootIsDecorated(true);
    update();
    connect(this, SIGNAL(expanded(QListViewItem *)), this, SLOT(itemExpanded(QListViewItem *)));
    connect(this, SIGNAL(collapsed(QListViewItem *)), this, SLOT(itemCollapsed(QListViewItem *)));
    connect(this, SIGNAL(rightButtonPressed(QListViewItem *,const QPoint &,int)),this,SLOT(requestPopupMenu(QListViewItem *,const QPoint&, int)));
    connect(m_timer, SIGNAL(timeout()), this, SLOT(timedUpdate()));
    m_timer->start(100);    
}

void 
IgOProfMainListView::requestPopupMenu(QListViewItem *item, const QPoint &p, int /*column*/)
{	
    static QString SaveFilePath = QDir::currentDirPath();
    QRegExp dirname("\\/.*\\/");

    IgOProfProcessListViewItem *pr_item;
    //	IgOProfSymbolListViewItem *i;
    IgOProfShowTextWindow *win;
    QDomDocument *doc;
    QDomElement Root;
    QString Filename;
    QFile File;
    QTextStream ds;
    switch(PopupMenu.exec(p))
    {
    case IgOProfPopupMenu::POPUP_SHOW_SOURCE:
	win = new IgOProfShowTextWindow(m_configuration,item->text(1));
	break;
    case IgOProfPopupMenu::POPUP_SAVE_TO_FILE:
	Filename = QFileDialog::getSaveFileName(SaveFilePath,"Xml (*.xml)",this,"save file dialog", "Choose filename for profiling data export");
	dirname.search(Filename);
	SaveFilePath = dirname.cap();
	
	if(Filename!=QString::null)
	{
	    File.setName(Filename);
	    File.open(IO_WriteOnly);
	    ds.setDevice(&File);
	    doc = new QDomDocument("PROFILE-SAVE-FILE");
	    Root = doc->createElement("PROCESS-LIST");
	    doc->appendChild(Root);
	    Root.appendChild(((IgOProfProcessListViewItem*)item)->saveToFile(doc));
	    ds << doc->toString().ascii();
	    ds.unsetDevice();
	    File.close();
	}
	break;
    case IgOProfPopupMenu::POPUP_SET_OFFSET:
	pr_item = (IgOProfProcessListViewItem *) item;
	OffsetMap[pr_item->getName()] = pr_item->getHits();
	for(IgOProfSymbolListViewItem *t = (IgOProfSymbolListViewItem *) item->firstChild() ; t != NULL ; t = (IgOProfSymbolListViewItem *)t->nextSibling())
	{
	    if(t->getHits() > 0 )
	    {
		OffsetMap[pr_item->getName()+t->getLibrary()+t->getMangledName()] = t->getHits(); 
	    }
	}
	update();
	break;
    case IgOProfPopupMenu::POPUP_SET_MASK:
	pr_item = (IgOProfProcessListViewItem *) item;
	if(m_configuration->m_maskProcess == false) 
	{
	    m_configuration->m_maskProcess = true;
	    m_configuration->m_processMask = (const char *) pr_item->getName();
	}
	else 
	{
	    m_configuration->m_maskProcess = false;
	    m_configuration->m_processMask = "";
	}
	update();
    default:
	break;
    }
}

void 
IgOProfMainListView::saveSession(void)
{
    QString filename = QFileDialog::getSaveFileName("./","Session file (*.xml)",this,"save file dialog","Save session");
    QDomDocument doc("PROFILER-SESSION");
    QDomElement main = doc.createElement("PROFILER-SESSION-DATA");
    QDomElement t;
    doc.appendChild(main);
    for(map<QString,int>::iterator i = OffsetMap.begin() ; i != OffsetMap.end() ; i++ )
    {
	if(i->second != 0)
	{
	    t = doc.createElement("SYMBOL");
	    t.setAttribute("KEY",i->first);
	    t.setAttribute("OFFSET",i->second);
	    main.appendChild(t);
	}
    }
    QFile f(filename);
    f.open(IO_WriteOnly);
    QTextStream s(&f);
    doc.save(s,2);
    s.unsetDevice();
    f.close();
}

void
IgOProfMainListView::loadSession(void)
{
    QDomDocument doc;
    QString filename = QFileDialog::getOpenFileName("./","Session file (*.xml)",this,"load file dialog","Load session");
    QFile f(filename);
    f.open(IO_ReadOnly);
    doc.setContent(&f);
    f.close();
    QDomNodeList nodelist = doc.elementsByTagName ("SYMBOL");
    OffsetMap.clear();
    
    for( unsigned int i = 0 ; i < nodelist.count(); i++) 
    {
	QDomElement t = nodelist.item(i).toElement (); 
	OffsetMap[t.attribute("KEY")] = t.attribute ("OFFSET").toInt ();
    }		
}

void 
IgOProfMainListView::update (void)
{
    m_representable->requestUpdateProcessList ();
    QString currentItemName;
    
    if(currentItem () != 0 && currentItem ()->text (1)[0] == '/')
	currentItemName = currentItem ()->text(1);
    
    clear ();

    if (m_configuration->m_showPercent)
    {
	setColumnText (0, "%");	    
    }else
    {
	setColumnText (0, "Number of hits");	    
    }

    setColumnWidthMode (0, Maximum);
    setColumnWidth (1, 400);
    setColumnWidth (2, 300);
      
    for (IgOProfRepresentable::ProcessMap::iterator i = m_representable->begin ();
	 i != m_representable->end ();
	 i++)
    {
	IgOProfProcessRepresentable &currentProcess = *(i->second);
	ASSERT(&currentProcess != 0 );
	
	QString name = currentProcess.name ();
	
	if(!m_configuration->m_maskProcess
	   || name.contains (m_configuration->m_processMask.c_str()))
	{
	    IgOProfProcessListViewItem *item = new IgOProfProcessListViewItem(this,
									      name, 
									      currentProcess.hits (), 
									      OffsetMap[currentProcess.name ()]);	    
	    item->setExpandable (true);
	    if (currentProcess.isExpanded ())
	    {
		item->setOpen(true);
		
		for (IgOProfProcessRepresentable::SymbolMap::iterator i = currentProcess.begin(); 
		     i != currentProcess.end();
		     i++ )
		{
		    IgOProfSymbolRepresentable &currentSymbol = *(i->second);
		    int offset = OffsetMap[item->getName () 
					   + currentSymbol.library () 
					   + currentSymbol.name ()];
		    if (m_configuration->m_showZeros == true 
			|| currentSymbol.hits () - offset != 0)
		    {	
			QString name 	= currentSymbol.name ();
			QString library = currentSymbol.library ();
			int 	hits 	= currentSymbol.hits ();
			
			new IgOProfSymbolListViewItem(item, 
						      name, 
						      library, 
						      hits, 
						      offset, 
						      m_configuration->m_showPercent);
		    }	
		}		
	    }	    
	}
    }
}

void 
IgOProfMainListView::timedUpdate (void)
{
    if ( m_outOfDate == true)
    {
	m_outOfDate = false;	
	
	update();
    }
}

void
IgOProfMainListView::itemExpanded (QListViewItem *item)
{
    std::string processName = item->text (1);
    
    ASSERT( m_representable->find (processName) != m_representable->end ());
    if ((*m_representable)[processName]->isExpanded () == false)
    {
	ASSERT ((*m_representable)[processName]->isExpanded () == false);
	(*m_representable)[processName]->setIsExpanded (true);
	m_outOfDate = true;	
    }    
}		

void 
IgOProfMainListView::itemCollapsed (QListViewItem *item)
{
    std::string processName = item->text (1);
    
    ASSERT(m_representable->find (processName) != m_representable->end ());
    (*m_representable)[processName]->setIsExpanded (false);    
}

void
IgOProfMainListView::timeProcessExited (void)
{
}
