/*  Lucas preferred style.
    
    Colour table at:
    
    http://www.december.com/html/spec/colorsafecodes.html
 */


// Default style.
* {
  diffuse-color: rgb(0.7, 0.7, 0.7);
  transparency: 0.0;
  line-width: 1.0;
  line-pattern: 0xffff;
  font-size: 12;
  font-family: Arial Bold;
  draw-style: solid;
  marker-shape: square;
  marker-size: normal;
  marker-style: filled;
  text-align: left;
  min-energy: 0.2;
  max-energy: 5.0;
  energy-scale: 1.0;
  min-pt: 0.5;
  left: 0.;
  top: 0.;
}

Background {
  diffuse-color: rgb(0, 0, 0);
}

// Event information.
Event_V1 {
  diffuse-color: rgb(1.0, 1.0, 1.0); 
  font-size: 24.0; 
  font-family: Arial;
  background: :/images/cms-logo-small.png;
  left: 0.125;
  top: 0.025;
}

Event_V2 {
  diffuse-color: rgb(1.0, 1.0, 1.0); 
  font-size: 20.0; 
  font-family: Arial;
  background: :/images/cms-logo-small.png;
  left: 0.125;
  top: 0.025;
  annotation-level: press;
}

L1GtTrigger_V1 { diffuse-color: rgb(0.8, 0.8, 0.8); 
  text-align: right;
  left: 0.985;  
  top: 0.025;
  }

TriggerPaths_V1 { diffuse-color: rgb(0.8, 0.8, 0.8); 
                  text-align: right; 
                  left: 0.125;
                  top: 0.5;
}

Internal_Limits_V1 {
  diffuse-color: rgb(0.8, 0.8, 0.8);
  left: 0.35;
  top: 0.8;
}

TechTrigger_V1 {
  diffuse-color: rgb(0.8, 0.8, 0.8);
  text-align: left; 
  left: 0.70;
  top: 0.025;
}

Products_V1 { diffuse-color: rgb(0.8, 0.8, 0.8); }
Errors_V1 { diffuse-color: rgb(0.8, 0.8, 0.8); }


// Default geometry style.
Tracker3D_V1 { diffuse-color: rgb(0.7, 0.7, 0.0); transparency:0.95; }
TrackerRPhi_V1 { diffuse-color: rgb(0.7, 0.7, 0.0); transparency:0.8;}
TrackerRZ_V1 {diffuse-color: rgb(0.7, 0.7, 0.0); transparency:0.8;}

PixelBarrel3D_V1 { diffuse-color: rgb(0.7, 0.7, 0.0); transparency:0.95; }
PixelEndcap3D_V1 { diffuse-color: rgb(0.7, 0.7, 0.0); transparency:0.95; }
PixelEndcapPlus3D_V1 { diffuse-color: rgb(0.7, 0.7, 0.0); transparency:0.95; }
PixelEndcapMinus3D_V1 { diffuse-color: rgb(0.7, 0.7, 0.0); transparency:0.95; }

SiStripTIB3D_V1 { diffuse-color: rgb(0.7, 0.7, 0.0); transparency:0.95; }
SiStripTOB3D_V1 { diffuse-color: rgb(0.7, 0.7, 0.0); transparency:0.95; }
SiStripTID3D_V1 { diffuse-color: rgb(0.7, 0.7, 0.0); transparency:0.95; }
SiStripTEC3D_V1 { diffuse-color: rgb(0.7, 0.7, 0.0); transparency:0.95; }
SiStripTECPlus3D_V1 { diffuse-color: rgb(0.7, 0.7, 0.0); transparency:0.95; }
SiStripTECMinus3D_V1 { diffuse-color: rgb(0.7, 0.7, 0.0); transparency:0.95; }
SiStripTIDPlus3D_V1 { diffuse-color: rgb(0.7, 0.7, 0.0); transparency:0.95; }
SiStripTIDMinus3D_V1 { diffuse-color: rgb(0.7, 0.7, 0.0); transparency:0.95; }

