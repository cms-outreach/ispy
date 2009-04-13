//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/View/interface/IViewRecoContent.h"
#include "Iguana/View/interface/IViewBasicClusterTwig.h"
#include "Iguana/View/interface/IViewCaloTowerTwig.h"
#include "Iguana/View/interface/IViewCSCSegmentTwig.h"
#include "Iguana/View/interface/IViewDTRecSegment4DTwig.h"
#include "Iguana/View/interface/IViewDetectorTwig.h"
#include "Iguana/View/interface/IViewEventTwig.h"
#include "Iguana/View/interface/IViewEcalRecHitTwig.h"
#include "Iguana/View/interface/IViewHBRecHitTwig.h"
#include "Iguana/View/interface/IViewHERecHitTwig.h"
#include "Iguana/View/interface/IViewHFRecHitTwig.h"
#include "Iguana/View/interface/IViewHORecHitTwig.h"
#include "Iguana/View/interface/IViewJetTwig.h"
#include "Iguana/View/interface/IViewMETTwig.h"
#include "Iguana/View/interface/IViewMuonTwig.h"
#include "Iguana/View/interface/IViewPFClusterTwig.h"
#include "Iguana/View/interface/IViewPFRecHitTwig.h"
#include "Iguana/View/interface/IViewPFRecTrackTwig.h"
#include "Iguana/View/interface/IViewPixelDigiTwig.h"
#include "Iguana/View/interface/IViewRPCRecHitTwig.h"
#include "Iguana/View/interface/IViewSiPixelClusterTwig.h"
#include "Iguana/View/interface/IViewSiPixelRecHitTwig.h"
#include "Iguana/View/interface/IViewSiStripClusterTwig.h"
#include "Iguana/View/interface/IViewSiStripDigiTwig.h"
#include "Iguana/View/interface/IViewTrackingRecHitTwig.h"
#include "Iguana/View/interface/IViewTrackTwig.h"
#include <classlib/utils/Callback.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

IG_DEFINE_STATE_ELEMENT (IViewRecoContent, "Data/iView Reco");

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IViewRecoContent::IViewRecoContent (IgState *state)
    : IViewContent (state, s_key, MAIN_THREAD,
		    lat::CreateCallback (this, &IViewRecoContent::init))
{
}

void
IViewRecoContent::init (void)
{    
    IgTwig *rootTwig = new IgSimpleTwig ("Root");// IgDocumentData::get (state ())->root ("Root", true);

    IgTwig *twig1 = new IViewEventTwig (state (), rootTwig, "Event");
    twig1->selfVisible (true);
    IgTwig *twig2 = new IViewBasicClusterTwig (state (), rootTwig, "BasicCluster");
    twig2->selfVisible (true);
    IgTwig *twig3 = new IViewCaloTowerTwig (state (), rootTwig, "CaloTower");
    twig3->selfVisible (true);
    IgTwig *twig4 = new IViewCSCSegmentTwig (state (), rootTwig, "CSCSegment");
    twig4->selfVisible (true);
    IgTwig *twig5 = new IViewDTRecSegment4DTwig (state (), rootTwig, "DTRecSegment4D");
    twig5->selfVisible (true);
    IgTwig *twig6 = new IViewDetectorTwig (state (), rootTwig, "Detector");
    twig6->selfVisible (true);
    IgTwig *twig7 = new IViewEcalRecHitTwig (state (), rootTwig, "EcalRecHit");
    twig7->selfVisible (true);
    IgTwig *twig8 = new IViewHBRecHitTwig (state (), rootTwig, "HBRecHit");
    twig8->selfVisible (true);
    IgTwig *twig9 = new IViewHERecHitTwig (state (), rootTwig, "HERecHit");
    twig9->selfVisible (true);
    IgTwig *twig10 = new IViewHFRecHitTwig (state (), rootTwig, "HFRecHit");
    twig10->selfVisible (true);
    IgTwig *twig11 = new IViewHORecHitTwig (state (), rootTwig, "HORecHit");
    twig11->selfVisible (true);
    IgTwig *twig12 = new IViewJetTwig (state (), rootTwig, "Jet");
    twig12->selfVisible (true);
    IgTwig *twig13 = new IViewMETTwig (state (), rootTwig, "MET");
    twig13->selfVisible (true);
    IgTwig *twig14 = new IViewMuonTwig (state (), rootTwig, "Muon");
    twig14->selfVisible (true);
    IgTwig *twig15 = new IViewPFClusterTwig (state (), rootTwig, "PFCluster");
    twig15->selfVisible (true);
    IgTwig *twig16 = new IViewPFRecHitTwig (state (), rootTwig, "PFRecHit");
    twig16->selfVisible (true);
    IgTwig *twig17 = new IViewPFRecTrackTwig (state (), rootTwig, "PFRecTrack");
    twig17->selfVisible (true);
    IgTwig *twig18 = new IViewPixelDigiTwig (state (), rootTwig, "PixelDigi");
    twig18->selfVisible (true);
    IgTwig *twig19 = new IViewRPCRecHitTwig (state (), rootTwig, "RPCRecHit");
    twig19->selfVisible (true);
    IgTwig *twig20 = new IViewSiPixelClusterTwig (state (), rootTwig, "SiPixelCluster");
    twig20->selfVisible (true);
    IgTwig *twig21 = new IViewSiPixelRecHitTwig (state (), rootTwig, "SiPixelRecHit");
    twig21->selfVisible (true);
    IgTwig *twig22 = new IViewSiStripClusterTwig (state (), rootTwig, "SiStripCluster");
    twig22->selfVisible (true);
    IgTwig *twig23 = new IViewSiStripDigiTwig (state (), rootTwig, "SiStripDigi");
    twig23->selfVisible (true);
    IgTwig *twig24 = new IViewTrackingRecHitTwig (state (), rootTwig, "TrackingRecHit");
    twig24->selfVisible (true);
    IgTwig *twig25 = new IViewTrackTwig (state (), rootTwig, "Track");
    twig25->selfVisible (true);
}
