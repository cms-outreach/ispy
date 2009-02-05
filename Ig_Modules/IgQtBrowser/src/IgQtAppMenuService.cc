//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgQtBrowser/interface/IgQtAppMenuService.h"
#include "Ig_Modules/IgQtBrowser/interface/IgQtAppContextService.h"
#include <classlib/utils/DebugAids.h>
#include <classlib/utils/Callback.h>

#define INCLUDE_MENUITEM_DEF
#include <qmainwindow.h>
#include <qmenubar.h>
#include <qmenudata.h>
#include <qpopupmenu.h>
#include <qapplication.h>
#include <qnamespace.h>
#include <qobject.h>
#include <string>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

IG_DEFINE_STATE_ELEMENT (IgQtAppMenuService, "Services/Qt/App Menu");

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgQtAppMenuService::IgQtAppMenuService (IgState *state, QWidget *mainWindow)
    : m_state (state),
      m_mainWindow (dynamic_cast<QMainWindow *> (mainWindow))
{
    ASSERT (state);
    ASSERT (mainWindow);
    ASSERT (m_mainWindow);
    state->put (s_key, this);

    QMenuBar	*menubar = m_mainWindow->menuBar ();
    QPopupMenu	*filemenu = new QPopupMenu (m_mainWindow);

    menubar->insertItem ("&File", filemenu, MENU_ID_FILE,
			 index (menubar, MENU_ID_FILE));
    filemenu->setCaption ("File");
    filemenu->insertTearOffHandle ();
    filemenu->insertItem ("&Quit", qApp, SLOT(quit()),
			  Qt::CTRL+Qt::Key_Q, MENU_ID_FILE_QUIT,
			  index (filemenu, MENU_ID_FILE_QUIT));
    menubar->show ();
}

IgQtAppMenuService::~IgQtAppMenuService (void)
{
    ASSERT (m_state);
    ASSERT (m_mainWindow);

    m_state->detach (s_key);
    m_mainWindow->menuBar ()->clear ();
}

QMenuBar *
IgQtAppMenuService::menuBar (void)
{
    return m_mainWindow->menuBar ();
}

QPopupMenu *
IgQtAppMenuService::subMenu (int id)
{
    QMenuItem *item = menuBar ()->findItem (id);
    if (item)
	return item->popup ();
    return 0;
}

QPopupMenu *
IgQtAppMenuService::subMenu (int id1, int id2)
{
    QPopupMenu *item1 = subMenu (id1);
    if (item1)
    {
	QMenuItem *item2 = item1->findItem (id2);
	if (item2)
	{
	    return item2->popup ();	    
	}
    }

    return 0;
}

QPopupMenu *
IgQtAppMenuService::findByName (QMenuData *menu, const char *itemName)
{
    ASSERT (menu);
    
    for (unsigned int i = 0; i < menu->count (); i++)
    {
	QMenuItem *item = menu->findItem (menu->idAt (i));
	
	if (item->text () == itemName)
	    return item->popup ();	    
    }
    return 0;    
}

int
IgQtAppMenuService::index (QMenuData *menu, int id)
{
    ASSERT (menu);
    
    for (int i = 0; i < (int) menu->count (); ++i)
	if (menu->idAt (i) >= id)
	    return i;

    return menu->count ();
}

int
IgQtAppMenuService::isIdThere (QMenuData *menu, int id)
{
    ASSERT (menu);
    
    for (int i = 0; i < (int) menu->count (); ++i)
	if (menu->idAt (i) == id)
	    return i;
    return -1;    
}

typedef struct 
{
    QMenuData 	*m_menu;
    int 	m_id;
    QObject 	*m_target;
    std::string m_slot;    
} MenuConnection;

void 
connectMenuItem (MenuConnection * data)
{
    QMenuData *menu = data->m_menu;
    int id = data->m_id;
    QObject *target = data->m_target;
    const char *slot = data->m_slot.c_str ();    
    menu->connectItem (id, target, slot);    
    //std::cerr << "Connected:" << target << std::endl;
}

void 
disconnectMenuItem (MenuConnection *data)
{
    QMenuData *menu = data->m_menu;
    int id = data->m_id;
    QObject *target = data->m_target;
    const char * slot = data->m_slot.c_str ();    
    menu->disconnectItem (id, target, slot);
    //std::cerr << "Disconnected:" << target << std::endl;
}

// FIXME: add accelerators support.
void
IgQtAppMenuService::insertItem (QMenuData *parentMenu,
				const QString &text,
				QObject *target,
				const char *slot,
				int id,
				IgState *state)
{
    if (isIdThere (parentMenu, id) == -1)
    {
	parentMenu->insertItem (text,
				target,
				slot,
				0,
				id, 
				IgQtAppMenuService::index (parentMenu, id));    
    }
    
    if (state)
    {
	
	// if a state is passed, than the connection between the
	// target and the menuitem is context sensitive.
	IgQtAppContextService *cs = IgQtAppContextService::get (state);
	MenuConnection *data = new MenuConnection;
	ASSERT (cs);
	ASSERT (data);
	
	data->m_menu = parentMenu;
	data->m_id = id;
	data->m_target = target;
	data->m_slot = slot ? slot : "";
	
	cs->addOnFocusIn (lat::CreateCallback (&connectMenuItem,
					       data));	

 	cs->addOnFocusOut (lat::CreateCallback (&disconnectMenuItem,
						data));
        disconnectMenuItem (data);
    }
}