EcalBarrel3D_V1 {diffuse-color: rgb(0.5, 0.8, 1.0); transparency:0.95;}
EcalEndcap3D_V1 {diffuse-color: rgb(0.5, 0.8, 1.0);transparency:0.95;}
 
EcalBarrelRPhi_V1 {diffuse-color: rgb(0.5, 0.8, 1.0); transparency:0.8;}
EcalBarrelRZ_V1 {diffuse-color: rgb(0.5, 0.8, 1.0);transparency:0.8;}

EcalEndcapRZ_V1 {diffuse-color: rgb(0.5, 0.8, 1.0);transparency:0.95;}
EcalEndcapPlus3D_V1 {diffuse-color: rgb(0.5, 0.8, 1.0);transparency:0.95;} 
EcalEndcapMinus3D_V1 {diffuse-color: rgb(0.5, 0.8, 1.0);transparency:0.95;} 

EcalPreshower3D_V1 {diffuse-color: rgb(0.5, 0.5, 0.5);transparency:0.95;}
EcalPreshowerRZ_V1 {diffuse-color: rgb(0.5, 0.5, 0.5);transparency:0.95;}

HcalBarrel3D_V1 {diffuse-color: rgb(0.7, 0.7, 0.0);transparency:0.95;}
HcalBarrelRPhi_V1 {diffuse-color: rgb(0.7, 0.7, 0.0);transparency:0.8;}
HcalBarrelRZ_V1 {diffuse-color: rgb(0.7, 0.7, 0.0);transparency:0.95;}

HcalEndcap3D_V1 {diffuse-color: rgb(0.7, 0.7, 0.0);transparency:0.95;}
HcalEndcapRZ_V1 {diffuse-color: rgb(0.7, 0.7, 0.0);transparency:0.95;}
HcalEndcapPlus3D_V1 {diffuse-color: rgb(0.7, 0.7, 0.0);transparency:0.95;}
HcalEndcapMinus3D_V1 {diffuse-color: rgb(0.7, 0.7, 0.0);transparency:0.95;}

HcalOuter3D_V1 {diffuse-color: rgb(0.7, 0.7, 0.0);transparency:0.95;}
HcalOuterRPhi_V1 {diffuse-color: rgb(0.7, 0.7, 0.0);transparency:0.8;}
HcalOuterRZ_V1 {diffuse-color: rgb(0.7, 0.7, 0.0);transparency:0.8;}

HcalForward3D_V1 {diffuse-color: rgb(0.7, 0.7, 0.0);transparency:0.95;}
HcalForwardRZ_V1 {diffuse-color: rgb(0.7, 0.7, 0.0);transparency:0.95;}
HcalForwardMinus3D_V1 {diffuse-color: rgb(0.7, 0.7, 0.0);transparency:0.95;}
HcalForwardPlus3D_V1 {diffuse-color: rgb(0.7, 0.7, 0.0);transparency:0.95;}

DTs3D_V1 {diffuse-color: rgb(0.8, 0.4, 0.0);transparency:0.5;}
DTsRPhi_V1 {diffuse-color: rgb(0.8, 0.4, 0.0);transparency:0.5;}
DTsRZ_V1 {diffuse-color: rgb(0.8, 0.4, 0.0);transparency:0.5;}

CSC3D_V1 {diffuse-color: rgb(0.6, 0.7, 0.1);transparency:0.5;}
CSCRZ_V1 {diffuse-color: rgb(0.6, 0.7, 0.1);transparency:0.5;}
CSCPlus3D_V1 {diffuse-color: rgb(0.6, 0.7, 0.1);transparency:0.5;}
CSCMinus3D_V1 {diffuse-color: rgb(0.6, 0.7, 0.1);transparency:0.5;}

