//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/QtGUI/interface/ISpyRecoContent.h"
#include "Iguana/Framework/interface/IgState.h"
#include "Iguana/QtGUI/interface/ISpyBasicClusterTwig.h"
#include "Iguana/QtGUI/interface/ISpyCaloTowerTwig.h"
#include "Iguana/QtGUI/interface/ISpyCSCSegmentTwig.h"
#include "Iguana/QtGUI/interface/ISpyDTRecSegment4DTwig.h"
#include "Iguana/QtGUI/interface/ISpyDetectorTwig.h"
#include "Iguana/QtGUI/interface/ISpyEventTwig.h"
#include "Iguana/QtGUI/interface/ISpyEcalRecHitTwig.h"
#include "Iguana/QtGUI/interface/ISpyHBRecHitTwig.h"
#include "Iguana/QtGUI/interface/ISpyHERecHitTwig.h"
#include "Iguana/QtGUI/interface/ISpyHFRecHitTwig.h"
#include "Iguana/QtGUI/interface/ISpyHORecHitTwig.h"
#include "Iguana/QtGUI/interface/ISpyJetTwig.h"
#include "Iguana/QtGUI/interface/ISpyMETTwig.h"
#include "Iguana/QtGUI/interface/ISpyMuonTwig.h"
#include "Iguana/QtGUI/interface/ISpyPFClusterTwig.h"
#include "Iguana/QtGUI/interface/ISpyPFRecHitTwig.h"
#include "Iguana/QtGUI/interface/ISpyPFRecTrackTwig.h"
#include "Iguana/QtGUI/interface/ISpyPixelDigiTwig.h"
#include "Iguana/QtGUI/interface/ISpyRPCRecHitTwig.h"
#include "Iguana/QtGUI/interface/ISpySiPixelClusterTwig.h"
#include "Iguana/QtGUI/interface/ISpySiPixelRecHitTwig.h"
#include "Iguana/QtGUI/interface/ISpySiStripClusterTwig.h"
#include "Iguana/QtGUI/interface/ISpySiStripDigiTwig.h"
#include "Iguana/QtGUI/interface/ISpyTrackingRecHitTwig.h"
#include "Iguana/QtGUI/interface/ISpyTrackTwig.h"
#include "Iguana/QtGUI/interface/ISpyDTDigiTwig.h"
#include "Iguana/QtGUI/interface/ISpyDTRecHitTwig.h"
#include "Iguana/QtGUI/interface/ISpyEBRecHitTwig.h"
#include "Iguana/QtGUI/interface/ISpyEERecHitTwig.h"

ISpyRecoContent::ISpyRecoContent (IgState *state)
    : m_state (state)
{
    init ();
}

IgState *
ISpyRecoContent::state (void) const
{ return m_state; }

void
ISpyRecoContent::init (void)
{    
    IgTwig *rootTwig = new IgSimpleTwig ("Root");// IgDocumentData::get (state ())->root ("Root", true);

    IgTwig *twig1 = new ISpyEventTwig (state (), rootTwig, "Event");
    twig1->selfVisible (true);
    IgTwig *twig2 = new ISpyBasicClusterTwig (state (), rootTwig, "BasicCluster");
    twig2->selfVisible (true);
    IgTwig *twig3 = new ISpyCaloTowerTwig (state (), rootTwig, "CaloTower");
    twig3->selfVisible (true);
    IgTwig *twig4 = new ISpyCSCSegmentTwig (state (), rootTwig, "CSCSegment");
    twig4->selfVisible (true);
    IgTwig *twig5 = new ISpyDTRecSegment4DTwig (state (), rootTwig, "DTRecSegment4D");
    twig5->selfVisible (true);
    IgTwig *twig6 = new ISpyDetectorTwig (state (), rootTwig, "Detector");
    twig6->selfVisible (true);
    IgTwig *twig7 = new ISpyEcalRecHitTwig (state (), rootTwig, "EcalRecHit");
    twig7->selfVisible (true);
    IgTwig *twig8 = new ISpyHBRecHitTwig (state (), rootTwig, "HBRecHit");
    twig8->selfVisible (true);
    IgTwig *twig9 = new ISpyHERecHitTwig (state (), rootTwig, "HERecHit");
    twig9->selfVisible (true);
    IgTwig *twig10 = new ISpyHFRecHitTwig (state (), rootTwig, "HFRecHit");
    twig10->selfVisible (true);
    IgTwig *twig11 = new ISpyHORecHitTwig (state (), rootTwig, "HORecHit");
    twig11->selfVisible (true);
    IgTwig *twig12 = new ISpyJetTwig (state (), rootTwig, "Jet");
    twig12->selfVisible (true);
    IgTwig *twig13 = new ISpyMETTwig (state (), rootTwig, "MET");
    twig13->selfVisible (true);
    IgTwig *twig14 = new ISpyMuonTwig (state (), rootTwig, "Muon");
    twig14->selfVisible (true);
    IgTwig *twig15 = new ISpyPFClusterTwig (state (), rootTwig, "PFCluster");
    twig15->selfVisible (true);
    IgTwig *twig16 = new ISpyPFRecHitTwig (state (), rootTwig, "PFRecHit");
    twig16->selfVisible (true);
    IgTwig *twig17 = new ISpyPFRecTrackTwig (state (), rootTwig, "PFRecTrack");
    twig17->selfVisible (true);
    IgTwig *twig18 = new ISpyPixelDigiTwig (state (), rootTwig, "PixelDigi");
    twig18->selfVisible (true);
    IgTwig *twig19 = new ISpyRPCRecHitTwig (state (), rootTwig, "RPCRecHit");
    twig19->selfVisible (true);
    IgTwig *twig20 = new ISpySiPixelClusterTwig (state (), rootTwig, "SiPixelCluster");
    twig20->selfVisible (true);
    IgTwig *twig21 = new ISpySiPixelRecHitTwig (state (), rootTwig, "SiPixelRecHit");
    twig21->selfVisible (true);
    IgTwig *twig22 = new ISpySiStripClusterTwig (state (), rootTwig, "SiStripCluster");
    twig22->selfVisible (true);
    IgTwig *twig23 = new ISpySiStripDigiTwig (state (), rootTwig, "SiStripDigi");
    twig23->selfVisible (true);
    IgTwig *twig24 = new ISpyTrackingRecHitTwig (state (), rootTwig, "TrackingRecHit");
    twig24->selfVisible (true);
    IgTwig *twig25 = new ISpyTrackTwig (state (), rootTwig, "Track");
    twig25->selfVisible (true);
    IgTwig *twig26 = new ISpyDTDigiTwig(state(), rootTwig, "DTDigi");
    twig26->selfVisible(true);
    IgTwig *twig29 = new ISpyDTRecHitTwig(state(), rootTwig, "DTRecHit");
    twig29->selfVisible(true);
}
