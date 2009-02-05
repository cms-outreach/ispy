#define QT_NO_EMIT

//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/View/interface/IViewSiStripDigiTwig.h"
#include "Iguana/View/interface/IViewReadService.h"
#include "Iguana/View/interface/IViewQWindowService.h"
#include "Iguana/Iggi/interface/IggiMainWindow.h"
#include "Iguana/Iggi/interface/IggiScene.h"
#include "Iguana/Iggi/interface/IgHits.h"
#include "Iguana/Framework/interface/IgCollection.h"
#include "Iguana/View/interface/debug.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IViewSiStripDigiTwig::IViewSiStripDigiTwig (IgState *state, IgTwig *parent,
					    const std::string &name /* = "" */)
    : IViewQueuedTwig (state, parent, name)
{
}

void
IViewSiStripDigiTwig::onNewEvent (IViewEventMessage& message)
{
    IViewQueuedTwig::onNewEvent (message);
    if (IViewReadService *readService = IViewReadService::get (state ()))
    {	
	IViewQWindowService *windowService = IViewQWindowService::get (state ());
	ASSERT (windowService);
	
	IgDataStorage *storage = readService->dataStorage ();
	IgCollection &digis = storage->getCollection ("SiStripDigis_V1");
	if (digis.size () > 0)
	{
	    IgColumnHandle xHandle = digis.getHandleByLabel ("x");
	    IgColumnHandle yHandle = digis.getHandleByLabel ("y");
	    IgColumnHandle zHandle = digis.getHandleByLabel ("z");

	    std::vector<QPointF> points;
	    int n = 0;
	    IgCollectionIterator cit = digis.begin ();
	    IgCollectionIterator cend = digis.end ();
	    for (; cit != cend ; cit++, n++) 
	    {
		double x = xHandle.get<double> (n);
		double y = yHandle.get<double> (n);
		double z = zHandle.get<double> (n);
		points.push_back (QPointF (x * 40., y * 40.));
	    }

	    IggiMainWindow *mainWindow = dynamic_cast<IggiMainWindow *>(windowService->mainWindow ());
	    QGraphicsView *graphicsView = mainWindow->graphicsView;

	    IggiScene *scene = dynamic_cast<IggiScene*>(mainWindow->graphicsView->scene ());
	    IgHits* hits = new IgHits (points);
	    hits->setColor (Qt::green);	    
	    scene->addItem (hits);
	    scene->update ();
	}
    }
}