RPC3D_V1 {diffuse-color: rgb(0.6, 0.8, 0.0);transparency:0.5;}
RPCBarrel3D_V1 {diffuse-color: rgb(0.6, 0.8, 0.0);transparency:0.5;}
RPCRPhi_V1 {diffuse-color: rgb(0.6, 0.8, 0.0);transparency:0.5;}
RPCRZ_V1 {diffuse-color: rgb(0.6, 0.8, 0.0);transparency:0.5;}
RPCPlusEndcap3D_V1 {diffuse-color: rgb(0.6, 0.8, 0.0);transparency:0.5;}
RPCMinusEndcap3D_V1 {diffuse-color: rgb(0.6, 0.8, 0.0);transparency:0.5;}

CaloLego_V1 {diffuse-color: rgb(1.0, 1.0, 1.0);transparency:0.5;}

// Simplified geometry
TOB_V1 { diffuse-color: rgb(0.7, 0.7, 0.0); transparency:0.8; }
TEC_V1 { diffuse-color: rgb(0.7, 0.7, 0.0); transparency:0.8; }
EB_V1 {diffuse-color: rgb(0.5, 0.8, 1.0); transparency:0.8;}
EE_V1 {diffuse-color: rgb(0.5, 0.8, 1.0);transparency:0.8;} 
HO_V1 {diffuse-color: rgb(0.7, 0.7, 0.0);transparency:0.8;}

// Default event objects style.
TriggerObjects_V1
{
  diffuse-color: rgb(1.0, 0.8, 1.0);
  font-size: 14;
  font-family: Helvetica;
  line-pattern: 0x0fff;
}

Tracks_V1
{
  draw-style: lines; line-width: 4;
  diffuse-color: rgb(1.0, 0.7, 0.1);
  marker-style: outline;
}

Tracks_V2
{
  draw-style: lines; 
  line-width: 3;
  diffuse-color: rgb(1.0, 0.7, 0.1);
  marker-style: outline;
  transparency: 0.3;
  min-pt: 1.0;
}


TrackDets_V1
{
  diffuse-color: rgb(1.0, 0.7, 0.1);
  transparency: 0.5;
}

PixelDigis_V1
{
  diffuse-color: rgb(1.0, 0.6, 0.0);
  marker-shape: plus;
}

SiPixelClusters_V1
{
  diffuse-color: rgb(1.0, 0.4, 0.0);
  marker-shape: cross; marker-size: normal; transparency: 0.5;
}

SiPixelRecHits_V1
{
  diffuse-color: rgb(1.0, 0.2, 0.0);
  marker-style: outline;
}

SiStripClusters_V1
{
  diffuse-color: rgb(0.8, 0.2, 0.0);
  marker-shape: cross; marker-size: normal; transparency: 0.5;
}

SiStripDigis_V1
{ 
  diffuse-color: rgb(0.8, 0.1, 0.0);
  marker-shape: plus;
}

TrackingRecHits_V1
{
  diffuse-color: rgb(1.0, 0.8, 0.0);
  marker-shape: circle; marker-size: big;
}

GsfTracks_V1
{
  draw-style: lines; 
  line-width: 2;
  diffuse-color: rgb(1.0, 0.8, 0.4);
}

EcalRecHits_V1 { diffuse-color: rgb(1.0, 0.2, 0.0); min-energy: 1.0; energy-scale: 0.03;}

EBRecHits_V1 {diffuse-color: rgb(1.0, 0.2, 0.0); min-energy: 1.0; energy-scale: 0.15;}
EERecHits_V1 {diffuse-color: rgb(1.0, 0.2, 0.0); min-energy: 0.2; energy-scale: 0.01;}
ESRecHits_V1 {diffuse-color: rgb(1.0, 0.6, 0.0); min-energy: 0.0005; energy-scale: 100.;}
HBRecHits_V1 {diffuse-color: rgb(0.4, 0.8, 1.0); min-energy: 1.0; energy-scale: 0.1;}
HERecHits_V1 {diffuse-color: rgb(0.4, 0.8, 1.0); min-energy: 1.0; energy-scale: 0.03;}
HFRecHits_V1 {diffuse-color: rgb(0.8, 1.0, 1.0); min-energy: 3.0; energy-scale: 0.03;}
HORecHits_V1 {diffuse-color: rgb(0.8, 1.0, 1.0); min-energy: 1.0; energy-scale: 0.03;}

