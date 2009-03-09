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
	    IgProperty PT  = tracks.getProperty ("pt");
	    IgProperty POS = tracks.getProperty ("pos");
	    IgProperty DIR = tracks.getProperty ("dir");

	    IgAssociationSet &trackExtras = storage->getAssociationSet ("TrackExtras_V1");
	    IgCollection &extras = storage->getCollection ("Extras_V1");
// 	    IgAssociationSet &trackHits = storage->getAssociationSet ("TrackHits_V1");
// 	    IgCollection &hits = storage->getCollection ("Hits_V1");

	    IgCollectionIterator cit = tracks.begin ();
	    IgCollectionIterator cend = tracks.end ();
	    for (; cit != cend ; cit++) 
	    {
		IgCollectionItem itrack = *cit;
		IgV3d p1  = itrack.get<IgV3d> (POS);
		double x = p1.x();
		double y = p1.y();
		double z = p1.z();

		QString trackName = QString ("Track %1 GeV (%2, %3, %4)")
				    .arg (itrack.get<double>(PT))
				    .arg (x)
				    .arg (y)
				    .arg (z);

		std::vector<QPointF> points;
		std::vector<QPointF> tangents;

		IgV3d d1  = itrack.get<IgV3d> (DIR);
		double dx = d1.x();
		double dy = d1.y();
		double dz = d1.z();
// 		points.push_back (QPointF (x * 10., y * 10.));
// 		tangents.push_back (QPointF (dx, dy));
		
		for (IgAssociationSet::Iterator a = trackExtras.begin ();
		     a != trackExtras.end ();
		     ++a)
		{
		    if (a->first ().objectId () == itrack.currentRow ())
		    {
			IgCollectionItem m (&extras, a->second ().objectId ());
			p1 = m.get<IgV3d> ("pos_1");
			x = p1.x();
			y = p1.y();
			z = p1.z();
			d1 = m.get<IgV3d> ("dir_1");
			dx = d1.x();
			dy = d1.y();
			dz = d1.z();
			// points.push_back (QPointF (x * 40., y * 40.));
			points.push_back (QPointF (x, y));
			tangents.push_back (QPointF (dx, dy));

			p1 = m.get<IgV3d> ("pos_2");
			x = p1.x();
			y = p1.y();
			z = p1.z();
			d1 = m.get<IgV3d> ("dir_2");
			dx = d1.x();
			dy = d1.y();
			dz = d1.z();
			// points.push_back (QPointF (x * 40., y * 40.));
			points.push_back (QPointF (x, y));
			tangents.push_back (QPointF (dx, dy));
		    }
		}

		IggiMainWindow *mainWindow = dynamic_cast<IggiMainWindow *>(windowService->mainWindow ());
		// QGraphicsView *graphicsView = mainWindow->graphicsView;

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
