//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/QtGUI/interface/ISpyControlCenter.h"
#include "Iguana/QtGUI/interface/IgPolarCSControl.h"
#include "Iguana/QtGUI/interface/IgRZCSControl.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

ISpyControlCenter::ISpyControlCenter (QWidget* parent /*= 0*/)
    : QTabWidget (parent)
{
	ui.setupUi (this);
	
	addController (new IgPolarCSControl);
	addController (new IgRZCSControl);
}

//TODO: change the method so that the index isnt needed anymore, i.e. read the type of the item and search for it in the controllerlist
void
ISpyControlCenter::setControlledItem (const unsigned controllerIndex, ISpyItem* item)
{
	m_controllerList[controllerIndex]->setControlledItem (item);
}

// used in order to uncheck the controlcenter button if it is closed by pressing "x"
void 
ISpyControlCenter::closeEvent (QCloseEvent *event)
{
	emit isShown (false);
	event->accept();
}


// adds a controller to a already given (!) tab. This tab has to be created in the designer before
void
ISpyControlCenter::addController (ISpyController* controller)
{
	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget (controller);
	
	const unsigned listLen = m_controllerList.size ();
	widget (listLen)->setLayout (layout);
	
	m_controllerList.push_back (controller);
}


