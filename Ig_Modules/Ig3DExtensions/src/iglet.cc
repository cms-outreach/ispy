//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Framework/IgObjectBrowser/interface/xtypeinfo.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgCapabilityDB.h"
#include "Ig_Framework/IgPluginManager/interface/IgModuleDef.h"
#include "Ig_Modules/Ig3DSystem/interface/Ig3DSystem.h"
#include "Ig_Modules/Ig3DExtensions/interface/config.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgParticleChar.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSoShapeKit.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSo2DArrow.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSo3DErrorBar.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSoArrow.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSoAxis.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSoCalHit.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSoCircleArc.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSoCircularHist.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSoCoordinateAxis.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSoCrystalHit.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSoCube.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSoEllipsoid.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSoFieldPlane.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSoFieldPlaneMap.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSoG4Box.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSoG4Trap.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSoG4Trd.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSoGrid.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSoHits.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSoIdealTrack.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSoJet.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSoLegoPlot.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSoLegoTowers.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSoPcon.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSoPolyVol.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSoQuad.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSoRotSolid.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSoRZHist.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSoSiStrips.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSoSimpleTrajectory.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSoSphereHit.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSoSplineTrack.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSoTower.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSoTowerRadii.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSoXYZGrid.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSoAnimator.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSoClipPlane.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSoSlicer.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSoSlicerEngine.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSoViewpoint.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSoPlaneManip.h"
#include "Ig_Extensions/IgOpenInventor/interface/IgSoGL2PSAction.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

static void initNodes (void)
{
    Ig3DSystem::initAction (lat::CreateCallback (&IgParticleChar::initParticles));
    Ig3DSystem::initAction (lat::CreateCallback (&IgSoShapeKit::initClass));
    Ig3DSystem::initAction (lat::CreateCallback (&IgSo2DArrow::initClass));
    Ig3DSystem::initAction (lat::CreateCallback (&IgSo3DErrorBar::initClass));
    Ig3DSystem::initAction (lat::CreateCallback (&IgSoArrow::initClass));
    Ig3DSystem::initAction (lat::CreateCallback (&IgSoAxis::initClass));
    Ig3DSystem::initAction (lat::CreateCallback (&IgSoCalHit::initClass));
    Ig3DSystem::initAction (lat::CreateCallback (&IgSoCircleArc::initClass));
    Ig3DSystem::initAction (lat::CreateCallback (&IgSoCircularHist::initClass));
    Ig3DSystem::initAction (lat::CreateCallback (&IgSoCoordinateAxis::initClass));
    Ig3DSystem::initAction (lat::CreateCallback (&IgSoCrystalHit::initClass));
    Ig3DSystem::initAction (lat::CreateCallback (&IgSoCube::initClass));
    Ig3DSystem::initAction (lat::CreateCallback (&IgSoEllipsoid::initClass));
    Ig3DSystem::initAction (lat::CreateCallback (&IgSoFieldPlane::initClass));
    Ig3DSystem::initAction (lat::CreateCallback (&IgSoFieldPlaneMap::initClass));
    Ig3DSystem::initAction (lat::CreateCallback (&IgSoG4Box::initClass));
    Ig3DSystem::initAction (lat::CreateCallback (&IgSoG4Trap::initClass));
    Ig3DSystem::initAction (lat::CreateCallback (&IgSoG4Trd::initClass));
    Ig3DSystem::initAction (lat::CreateCallback (&IgSoGrid::initClass));
    Ig3DSystem::initAction (lat::CreateCallback (&IgSoHits::initClass));
    Ig3DSystem::initAction (lat::CreateCallback (&IgSoIdealTrack::initClass));
    Ig3DSystem::initAction (lat::CreateCallback (&IgSoJet::initClass));
    Ig3DSystem::initAction (lat::CreateCallback (&IgSoLegoPlot::initClass));
    Ig3DSystem::initAction (lat::CreateCallback (&IgSoLegoTowers::initClass));
    Ig3DSystem::initAction (lat::CreateCallback (&IgSoPcon::initClass));
    Ig3DSystem::initAction (lat::CreateCallback (&IgSoPolyVol::initClass));
    Ig3DSystem::initAction (lat::CreateCallback (&IgSoQuad::initClass));
    Ig3DSystem::initAction (lat::CreateCallback (&IgSoRotSolid::initClass));
    Ig3DSystem::initAction (lat::CreateCallback (&IgSoRZHist::initClass));
    Ig3DSystem::initAction (lat::CreateCallback (&IgSoSiStrips::initClass));
    Ig3DSystem::initAction (lat::CreateCallback (&IgSoSimpleTrajectory::initClass));
    Ig3DSystem::initAction (lat::CreateCallback (&IgSoSphereHit::initClass));
    Ig3DSystem::initAction (lat::CreateCallback (&IgSoSplineTrack::initClass));
    Ig3DSystem::initAction (lat::CreateCallback (&IgSoTower::initClass));
    Ig3DSystem::initAction (lat::CreateCallback (&IgSoTowerRadii::initClass));
    Ig3DSystem::initAction (lat::CreateCallback (&IgSoXYZGrid::initClass));
    Ig3DSystem::initAction (lat::CreateCallback (&IgSoAnimator::initClass));
    Ig3DSystem::initAction (lat::CreateCallback (&IgSoClipPlane::initClass));
    Ig3DSystem::initAction (lat::CreateCallback (&IgSoSlicer::initClass));
    Ig3DSystem::initAction (lat::CreateCallback (&IgSoSlicerEngine::initClass));
    Ig3DSystem::initAction (lat::CreateCallback (&IgSoViewpoint::initClass));
    Ig3DSystem::initAction (lat::CreateCallback (&IgSoPlaneManip::initClass));
    Ig3DSystem::initAction (lat::CreateCallback (&IgSoGL2PSAction::initClass));
   
}

DEFINE_IGUANA_MODULE ();
DEFINE_IG_PLUGIN_NAME (IgCapabilityDB, "Services/Inventor/NodeKits/IgOpenInventor");
IG_MODULE_ATTACH_ACTION ((&initNodes));
