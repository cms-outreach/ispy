#define QT_NO_EMIT

//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/View/interface/IViewTrackTwig.h"
#include "Iguana/View/interface/IViewReadService.h"
#include "Iguana/View/interface/IViewQWindowService.h"
#include "Iguana/View/interface/Ig3DRep.h"
#include "Iguana/View/interface/IgRPhiRep.h"
#include "Iguana/View/interface/IgRZRep.h"
#include "Iguana/View/interface/IgLegoRep.h"
#include "Iguana/View/interface/IgTextRep.h"
#include "Iguana/Iggi/interface/IggiMainWindow.h"
#include "Iguana/Iggi/interface/IggiScene.h"
#include "Iguana/Iggi/interface/IgTrack.h"
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

IViewTrackTwig::IViewTrackTwig (IgState *state, IgTwig *parent,
				const std::string &name /* = "" */)
    : IViewQueuedTwig (state, parent, name),
      m_text ("no info")
{
}

void
IViewTrackTwig::onNewEvent (IViewEventMessage& message)
{
    IViewQueuedTwig::onNewEvent (message);
    if (IViewReadService *readService = IViewReadService::get (state ()))
    {	
	IViewQWindowService *windowService = IViewQWindowService::get (state ());
	ASSERT (windowService);
	
	IgDataStorage *storage = readService->dataStorage ();
	IgCollection &tracks = storage->getCollection ("Tracks_V1");
	if (tracks.size () > 0)
	{    
	    IgColumnHandle ptHandle = tracks.getHandleByLabel ("pt");
	    IgColumnHandle xHandle = tracks.getHandleByLabel ("x");
	    IgColumnHandle yHandle = tracks.getHandleByLabel ("y");
	    IgColumnHandle zHandle = tracks.getHandleByLabel ("z");
	    IgColumnHandle pxHandle = tracks.getHandleByLabel ("px");
	    IgColumnHandle pyHandle = tracks.getHandleByLabel ("py");
	    IgColumnHandle pzHandle = tracks.getHandleByLabel ("pz");
	    IgColumnHandle phiHandle = tracks.getHandleByLabel ("phi");
	    IgColumnHandle etaHandle = tracks.getHandleByLabel ("eta");
	    IgColumnHandle chargeHandle = tracks.getHandleByLabel ("charge");

	    IgAssociationSet &trackExtras = storage->getAssociationSet ("TrackExtras_V1");
	    IgCollection &extras = storage->getCollection ("Extras_V1");
	    IgAssociationSet &trackHits = storage->getAssociationSet ("TrackHits_V1");
	    IgCollection &hits = storage->getCollection ("Hits_V1");

	    int n = 0;
	    IgCollectionIterator cit = tracks.begin ();
	    IgCollectionIterator cend = tracks.end ();
	    for (; cit != cend ; cit++, ++n) 
	    {
		IgCollectionItem itrack = *cit;
		QString trackName = QString ("Track %1 GeV (%2, %3, %4)")
				    .arg (ptHandle.get<double> (n))
				    .arg (xHandle.get<double> (n))
				    .arg (yHandle.get<double> (n))
				    .arg (zHandle.get<double> (n));

		std::vector<QPointF> points;
		std::vector<QPointF> tangents;

		double x = xHandle.get<double> (n);
		double y = yHandle.get<double> (n);
		double z = zHandle.get<double> (n);
		double dx = pxHandle.get<double> (n);
		double dy = pyHandle.get<double> (n);
		double dz = pzHandle.get<double> (n);
// 		points.push_back (QPointF (x * 10., y * 10.));
// 		tangents.push_back (QPointF (dx, dy));
		
		for (IgAssociationSet::Iterator a = trackExtras.begin ();
		     a != trackExtras.end ();
		     ++a)
		{
		    if (a->first ().objectId () == itrack.currentRow ())
		    {
			IgCollectionItem m (&extras, a->second ().objectId ());
			x = m.get<double>("ix");
			y = m.get<double>("iy");
			z = m.get<double>("iz");
			dx = m.get<double>("ipx");
			dy = m.get<double>("ipy");
			dz = m.get<double>("ipz");
			points.push_back (QPointF (x * 40., y * 40.));
			tangents.push_back (QPointF (dx, dy));

			x = m.get<double>("ox");
			y = m.get<double>("oy");
			z = m.get<double>("oz");
			dx = m.get<double>("opx");
			dy = m.get<double>("opy");
			dz = m.get<double>("opz");
			points.push_back (QPointF (x * 40., y * 40.));
			tangents.push_back (QPointF (dx, dy));
		    }
		}

		IggiMainWindow *mainWindow = dynamic_cast<IggiMainWindow *>(windowService->mainWindow ());
		QGraphicsView *graphicsView = mainWindow->graphicsView;

		IggiScene *scene = dynamic_cast<IggiScene*>(mainWindow->graphicsView->scene ());
		IgTrack* track = new IgTrack (points, tangents, IgTrack::SPLINE);
		QPen pen;
		pen.setBrush (Qt::blue);	
		track->setPen (pen);
		track->setToolTip (trackName);
		scene->addItem (track);
		scene->update ();
	    }
	}
    }
}

void
IViewTrackTwig::update (IgTextRep *rep)
{
    // Get debugging dump.
    IViewQueuedTwig::update (rep);

    rep->setText (m_text);
}

void
IViewTrackTwig::update (Ig3DRep *rep)
{
    // Get debugging dump.
    IViewQueuedTwig::update (rep);

    //    rep->clear ();
}

void
IViewTrackTwig::update (IgRPhiRep *rep)
{
    // Get debugging dump.
    IViewQueuedTwig::update (rep);

    //    rep->clear ();
}

void
IViewTrackTwig::update (IgRZRep *rep)
{
    // Get debugging dump.
    IViewQueuedTwig::update (rep);

    //    rep->clear ();
}

void
IViewTrackTwig::update (IgLegoRep *rep)
{
    // Get debugging dump.
    IViewQueuedTwig::update (rep);

    //    rep->clear ();
}