/* FIXME: we should really support wildcards */
EBRecHits_V2 {diffuse-color: rgb(1.0, 0.2, 0.0); min-energy: 1.0; energy-scale: 0.1;}
EERecHits_V2 {diffuse-color: rgb(1.0, 0.2, 0.0); min-energy: 0.2; energy-scale: 0.1;}
ESRecHits_V2 {diffuse-color: rgb(1.0, 0.6, 0.0); min-energy: 0.0005; energy-scale: 100.;}
HBRecHits_V2 {diffuse-color: rgb(0.4, 0.8, 1.0); min-energy: 1.0; energy-scale: 0.02;}
HERecHits_V2 {diffuse-color: rgb(0.4, 0.8, 1.0); min-energy: 1.0; energy-scale: 0.02;}
HFRecHits_V2 {diffuse-color: rgb(0.8, 1.0, 1.0); min-energy: 3.0; energy-scale: 0.02;}
HORecHits_V2 {diffuse-color: rgb(0.8, 1.0, 1.0); min-energy: 1.0; energy-scale: 0.02;}

EBDigis_V1 {min-energy: 0.25; energy-scale: 1.0;}
EEDigis_V1 {min-energy: 0.8; energy-scale: 1.0;}

CaloClusters_V1 {diffuse-color: rgb(1.0, 0.5, 0.0); }
SuperCluster_V1 {diffuse-color: rgb(1.0, 0.2, 0.0); font-size: 14;}

DTDigis_V1 {
  diffuse-color: rgb(0.4, 1.0, 0.4);
  transparency: 0.2;
}

DTRecHits_V1 {
  draw-style: lines; 
  line-width: 3;
  diffuse-color: rgb(0.0, 0.8, 0.0);
}

DTRecSegment4D_V1 { 
  draw-style: lines; 
  line-width: 3;
  diffuse-color: rgb(0.8, 1.0, 0.8);
}

CSCSegments_V1 {
  draw-style: lines; 
  line-width: 3;
  diffuse-color: rgb(1.0, 0.8, 1.0);
}

CSCWireDigis_V1 {diffuse-color: rgb(1.0, 0.6, 1.0); transparency: 0.5;}
CSCStripDigis_V1 {diffuse-color: rgb(1.0, 0.2, 1.0); transparency: 0.5;}

CSCLCTDigis_V1 {
  marker-shape: cross;
  marker-size: huge;
  diffuse-color: rgb(0.0, 1.0, 1.0);
}


RPCRecHits_V1 {
  diffuse-color: rgb(1.0, 1.0, 0.2);
  line-width: 3;
}

CSCRecHit2Ds_V1 {
  diffuse-color: rgb(0.54, 0.89, 0.63);
  line-width: 3;
}

CSCRecHit2Ds_V2 {
  diffuse-color: rgb(0.54, 0.89, 0.63);
  line-width: 3;
}

Muons_V1 {
  draw-style: lines; 
  line-width: 3;
  diffuse-color: rgb(1.0, 0.0, 0.0);
}

TrackerMuons_V1 {
  draw-style: lines; 
  line-width: 3;
  diffuse-color: rgb(1.0, 0.0, 0.0);
}

StandaloneMuons_V1 {
  draw-style: lines; 
  line-width: 3;
  diffuse-color: rgb(1.0, 0.0, 0.0);
}

StandaloneMuons_V2 {
  draw-style: lines; 
  line-width: 3;
  diffuse-color: rgb(1.0, 0.0, 0.0);
  marker-style: outline;
}

