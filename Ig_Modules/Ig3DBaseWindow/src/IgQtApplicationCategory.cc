//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/Ig3DBaseWindow/interface/IgQtApplicationCategory.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgState.h"
#include <qhbox.h>
#include <qwidget.h>
#include <qcombobox.h>
#include <qlabel.h>
#include <qlayout.h>
#include <classlib/utils/DebugAids.h>
#include <qapplication.h>
#include <qstyle.h>
#include <qstylefactory.h>
#include <qwhatsthis.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

IG_DEFINE_STATE_ELEMENT (IgQtApplicationCategory, "/ControlCentre/Categories/View Properties/Qt Application");

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgQtApplicationCategory::IgQtApplicationCategory (IgState *state)
    : IgCommonViewCategory (state, "Qt Application"),
      m_windowStyle	(0)
{
    addQApplicationTab ();
    updateWindowStyle (QApplication::style ().name ());
    state->put (s_key, this);
}

IgQtApplicationCategory::~IgQtApplicationCategory (void)
{ state()->detach (s_key); }

void
IgQtApplicationCategory::addQApplicationTab (void)
{
    QWidget	*qApplicationOptions = new QWidget (topArea ());
    topLayout ()->addWidget (qApplicationOptions);
    
    QVBoxLayout	*qApplicationVLayout  = new QVBoxLayout (qApplicationOptions);
    QGridLayout	*qApplicationLayout  = new QGridLayout (qApplicationVLayout);
    qApplicationLayout->setMargin (20);
    QWhatsThis::add (qApplicationOptions, "Edit QApplication's Properties");
    
    //Window Style
    QWidget 	*vlab1 = makeIndentedLabel ("Window Style",
                                             qApplicationOptions);
    qApplicationLayout->addWidget (vlab1, 0, 0);
    QWhatsThis::add (vlab1, "Select QApplication window's style");
    
    m_windowStyle = new QComboBox (qApplicationOptions);
    QStringList styleList = QStyleFactory::keys ();
    
    //adding available styles to the combobox
    for (QStringList::const_iterator it = styleList.begin ();
         it != styleList.end ();
	 it++ )
        m_windowStyle->insertItem (*it);
    qApplicationLayout->
	addWidget (makeBox (qApplicationOptions, m_windowStyle), 0, 1);
    QWhatsThis::add (m_windowStyle, "Select QApplication window's style");

    qApplicationVLayout->addItem (new QSpacerItem ( 0, 0, QSizePolicy::Expanding,
						    QSizePolicy::Expanding));
    connect (m_windowStyle,	SIGNAL (activated (const QString &)),
             this,		SLOT   (setWindowStyle (const QString &)));
    
    QWhatsThis::add (panel (), "Edit Qt applications properties");
    QWhatsThis::add (qApplicationOptions, "Edit Qt applications properties");
}

//////////////////////////////////////////////////////////////////////////
void
IgQtApplicationCategory::setWindowStyle (const QString & style)
{
    if (style != QApplication::style ().name ())
	QApplication::setStyle (style);
}

void
IgQtApplicationCategory::updateWindowStyle (const QString & style)
{
    for (int i = 0; i < m_windowStyle->count (); i++)
    {
	if (m_windowStyle->text (i) == style)
	{
	    m_windowStyle->setCurrentItem (i);
	    break;
	}
    }
}
