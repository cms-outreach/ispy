#define QT_NO_EMIT

//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/View/interface/IViewTrackingRecHitTwig.h"
#include "Iguana/View/interface/IViewReadService.h"
#include "Iguana/View/interface/IViewQWindowService.h"
#include "Iguana/View/interface/Ig3DRep.h"
#include "Iguana/View/interface/IgRPhiRep.h"
#include "Iguana/View/interface/IgRZRep.h"
#include "Iguana/View/interface/IgLegoRep.h"
#include "Iguana/View/interface/IgTextRep.h"
#include "Iguana/Iggi/interface/IggiMainWindow.h"
#include "Iguana/Iggi/interface/IggiScene.h"
#include "Iguana/Iggi/interface/IgHits.h"
#include "Iguana/Framework/interface/IgCollection.h"
#include "Iguana/View/interface/debug.h"
#include <QString>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IViewTrackingRecHitTwig::IViewTrackingRecHitTwig (IgState *state, IgTwig *parent,
						  const std::string &name /* = "" */)
    : IViewQueuedTwig (state, parent, name)
{
}

void
IViewTrackingRecHitTwig::onNewEvent (IViewEventMessage& message)
{
    IViewQueuedTwig::onNewEvent (message);
    if (IViewReadService *readService = IViewReadService::get (state ()))
    {	
	IViewQWindowService *windowService = IViewQWindowService::get (state ());
	ASSERT (windowService);
	
	IgDataStorage *storage = readService->dataStorage ();
	IgCollection &digis = storage->getCollection ("TrackingRecHits_V1");
	if (digis.size () > 0)
	{
	    IgColumnHandle pos = digis.getHandleByLabel ("pos");

	    std::vector<QPointF> points;
	    int n = 0;
	    IgCollectionIterator cit = digis.begin ();
	    IgCollectionIterator cend = digis.end ();
	    for (; cit != cend ; cit++, n++) 
	    {
		IgCollectionItem ipos = *cit;
		double x = ipos.get<IgV3d> ("pos").x ();		
		double y = ipos.get<IgV3d> ("pos").y ();
		double z = ipos.get<IgV3d> ("pos").z ();
		// points.push_back (QPointF (x * 40., y * 40.));
		points.push_back (QPointF (x, y));
	    }

	    IggiMainWindow *mainWindow = dynamic_cast<IggiMainWindow *>(windowService->mainWindow ());
	    QGraphicsView *graphicsView = mainWindow->graphicsView;

	    IggiScene *scene = dynamic_cast<IggiScene*>(mainWindow->graphicsView->scene ());
	    IgHits* hits = new IgHits (points);
	    hits->setColor (Qt::red);	    
	    scene->addItem (hits);
	    scene->update ();
	}
    }
}
