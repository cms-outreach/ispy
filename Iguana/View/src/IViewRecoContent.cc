//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/View/interface/IViewRecoContent.h"
#include "Iguana/View/interface/IViewEventTwig.h"
#include "Iguana/View/interface/IViewDetectorTwig.h"
#include "Iguana/View/interface/IViewTrackTwig.h"
#include "Iguana/View/interface/IViewDTRecSegment4DTwig.h"
#include "Iguana/View/interface/IViewSiStripDigiTwig.h"
#include "Iguana/View/interface/IViewTrackingRecHitTwig.h"
#include "Iguana/View/interface/IViewMuonTwig.h"
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

    IgTwig *twig1 = new IViewEventTwig (state (), rootTwig, "Event ID");
    twig1->selfVisible (true);
    IgTwig *twig2 = new IViewTrackTwig (state (), rootTwig, "Tracks");
    twig2->selfVisible (true);
//     IgTwig *twig3 = new IViewCaloTowerTwig (state (), rootTwig, "CaloTowers");
//     twig3->selfVisible (true);
//     IgTwig *twig4 = new IViewCSCSegmentTwig (state (), rootTwig, "CSC Segments");
//     twig4->selfVisible (true);
    IgTwig *twig5 = new IViewDTRecSegment4DTwig (state (), rootTwig, "DT Segments 4D");
    twig5->selfVisible (true);
//     IgTwig *twig6 = new IViewJetTwig (state (), rootTwig, "Jets");
//     twig6->selfVisible (true);
//     IgTwig *twig7 = new IViewMETTwig (state (), rootTwig, "METs");
//     twig7->selfVisible (true);
//     IgTwig *twig8 = new IViewPFClusterTwig (state (), rootTwig, "PFClusters");
//     twig8->selfVisible (true);
//     IgTwig *twig9 = new IViewPixelDigiTwig (state (), rootTwig, "Pixel Digis");
//     twig9->selfVisible (true);
//     IgTwig *twig10 = new IViewSiPixelClusterTwig (state (), rootTwig, "SiPixel Clusters");
//     twig10->selfVisible (true);
//     IgTwig *twig11 = new IViewSiPixelRecHitTwig (state (), rootTwig, "SiPixel RecHits");
//     twig11->selfVisible (true);
//     IgTwig *twig12 = new IViewSiStripClusterTwig (state (), rootTwig, "SiStrip Clusters");
//     twig12->selfVisible (true);
    IgTwig *twig13 = new IViewSiStripDigiTwig (state (), rootTwig, "SiStrip Digis");
    twig13->selfVisible (true);
    IgTwig *twig14 = new IViewTrackingRecHitTwig (state (), rootTwig, "Tracking RecHits");
    twig14->selfVisible (true);
    IgTwig *twig15 = new IViewMuonTwig (state (), rootTwig, "Muons");
    twig15->selfVisible (true);
    IgTwig *twig16 = new IViewDetectorTwig (state (), rootTwig, "Geometry");
    twig16->selfVisible (true);
}