GlobalMuons_V1 {
  draw-style: lines; 
  line-width: 3;
  diffuse-color: rgb(1.0, 0.0, 0.0);
}

MuonChambers_V1 {
  line-width: 2;
  diffuse-color: rgb(1.0, 0.0, 0.0);
  transparency:0.9;
}

PFRecTracks_V1 {
  draw-style: lines; 
  line-width: 3;
  diffuse-color: rgb(0.8, 0.8, 0.5);
}

GsfPFRecTracks_V1 {
  draw-style: lines; 
  line-width: 3;
  diffuse-color: rgb(0.0, 0.9, 1.0);
}

PFEBRecHits_V1 {diffuse-color: rgb(1.0, 0.1, 1.0);}
PFEERecHits_V1 {diffuse-color: rgb(1.0, 0.1, 1.0);}

PFBrems_V1 { 
  draw-style: lines; 
  diffuse-color: rgb(0.5, 1.0, 0.0);
  line-pattern: 0x9999; 
  line-width: 1;
}

PATTrackerMuons_V1 {
  draw-style: lines; 
  line-width: 3;
  diffuse-color: rgb(1.0, 0.0, 0.0);
}

PATStandaloneMuons_V1 {
  draw-style: lines; 
  line-width: 3;
  diffuse-color: rgb(1.0, 0.0, 0.0);
  marker-style: outline;
}

PATGlobalMuons_V1 {
  draw-style: lines; 
  line-width: 3;
  diffuse-color: rgb(1.0, 0.0, 0.0);
}

GsfElectrons_V1
{
  draw-style: lines; 
  line-width:3;
  diffuse-color: rgb(0.0, 1.0, 0.5);
}

PATElectrons_V1
{
  draw-style: lines; 
  line-width:3;
  diffuse-color: rgb(0.0, 1.0, 0.5);
}

CaloTowers_V1 {
  transparency: 0.75;
  diffuse-color: rgb(0.1, 1.0, 0.2);
}

CaloTowers_V2 {
  transparency: 0.75;
  diffuse-color: rgb(0.1, 1.0, 0.2);
  min-energy: 1.0;
  energy-scale: 0.05;
}

Photons_V1 { 
  diffuse-color: rgb(1.0, 1.0, 0.0); 
  draw-style: lines;
  line-width: 4;
  line-pattern: 0x0fff;
  min-energy: 5.0;
}

PATPhotons_V1 { 
  diffuse-color: rgb(1.0, 1.0, 0.0); 
  draw-style: lines;
  line-width: 4;
  line-pattern: 0x0fff;
  min-energy: 5.0;
}

Jets_V1 {
  transparency: 0.6; 
  diffuse-color: rgb(1.0, 1.0, 0.0);
  marker-shape: cross;
  min-energy: 15.0;
}

PATJets_V1 {
  transparency: 0.6; 
  diffuse-color: rgb(1.0, 1.0, 0.0);
  marker-shape: cross;
}

PFJets_V1 {
  transparency: 0.6; 
  diffuse-color: rgb(1.0, 1.0, 0.0);
  marker-shape: cross;
}

METs_V1 { 
  diffuse-color: rgb(1.0, 1.0, 0.5); 
  draw-style: lines;
  line-width: 3;
  line-pattern: 0x0fff;
  font-size: 14.0;
  font-family: Helvetica;
  top: 0;
  left: 0;
  text-align: center;
}

PATMETs_V1 { 
  diffuse-color: rgb(1.0, 1.0, 0.5); 
  draw-style: lines;
  line-width: 3;
  line-pattern: 0x0fff;
  font-size: 14.0;
}

PSimHits_V1 {diffuse-color: rgb(1.0, 0.1, 1.0);}
TrackingParticlePSimHits_V1 {diffuse-color: rgb(1.0, 0.1, 1.0);}
