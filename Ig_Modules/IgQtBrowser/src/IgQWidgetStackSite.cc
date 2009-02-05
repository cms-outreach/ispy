//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgQtBrowser/interface/IgQWidgetStackSite.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgDocumentDataRoot.h"
#include <qwidgetstack.h>
#include <qlayout.h>
#include <qvbox.h>
#include <qcombobox.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

// FIXME: the way widgets embeds into this site should be handled in a
// better way although I've no idea how....

IgQWidgetStackSite::IgQWidgetStackSite (IgState */*state*/, IgSite *parent)
    :m_box (new QVBox (hostFrom (parent))),
     m_combo (new QComboBox (m_box)),
     m_widget (new QWidgetStack (m_box)),
     m_layout (new QGridLayout (m_widget)),
     m_currentName (IgDocumentDataRoot::getCurrentRoot()),
     m_parent(parent)
{
    QObject::connect (m_combo,
		      SIGNAL (activated (int)), 
		      this, 
		      SLOT (switchToWidget (int)));    
    IgQtSite::host (parent, m_box);
    hostFrom (m_parent)->hide();
}

const char *
IgQWidgetStackSite::catalogLabel (void)
{ return "QWidgetStack"; }

QWidget *
IgQWidgetStackSite::self (void)
{
    return m_widget;
}

QWidget *
IgQWidgetStackSite::host (void)
{
    return m_widget;
}


IgSite *
IgQWidgetStackSite::hostWithName (const std::string &name)
{
    m_currentName = name;
    return this;
}

void
IgQWidgetStackSite::host (QWidget *child)
{
    QWidget *previousWidget = m_widget->visibleWidget ();
    if (previousWidget)
    {
    	m_layout->remove (previousWidget);	
    }
    
    int currentWidget = m_widget->addWidget (child);    
    m_layout->add (child);
    m_widget->raiseWidget (currentWidget);
    m_combo->insertItem (m_currentName);
    m_combo->setCurrentItem (m_combo->count () - 1);
    
    m_currentName = IgDocumentDataRoot::getCurrentRoot();
    m_comboIndexToWidgetID[m_combo->count () - 1] = currentWidget;
    hostFrom (m_parent)->show();
}

void
IgQWidgetStackSite::switchToWidget (int id)
{
    QWidget *previousWidget = m_widget->visibleWidget ();
    if (previousWidget)
    {
    	m_layout->remove (previousWidget);	
    }
    m_layout->add (m_widget->widget (m_comboIndexToWidgetID[id]));
    m_widget->raiseWidget (m_comboIndexToWidgetID[id]);
    IgDocumentDataRoot::currentRoot(m_combo->text(id));
    m_currentName = IgDocumentDataRoot::getCurrentRoot ();
}

void
IgQWidgetStackSite::tile (void)
{    
}

void
IgQWidgetStackSite::cascade (void)
{
}
