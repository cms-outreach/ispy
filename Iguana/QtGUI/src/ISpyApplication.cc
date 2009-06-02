//<<<<<< INCLUDES                                                       >>>>>>
#define QT_NO_EMIT
#include "Iguana/QtGUI/interface/ISpyApplication.h"
#include "Iguana/QtGUI/interface/ISpy3DView.h"
#include "Iguana/QtGUI/interface/ISpyEventFilter.h"
#include "Iguana/QtGUI/interface/ISpyMainWindow.h"
#include "Iguana/QtGUI/interface/ISpySplashScreen.h"
#include "Iguana/QtGUI/interface/Ig3DBaseModel.h"
#include "Iguana/QtGUI/interface/IgCollectionTableModel.h"
#include "Iguana/Framework/interface/IgCollection.h"
#include "Iguana/Framework/interface/IgParser.h"
#include "Iguana/Inventor/interface/IgParticleChar.h"
#include "Iguana/Inventor/interface/IgSbColorMap.h"
#include "Iguana/Inventor/interface/IgSo2DArrow.h"
#include "Iguana/Inventor/interface/IgSo3DErrorBar.h"
#include "Iguana/Inventor/interface/IgSoAnimator.h"
#include "Iguana/Inventor/interface/IgSoArrow.h"
#include "Iguana/Inventor/interface/IgSoAxis.h"
#include "Iguana/Inventor/interface/IgSoCalHit.h"
#include "Iguana/Inventor/interface/IgSoCircleArc.h"
#include "Iguana/Inventor/interface/IgSoCircularHist.h"
#include "Iguana/Inventor/interface/IgSoClipPlane.h"
#include "Iguana/Inventor/interface/IgSoCoordinateAxis.h"
#include "Iguana/Inventor/interface/IgSoCrystalHit.h"
#include "Iguana/Inventor/interface/IgSoCube.h"
#include "Iguana/Inventor/interface/IgSoEllipsoid.h"
#include "Iguana/Inventor/interface/IgSoFieldPlane.h"
#include "Iguana/Inventor/interface/IgSoFieldPlaneMap.h"
#include "Iguana/Inventor/interface/IgSoG4Box.h"
#include "Iguana/Inventor/interface/IgSoG4Trap.h"
#include "Iguana/Inventor/interface/IgSoG4Trd.h"
#include "Iguana/Inventor/interface/IgSoGrid.h"
#include "Iguana/Inventor/interface/IgSoGridPlane.h"
#include "Iguana/Inventor/interface/IgSoGridPlaneMap.h"
#include "Iguana/Inventor/interface/IgSoHits.h"
#include "Iguana/Inventor/interface/IgSoIdealTrack.h"
#include "Iguana/Inventor/interface/IgSoJet.h"
#include "Iguana/Inventor/interface/IgSoLegoPlot.h"
#include "Iguana/Inventor/interface/IgSoLegoTowers.h"
#include "Iguana/Inventor/interface/IgSoPcon.h"
#include "Iguana/Inventor/interface/IgSoPlaneManip.h"
#include "Iguana/Inventor/interface/IgSoPolyVol.h"
#include "Iguana/Inventor/interface/IgSoQuad.h"
#include "Iguana/Inventor/interface/IgSoRZHist.h"
#include "Iguana/Inventor/interface/IgSoRectColHist.h"
#include "Iguana/Inventor/interface/IgSoRectHist.h"
#include "Iguana/Inventor/interface/IgSoRotSolid.h"
#include "Iguana/Inventor/interface/IgSoShapeKit.h"
#include "Iguana/Inventor/interface/IgSoSiStrips.h"
#include "Iguana/Inventor/interface/IgSoSimpleTrajectory.h"
#include "Iguana/Inventor/interface/IgSoSlicer.h"
#include "Iguana/Inventor/interface/IgSoSlicerEngine.h"
#include "Iguana/Inventor/interface/IgSoSphereHit.h"
#include "Iguana/Inventor/interface/IgSoSplineTrack.h"
#include "Iguana/Inventor/interface/IgSoTower.h"
#include "Iguana/Inventor/interface/IgSoTowerRadii.h"
#include "Iguana/Inventor/interface/IgSoViewpoint.h"
#include "Iguana/Inventor/interface/IgSoXYZGrid.h"
#include "classlib/iobase/File.h"
#include "classlib/iobase/Filename.h"
#include "classlib/iotools/InputStream.h"
#include "classlib/utils/ShellEnvironment.h"
#include "classlib/utils/StringOps.h"
#include "classlib/utils/Error.h"
#include "classlib/zip/ZipMember.h"
#include "classlib/zip/ZipArchive.h"
#include <Inventor/Qt/SoQt.h>
#include <Inventor/Qt/viewers/SoQtExaminerViewer.h>
#include <Inventor/nodes/SoAnnotation.h>
#include <Inventor/nodes/SoBaseColor.h>
#include <Inventor/nodes/SoCone.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoFont.h>
#include <Inventor/nodes/SoIndexedLineSet.h>
#include <Inventor/nodes/SoLineSet.h>
#include <Inventor/nodes/SoMarkerSet.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoMaterialBinding.h>
#include <Inventor/nodes/SoNodes.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoPointSet.h>
#include <Inventor/nodes/SoPolygonOffset.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoText2.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoVertexProperty.h>
#include <QApplication>
#include <QEventLoop>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QPixmap>
#include <QSettings>
#include <QString>
#include <QStringList>
#include <QTreeWidget>
#include <QtGui>
#include <iostream>

static const double cCrystalHitScale = 0.3;
static const double cCaloTowerScale = 0.03;

using namespace lat;

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>

static void initShapes (void)
{
  IgParticleChar::initParticles ();
  IgSoShapeKit::initClass ();
  IgSo2DArrow::initClass ();
  IgSo3DErrorBar::initClass ();
  IgSoArrow::initClass ();
  IgSoAxis::initClass ();
  IgSoCalHit::initClass ();
  IgSoCircleArc::initClass ();
  IgSoCoordinateAxis::initClass ();
  IgSoCrystalHit::initClass ();
  IgSoCube::initClass ();
  IgSoEllipsoid::initClass ();
  IgSoFieldPlane::initClass ();
  IgSoFieldPlaneMap::initClass ();
  IgSoG4Box::initClass ();
  IgSoG4Trap::initClass ();
  IgSoG4Trd::initClass ();
  IgSoGrid::initClass ();
  IgSoHits::initClass ();
  IgSoIdealTrack::initClass ();
  IgSoJet::initClass ();
  IgSoLegoPlot::initClass ();
  IgSoLegoTowers::initClass ();
  IgSoPcon::initClass ();
  IgSoPolyVol::initClass ();
  IgSoQuad::initClass ();
  IgSoRectHist::initClass ();
  IgSoRectColHist::initClass ();
  IgSoRotSolid::initClass ();
  IgSoRZHist::initClass ();
  IgSoSiStrips::initClass ();
  IgSoSimpleTrajectory::initClass ();
  IgSoSphereHit::initClass ();
  IgSoSplineTrack::initClass ();
  IgSoTower::initClass ();
  IgSoTowerRadii::initClass ();
  IgSoXYZGrid::initClass ();
  IgSoAnimator::initClass ();
  IgSoClipPlane::initClass ();
  IgSoSlicer::initClass ();
  IgSoSlicerEngine::initClass ();
  IgSoViewpoint::initClass ();
  IgSoPlaneManip::initClass();
  IgSoCircularHist::initClass ();
  IgSoGridPlane::initClass ();
  IgSoGridPlaneMap::initClass ();
}

static void
createTextLine(SoGroup *group, SoTranslation *trans, const std::string &text)
{
  SoText2 *label = new SoText2;
  label->string = text.c_str ();
  group->addChild (trans);
  group->addChild (label);
}

static void
make3DPointSetShapes(IgCollection **collections,
		     IgAssociationSet **,
		     SoSeparator *sep,
		     SbColor colour,
		     int kind)
{
  IgCollection		*c = collections[0];
  IgProperty		POS = c->getProperty ("pos");
  SoMaterial		*mat = new SoMaterial;
  SoMarkerSet		*points = new SoMarkerSet;
  SoVertexProperty	*vertices = new SoVertexProperty;
  int			n = 0;

  mat->diffuseColor = colour;
  sep->addChild (mat);

  for (IgCollectionIterator ci = c->begin(), ce = c->end(); ci != ce; ++ci)
  {
    IgV3d p1 = ci->get<IgV3d> (POS);

    double x = p1.x ();
    double y = p1.y ();
    double z = p1.z ();
    vertices->vertex.set1Value (n++, SbVec3f (x, y, z));
  }
  vertices->vertex.setNum (n);

  points->markerIndex = kind;
  points->vertexProperty = vertices;
  points->numPoints = n;
  sep->addChild (points);
}

static void
make3DCrystalHitShapes(IgCollection **collections, IgAssociationSet **, SoSeparator *sep)
{
  IgCollection		*c = collections[0];
  QSettings		settings;
  double		ecut = 0.1;
  bool			hlrMode = false;
  SoAnnotation		*ann = 0;
  SoVertexProperty	*vertices = new SoVertexProperty;
  SoIndexedLineSet	*lineSet = new SoIndexedLineSet;
  SoIndexedFaceSet	*faces = new SoIndexedFaceSet;
  std::vector<int>	lineIndices;
  std::vector<int>	indices;
  std::vector<SbVec3f>	corners;
  int			i = 0;
  float			epsilon = 0.0001;
  float			scale = cCrystalHitScale;

  ecut = settings.value ("igevents/cuts/ecal/barrel/rechits/energy", ecut).value<double> ();
  hlrMode = settings.value ("igdisplay/crystalhits/view3d/hiddenlineremoval", hlrMode).value<bool> ();

  if (hlrMode)
  {
    ann = new SoAnnotation;
    sep->addChild (ann);
    SoDrawStyle *dashed = new SoDrawStyle;
    dashed->style = SoDrawStyle::LINES;
    dashed->lineWidth = 1;
    dashed->linePattern = 0x0f0f;
    ann->addChild (dashed);
  }

  for (IgCollectionIterator ci = c->begin(), ce = c->end(); ci != ce; ++ci)
  {
    double energy = ci->get<double> ("energy");
    if (energy > ecut)
    {
      float eScale = scale * energy;

      IgV3d p1  = ci->get<IgV3d> ("front_1");
      IgV3d p2  = ci->get<IgV3d> ("front_2");
      IgV3d p3  = ci->get<IgV3d> ("front_3");
      IgV3d p4  = ci->get<IgV3d> ("front_4");
      IgV3d p5  = ci->get<IgV3d> ("back_1");
      IgV3d p6  = ci->get<IgV3d> ("back_2");
      IgV3d p7  = ci->get<IgV3d> ("back_3");
      IgV3d p8  = ci->get<IgV3d> ("back_4");

      SbVec3f front1(static_cast<double>(p1.x ()),
		     static_cast<double>(p1.y ()),
		     static_cast<double>(p1.z ()));

      SbVec3f front2(static_cast<double>(p2.x ()),
		     static_cast<double>(p2.y ()),
		     static_cast<double>(p2.z ()));

      SbVec3f front3(static_cast<double>(p3.x ()),
		     static_cast<double>(p3.y ()),
		     static_cast<double>(p3.z ()));

      SbVec3f front4(static_cast<double>(p4.x ()),
		     static_cast<double>(p4.y ()),
		     static_cast<double>(p4.z ()));

      SbVec3f back1(static_cast<double>(p5.x ()),
		    static_cast<double>(p5.y ()),
		    static_cast<double>(p5.z ()));

      SbVec3f back2(static_cast<double>(p6.x ()),
		    static_cast<double>(p6.y ()),
		    static_cast<double>(p6.z ()));

      SbVec3f back3(static_cast<double>(p7.x ()),
		    static_cast<double>(p7.y ()),
		    static_cast<double>(p7.z ()));

      SbVec3f back4(static_cast<double>(p8.x ()),
		    static_cast<double>(p8.y ()),
		    static_cast<double>(p8.z ()));

      SbVec3f eFront1 = front1.getValue () + (front3.getValue () - front1.getValue ());
      SbVec3f eFront2 = front2.getValue () + (front4.getValue () - front2.getValue ());
      SbVec3f eFront3 = front3.getValue () + (front1.getValue () - front3.getValue ());
      SbVec3f eFront4 = front4.getValue () + (front2.getValue () - front4.getValue ());

      SbVec3f eBack1  = back1.getValue ()  + (back3.getValue ()  - back1.getValue ());
      SbVec3f eBack2  = back2.getValue ()  + (back4.getValue ()  - back2.getValue ());
      SbVec3f eBack3  = back3.getValue ()  + (back1.getValue ()  - back3.getValue ());
      SbVec3f eBack4  = back4.getValue ()  + (back2.getValue ()  - back4.getValue ());

      corners.push_back (eBack1 + (eBack1 - eFront1) * epsilon);
      corners.push_back (eBack2 + (eBack2 - eFront2) * epsilon);
      corners.push_back (eBack3 + (eBack3 - eFront3) * epsilon);
      corners.push_back (eBack4 + (eBack4 - eFront4) * epsilon);
      corners.push_back (corners[i]  + (eBack1 - eFront1) * eScale);
      corners.push_back (corners[i + 1]  + (eBack2 - eFront2) * eScale);
      corners.push_back (corners[i + 2] + (eBack3 - eFront3) * eScale);
      corners.push_back (corners[i + 3] + (eBack4 - eFront4) * eScale);

      lineIndices.push_back (i);
      lineIndices.push_back (i + 1);
      lineIndices.push_back (i + 2);
      lineIndices.push_back (i + 3);
      lineIndices.push_back (i);
      lineIndices.push_back (SO_END_LINE_INDEX);

      lineIndices.push_back (i + 4);
      lineIndices.push_back (i + 5);
      lineIndices.push_back (i + 6);
      lineIndices.push_back (i + 7);
      lineIndices.push_back (i + 4);
      lineIndices.push_back (SO_END_LINE_INDEX);

      lineIndices.push_back (i);
      lineIndices.push_back (i + 4);
      lineIndices.push_back (SO_END_LINE_INDEX);
      lineIndices.push_back (i + 1);
      lineIndices.push_back (i + 5);
      lineIndices.push_back (SO_END_LINE_INDEX);
      lineIndices.push_back (i + 2);
      lineIndices.push_back (i + 6);
      lineIndices.push_back (SO_END_LINE_INDEX);
      lineIndices.push_back (i + 3);
      lineIndices.push_back (i + 7);
      lineIndices.push_back (SO_END_LINE_INDEX);

      // Face set indices
      indices.push_back (i + 3);
      indices.push_back (i + 2);
      indices.push_back (i + 1);
      indices.push_back (i + 0);
      indices.push_back (SO_END_FACE_INDEX);

      indices.push_back (i + 4);
      indices.push_back (i + 5);
      indices.push_back (i + 6);
      indices.push_back (i + 7);
      indices.push_back (SO_END_FACE_INDEX);

      indices.push_back (i + 5);
      indices.push_back (i + 1);
      indices.push_back (i + 2);
      indices.push_back (i + 6);
      indices.push_back (SO_END_FACE_INDEX);

      indices.push_back (i + 2);
      indices.push_back (i + 3);
      indices.push_back (i + 7);
      indices.push_back (i + 6);
      indices.push_back (SO_END_FACE_INDEX);

      indices.push_back (i + 7);
      indices.push_back (i + 3);
      indices.push_back (i);
      indices.push_back (i + 4);
      indices.push_back (SO_END_FACE_INDEX);

      indices.push_back (i + 1);
      indices.push_back (i + 5);
      indices.push_back (i + 4);
      indices.push_back (i);
      indices.push_back (SO_END_FACE_INDEX); // end of crystal vertices: 6*5

      i += 8;
    }
  }

  vertices->vertex.setValues (0, corners.size (), &corners [0]);
  vertices->vertex.setNum (corners.size ());

  lineSet->coordIndex.setValues (0, lineIndices.size (), &lineIndices [0]);
  lineSet->vertexProperty = vertices;

  faces->coordIndex.setValues (0, indices.size (), &indices [0]);
  faces->vertexProperty = vertices;

  if (hlrMode)
    ann->addChild (lineSet);

  sep->addChild (lineSet);
  sep->addChild (faces);
}

static void
make3DJetShapes(IgCollection **collections, IgAssociationSet **, SoSeparator *sep)
{
  IgCollection		*c = collections[0];
  double		ecut = 0.1;

  ecut = QSettings().value ("igevents/cuts/jets/energy", ecut).value<double> ();

  for (IgCollectionIterator ci = c->begin(), ce = c->end(); ci != ce; ++ci)
  {
    double energy = ci->get<double> ("et");
    if (energy > ecut)
    {
      IgSoJet *recoJet = new IgSoJet;
      recoJet->theta.setValue (ci->get<double> ("theta"));
      recoJet->phi.setValue (ci->get<double> ("phi"));
      recoJet->energy.setValue (energy);
      sep->addChild (recoJet);
    }
  }
}

static void
make3DEmTowerShapes(IgCollection **collections, IgAssociationSet **, SoSeparator *sep)
{
  IgCollection		*c = collections[0];
  double		ecut = 0.1;
  int			i = 0;
  SoVertexProperty	*vertices = new SoVertexProperty;
  SoIndexedLineSet	*lineSet = new SoIndexedLineSet;
  SoIndexedFaceSet	*faces = new SoIndexedFaceSet;
  std::vector<int>	lineIndices;
  std::vector<int>	indices;
  std::vector<SbVec3f>	corners;
  float			epsilon = 0.0001;
  float			scale = cCaloTowerScale;

  ecut = QSettings().value ("igevents/cuts/calotowers/energy", ecut).value<double> ();

  for (IgCollectionIterator ci = c->begin(), ce = c->end(); ci != ce; ++ci)
  {
    double energy = ci->get<double> ("emEnergy");
    if (energy > ecut)
    {
      float eScale = scale * energy;

      IgV3d p1  = ci->get<IgV3d> ("front_1");
      IgV3d p2  = ci->get<IgV3d> ("front_2");
      IgV3d p3  = ci->get<IgV3d> ("front_3");
      IgV3d p4  = ci->get<IgV3d> ("front_4");
      IgV3d p5  = ci->get<IgV3d> ("back_1");
      IgV3d p6  = ci->get<IgV3d> ("back_2");
      IgV3d p7  = ci->get<IgV3d> ("back_3");
      IgV3d p8  = ci->get<IgV3d> ("back_4");

      SbVec3f front1(static_cast<double>(p1.x ()),
		     static_cast<double>(p1.y ()),
		     static_cast<double>(p1.z ()));

      SbVec3f front2(static_cast<double>(p2.x ()),
		     static_cast<double>(p2.y ()),
		     static_cast<double>(p2.z ()));

      SbVec3f front3(static_cast<double>(p3.x ()),
		     static_cast<double>(p3.y ()),
		     static_cast<double>(p3.z ()));

      SbVec3f front4(static_cast<double>(p4.x ()),
		     static_cast<double>(p4.y ()),
		     static_cast<double>(p4.z ()));

      SbVec3f back1(static_cast<double>(p5.x ()),
		    static_cast<double>(p5.y ()),
		    static_cast<double>(p5.z ()));

      SbVec3f back2(static_cast<double>(p6.x ()),
		    static_cast<double>(p6.y ()),
		    static_cast<double>(p6.z ()));

      SbVec3f back3(static_cast<double>(p7.x ()),
		    static_cast<double>(p7.y ()),
		    static_cast<double>(p7.z ()));

      SbVec3f back4(static_cast<double>(p8.x ()),
		    static_cast<double>(p8.y ()),
		    static_cast<double>(p8.z ()));

      SbVec3f eFront1 = front1.getValue () + (front3.getValue () - front1.getValue ());
      SbVec3f eFront2 = front2.getValue () + (front4.getValue () - front2.getValue ());
      SbVec3f eFront3 = front3.getValue () + (front1.getValue () - front3.getValue ());
      SbVec3f eFront4 = front4.getValue () + (front2.getValue () - front4.getValue ());

      SbVec3f eBack1  = back1.getValue ()  + (back3.getValue ()  - back1.getValue ());
      SbVec3f eBack2  = back2.getValue ()  + (back4.getValue ()  - back2.getValue ());
      SbVec3f eBack3  = back3.getValue ()  + (back1.getValue ()  - back3.getValue ());
      SbVec3f eBack4  = back4.getValue ()  + (back2.getValue ()  - back4.getValue ());

      corners.push_back (eBack1 + (eBack1 - eFront1) * epsilon);
      corners.push_back (eBack2 + (eBack2 - eFront2) * epsilon);
      corners.push_back (eBack3 + (eBack3 - eFront3) * epsilon);
      corners.push_back (eBack4 + (eBack4 - eFront4) * epsilon);
      corners.push_back (corners[i]  + (eBack1 - eFront1) * eScale);
      corners.push_back (corners[i + 1]  + (eBack2 - eFront2) * eScale);
      corners.push_back (corners[i + 2] + (eBack3 - eFront3) * eScale);
      corners.push_back (corners[i + 3] + (eBack4 - eFront4) * eScale);

      lineIndices.push_back (i);
      lineIndices.push_back (i + 1);
      lineIndices.push_back (i + 2);
      lineIndices.push_back (i + 3);
      lineIndices.push_back (i);
      lineIndices.push_back (SO_END_LINE_INDEX);

      lineIndices.push_back (i + 4);
      lineIndices.push_back (i + 5);
      lineIndices.push_back (i + 6);
      lineIndices.push_back (i + 7);
      lineIndices.push_back (i + 4);
      lineIndices.push_back (SO_END_LINE_INDEX);

      lineIndices.push_back (i);
      lineIndices.push_back (i + 4);
      lineIndices.push_back (SO_END_LINE_INDEX);
      lineIndices.push_back (i + 1);
      lineIndices.push_back (i + 5);
      lineIndices.push_back (SO_END_LINE_INDEX);
      lineIndices.push_back (i + 2);
      lineIndices.push_back (i + 6);
      lineIndices.push_back (SO_END_LINE_INDEX);
      lineIndices.push_back (i + 3);
      lineIndices.push_back (i + 7);
      lineIndices.push_back (SO_END_LINE_INDEX);

      // Face set indices
      indices.push_back (i + 3);
      indices.push_back (i + 2);
      indices.push_back (i + 1);
      indices.push_back (i + 0);
      indices.push_back (SO_END_FACE_INDEX);

      indices.push_back (i + 4);
      indices.push_back (i + 5);
      indices.push_back (i + 6);
      indices.push_back (i + 7);
      indices.push_back (SO_END_FACE_INDEX);

      indices.push_back (i + 5);
      indices.push_back (i + 1);
      indices.push_back (i + 2);
      indices.push_back (i + 6);
      indices.push_back (SO_END_FACE_INDEX);

      indices.push_back (i + 2);
      indices.push_back (i + 3);
      indices.push_back (i + 7);
      indices.push_back (i + 6);
      indices.push_back (SO_END_FACE_INDEX);

      indices.push_back (i + 7);
      indices.push_back (i + 3);
      indices.push_back (i);
      indices.push_back (i + 4);
      indices.push_back (SO_END_FACE_INDEX);

      indices.push_back (i + 1);
      indices.push_back (i + 5);
      indices.push_back (i + 4);
      indices.push_back (i);
      indices.push_back (SO_END_FACE_INDEX); // end of crystal vertices: 6*5

      i += 8;
    }
  }

  vertices->vertex.setValues (0, corners.size (), &corners [0]);
  vertices->vertex.setNum (corners.size ());

  lineSet->coordIndex.setValues (0, lineIndices.size (), &lineIndices [0]);
  lineSet->vertexProperty = vertices;

  faces->coordIndex.setValues (0, indices.size (), &indices [0]);
  faces->vertexProperty = vertices;

  sep->addChild (lineSet);
  sep->addChild (faces);
}

static void
make3DHadTowerShapes(IgCollection **collections, IgAssociationSet **, SoSeparator *sep)
{
  IgCollection		*c = collections[0];
  double		ecut = 0.1;
  bool			hlrMode = false;
  int			i = 0;
  QSettings		settings;
  SoAnnotation		*ann = 0;
  SoVertexProperty	*vertices = new SoVertexProperty;
  SoIndexedLineSet	*lineSet = new SoIndexedLineSet;
  SoIndexedFaceSet	*faces = new SoIndexedFaceSet;
  std::vector<int>	lineIndices;
  std::vector<int>	indices;
  std::vector<SbVec3f>	corners;
  float			epsilon = 0.0001;
  float			scale = cCaloTowerScale;

  ecut = settings.value ("igevents/cuts/calotowers/energy", ecut).value<double> ();
  hlrMode = settings.value ("igdisplay/hadtowers/view3d/hiddenlineremoval", hlrMode).value<bool> ();

  if (hlrMode)
  {
    ann = new SoAnnotation;
    sep->addChild (ann);
    SoDrawStyle *dashed = new SoDrawStyle;
    dashed->style = SoDrawStyle::LINES;
    dashed->lineWidth.setValue (1);
    dashed->linePattern.setValue (0x0f0f);
    ann->addChild (dashed);
  }

  for (IgCollectionIterator ci = c->begin(), ce = c->end(); ci != ce; ++ci)
  {
    double energy = ci->get<double> ("emEnergy") + ci->get<double> ("hadEnergy");
    if (energy > ecut)
    {
      float eScale = scale * energy;

      IgV3d p1  = ci->get<IgV3d> ("front_1");
      IgV3d p2  = ci->get<IgV3d> ("front_2");
      IgV3d p3  = ci->get<IgV3d> ("front_3");
      IgV3d p4  = ci->get<IgV3d> ("front_4");
      IgV3d p5  = ci->get<IgV3d> ("back_1");
      IgV3d p6  = ci->get<IgV3d> ("back_2");
      IgV3d p7  = ci->get<IgV3d> ("back_3");
      IgV3d p8  = ci->get<IgV3d> ("back_4");

      SbVec3f front1(static_cast<double>(p1.x ()),
		     static_cast<double>(p1.y ()),
		     static_cast<double>(p1.z ()));

      SbVec3f front2(static_cast<double>(p2.x ()),
		     static_cast<double>(p2.y ()),
		     static_cast<double>(p2.z ()));

      SbVec3f front3(static_cast<double>(p3.x ()),
		     static_cast<double>(p3.y ()),
		     static_cast<double>(p3.z ()));

      SbVec3f front4(static_cast<double>(p4.x ()),
		     static_cast<double>(p4.y ()),
		     static_cast<double>(p4.z ()));

      SbVec3f back1(static_cast<double>(p5.x ()),
		    static_cast<double>(p5.y ()),
		    static_cast<double>(p5.z ()));

      SbVec3f back2(static_cast<double>(p6.x ()),
		    static_cast<double>(p6.y ()),
		    static_cast<double>(p6.z ()));

      SbVec3f back3(static_cast<double>(p7.x ()),
		    static_cast<double>(p7.y ()),
		    static_cast<double>(p7.z ()));

      SbVec3f back4(static_cast<double>(p8.x ()),
		    static_cast<double>(p8.y ()),
		    static_cast<double>(p8.z ()));

      SbVec3f eFront1 = front1.getValue () + (front3.getValue () - front1.getValue ());
      SbVec3f eFront2 = front2.getValue () + (front4.getValue () - front2.getValue ());
      SbVec3f eFront3 = front3.getValue () + (front1.getValue () - front3.getValue ());
      SbVec3f eFront4 = front4.getValue () + (front2.getValue () - front4.getValue ());

      SbVec3f eBack1  = back1.getValue ()  + (back3.getValue ()  - back1.getValue ());
      SbVec3f eBack2  = back2.getValue ()  + (back4.getValue ()  - back2.getValue ());
      SbVec3f eBack3  = back3.getValue ()  + (back1.getValue ()  - back3.getValue ());
      SbVec3f eBack4  = back4.getValue ()  + (back2.getValue ()  - back4.getValue ());

      corners.push_back (eBack1 + (eBack1 - eFront1) * epsilon);
      corners.push_back (eBack2 + (eBack2 - eFront2) * epsilon);
      corners.push_back (eBack3 + (eBack3 - eFront3) * epsilon);
      corners.push_back (eBack4 + (eBack4 - eFront4) * epsilon);
      corners.push_back (corners[i]  + (eBack1 - eFront1) * eScale);
      corners.push_back (corners[i + 1]  + (eBack2 - eFront2) * eScale);
      corners.push_back (corners[i + 2] + (eBack3 - eFront3) * eScale);
      corners.push_back (corners[i + 3] + (eBack4 - eFront4) * eScale);

      lineIndices.push_back (i);
      lineIndices.push_back (i + 1);
      lineIndices.push_back (i + 2);
      lineIndices.push_back (i + 3);
      lineIndices.push_back (i);
      lineIndices.push_back (SO_END_LINE_INDEX);

      lineIndices.push_back (i + 4);
      lineIndices.push_back (i + 5);
      lineIndices.push_back (i + 6);
      lineIndices.push_back (i + 7);
      lineIndices.push_back (i + 4);
      lineIndices.push_back (SO_END_LINE_INDEX);

      lineIndices.push_back (i);
      lineIndices.push_back (i + 4);
      lineIndices.push_back (SO_END_LINE_INDEX);
      lineIndices.push_back (i + 1);
      lineIndices.push_back (i + 5);
      lineIndices.push_back (SO_END_LINE_INDEX);
      lineIndices.push_back (i + 2);
      lineIndices.push_back (i + 6);
      lineIndices.push_back (SO_END_LINE_INDEX);
      lineIndices.push_back (i + 3);
      lineIndices.push_back (i + 7);
      lineIndices.push_back (SO_END_LINE_INDEX);

      // Face set indices
      indices.push_back (i + 3);
      indices.push_back (i + 2);
      indices.push_back (i + 1);
      indices.push_back (i + 0);
      indices.push_back (SO_END_FACE_INDEX);

      indices.push_back (i + 4);
      indices.push_back (i + 5);
      indices.push_back (i + 6);
      indices.push_back (i + 7);
      indices.push_back (SO_END_FACE_INDEX);

      indices.push_back (i + 5);
      indices.push_back (i + 1);
      indices.push_back (i + 2);
      indices.push_back (i + 6);
      indices.push_back (SO_END_FACE_INDEX);

      indices.push_back (i + 2);
      indices.push_back (i + 3);
      indices.push_back (i + 7);
      indices.push_back (i + 6);
      indices.push_back (SO_END_FACE_INDEX);

      indices.push_back (i + 7);
      indices.push_back (i + 3);
      indices.push_back (i);
      indices.push_back (i + 4);
      indices.push_back (SO_END_FACE_INDEX);

      indices.push_back (i + 1);
      indices.push_back (i + 5);
      indices.push_back (i + 4);
      indices.push_back (i);
      indices.push_back (SO_END_FACE_INDEX); // end of crystal vertices: 6*5

      i += 8;
    }
  }

  vertices->vertex.setValues (0, corners.size (), &corners [0]);
  vertices->vertex.setNum (corners.size ());

  lineSet->coordIndex.setValues (0, lineIndices.size (), &lineIndices [0]);
  lineSet->vertexProperty = vertices;

  faces->coordIndex.setValues (0, indices.size (), &indices [0]);
  faces->vertexProperty = vertices;

  if (hlrMode)
    ann->addChild (lineSet);

  sep->addChild (lineSet);
  sep->addChild (faces);
}

#if 0
static void
make3DCrystalShapes(IgCollection **collections, IgAssociationSet **, SoSeparator *sep)
{
  IgCollection		*c = collections[0];
  bool			hlrMode = false;
  SoAnnotation		*ann = 0;
  SoVertexProperty	*vertices = new SoVertexProperty;
  SoIndexedLineSet	*lineSet = new SoIndexedLineSet;
  SoIndexedFaceSet	*faces = new SoIndexedFaceSet;
  std::vector<int>	lineIndices;
  std::vector<int>	indices;
  std::vector<SbVec3f>	corners;
  int			i = 0;

  hlrMode = QSettings().value ("igdisplay/crystals/view3d/hiddenlineremoval").value<bool> ();

  if (hlrMode)
  {
    ann = new SoAnnotation;
    sep->addChild (ann);
    SoDrawStyle *dashed = new SoDrawStyle;
    dashed->style = SoDrawStyle::LINES;
    dashed->lineWidth = 1;
    dashed->linePattern = 0x0f0f;
    ann->addChild (dashed);
  }

  for (IgCollectionIterator ci = c->begin(), ce = c->end(); ci != ce; ++ci)
  {
    IgV3d p1  = ci->get<IgV3d> ("front_1");
    IgV3d p2  = ci->get<IgV3d> ("front_2");
    IgV3d p3  = ci->get<IgV3d> ("front_3");
    IgV3d p4  = ci->get<IgV3d> ("front_4");
    IgV3d p5  = ci->get<IgV3d> ("back_1");
    IgV3d p6  = ci->get<IgV3d> ("back_2");
    IgV3d p7  = ci->get<IgV3d> ("back_3");
    IgV3d p8  = ci->get<IgV3d> ("back_4");

    SbVec3f front1(static_cast<double>(p1.x ()),
		   static_cast<double>(p1.y ()),
		   static_cast<double>(p1.z ()));
    corners.push_back (front1);

    SbVec3f front2(static_cast<double>(p2.x ()),
		   static_cast<double>(p2.y ()),
		   static_cast<double>(p2.z ()));
    corners.push_back (front2);

    SbVec3f front3(static_cast<double>(p3.x ()),
		   static_cast<double>(p3.y ()),
		   static_cast<double>(p3.z ()));
    corners.push_back (front3);

    SbVec3f front4(static_cast<double>(p4.x ()),
		   static_cast<double>(p4.y ()),
		   static_cast<double>(p4.z ()));
    corners.push_back (front4);

    SbVec3f back1(static_cast<double>(p5.x ()),
		  static_cast<double>(p5.y ()),
		  static_cast<double>(p5.z ()));
    corners.push_back (back1);

    SbVec3f back2(static_cast<double>(p6.x ()),
		  static_cast<double>(p6.y ()),
		  static_cast<double>(p6.z ()));
    corners.push_back (back2);

    SbVec3f back3(static_cast<double>(p7.x ()),
		  static_cast<double>(p7.y ()),
		  static_cast<double>(p7.z ()));
    corners.push_back (back3);

    SbVec3f back4(static_cast<double>(p8.x ()),
		  static_cast<double>(p8.y ()),
		  static_cast<double>(p8.z ()));
    corners.push_back (back4);

    lineIndices.push_back (i);
    lineIndices.push_back (i + 1);
    lineIndices.push_back (i + 2);
    lineIndices.push_back (i + 3);
    lineIndices.push_back (i);
    lineIndices.push_back (SO_END_LINE_INDEX);

    lineIndices.push_back (i + 4);
    lineIndices.push_back (i + 5);
    lineIndices.push_back (i + 6);
    lineIndices.push_back (i + 7);
    lineIndices.push_back (i + 4);
    lineIndices.push_back (SO_END_LINE_INDEX);

    lineIndices.push_back (i);
    lineIndices.push_back (i + 4);
    lineIndices.push_back (SO_END_LINE_INDEX);
    lineIndices.push_back (i + 1);
    lineIndices.push_back (i + 5);
    lineIndices.push_back (SO_END_LINE_INDEX);
    lineIndices.push_back (i + 2);
    lineIndices.push_back (i + 6);
    lineIndices.push_back (SO_END_LINE_INDEX);
    lineIndices.push_back (i + 3);
    lineIndices.push_back (i + 7);
    lineIndices.push_back (SO_END_LINE_INDEX);

    // Face set indices
    indices.push_back (i + 3);
    indices.push_back (i + 2);
    indices.push_back (i + 1);
    indices.push_back (i + 0);
    indices.push_back (SO_END_FACE_INDEX);

    indices.push_back (i + 4);
    indices.push_back (i + 5);
    indices.push_back (i + 6);
    indices.push_back (i + 7);
    indices.push_back (SO_END_FACE_INDEX);

    indices.push_back (i + 5);
    indices.push_back (i + 1);
    indices.push_back (i + 2);
    indices.push_back (i + 6);
    indices.push_back (SO_END_FACE_INDEX);

    indices.push_back (i + 2);
    indices.push_back (i + 3);
    indices.push_back (i + 7);
    indices.push_back (i + 6);
    indices.push_back (SO_END_FACE_INDEX);

    indices.push_back (i + 7);
    indices.push_back (i + 3);
    indices.push_back (i);
    indices.push_back (i + 4);
    indices.push_back (SO_END_FACE_INDEX);

    indices.push_back (i + 1);
    indices.push_back (i + 5);
    indices.push_back (i + 4);
    indices.push_back (i);
    indices.push_back (SO_END_FACE_INDEX); // end of crystal vertices: 6*5

    i += 8;
  }

  vertices->vertex.setValues (0, corners.size (), &corners [0]);
  vertices->vertex.setNum (corners.size ());

  lineSet->coordIndex.setValues (0, lineIndices.size (), &lineIndices [0]);
  lineSet->vertexProperty = vertices;

  faces->coordIndex.setValues (0, indices.size (), &indices [0]);
  faces->vertexProperty = vertices;

  if (hlrMode)
    ann->addChild (lineSet);

  sep->addChild (lineSet);
  sep->addChild (faces);
}
#endif

static void
make3DSegmentShapes(IgCollection **collections, IgAssociationSet **, SoSeparator *sep)
{
  IgCollection		*c = collections[0];
  SoVertexProperty	*vertices = new SoVertexProperty;
  SoIndexedLineSet	*lineSet = new SoIndexedLineSet;
  std::vector<int>	lineIndices;
  std::vector<SbVec3f>	points;
  int			i = 0;

  for (IgCollectionIterator ci = c->begin(), ce = c->end(); ci != ce; ++ci)
  {
    IgV3d p1 = ci->get<IgV3d> ("pos_1");
    IgV3d p2 = ci->get<IgV3d> ("pos_2");

    points.push_back (SbVec3f (static_cast<double>(p1.x ()),
			       static_cast<double>(p1.y ()),
			       static_cast<double>(p1.z ())));
    points.push_back (SbVec3f (static_cast<double>(p2.x ()),
			       static_cast<double>(p2.y ()),
			       static_cast<double>(p2.z ())));
    lineIndices.push_back (i);
    lineIndices.push_back (i + 1);
    lineIndices.push_back (SO_END_LINE_INDEX);
    i += 2;
  }

  vertices->vertex.setValues (0, points.size (), &points [0]);
  vertices->vertex.setNum (points.size ());

  lineSet->coordIndex.setValues (0, lineIndices.size (), &lineIndices [0]);
  lineSet->vertexProperty = vertices;

  sep->addChild (lineSet);
}

#if 0
static void
make3DBasicCluster(IgCollection **collections, IgAssociationSet **assocs, SoSeparator *sep)
{
  IgCollection		*clusters = collections[0];
  IgCollection		*calohits = collections[1];
  IgAssociationSet	*assoc = assocs[0];
  IgProperty		POS = clusters->getProperty("pos");
  IgProperty		ENERGY = clusters->getProperty("energy");
  IgProperty		FRACTION = calohits->getProperty("fraction");
  IgProperty		F1 = calohits->getProperty("front_1");
  IgProperty		F2 = calohits->getProperty("front_2");
  IgProperty		F3 = calohits->getProperty("front_3");
  IgProperty		F4 = calohits->getProperty("front_4");
  IgProperty		B1 = calohits->getProperty("back_1");
  IgProperty		B2 = calohits->getProperty("back_2");
  IgProperty		B3 = calohits->getProperty("back_3");
  IgProperty		B4 = calohits->getProperty("back_4");
  SoMaterial		*mat = new SoMaterial;

  mat->diffuseColor = SbColor(0xB0/255., 0xE5/255., 0x7C/255.);
  sep->addChild (mat);

  for (IgCollectionIterator ci = clusters->begin(), ce = clusters->end(); ci != ce; ++ci)
  {
    SoVertexProperty *vertices = new SoVertexProperty;
    SoMarkerSet *points = new SoMarkerSet;
    IgV3d p1 = ci->get<IgV3d> (POS);
    int n = 0;

    double x = p1.x ();
    double y = p1.y ();
    double z = p1.z ();
    vertices->vertex.set1Value (n, SbVec3f (x, y, z));

    double energy = ci->get<double>(ENERGY);

    for (IgAssociationSet::Iterator ai = assoc->begin(), ae = assoc->end(); ai != ae; ++ai)
    {
      if ( ai->first().objectId() == ci->currentRow())
      {
	IgCollectionItem c(calohits, ai->second().objectId());

	IgV3d f1 = c.get<IgV3d>(F1);
	IgV3d f2 = c.get<IgV3d>(F2);
	IgV3d f3 = c.get<IgV3d>(F3);
	IgV3d f4 = c.get<IgV3d>(F4);

	IgV3d b1 = c.get<IgV3d>(B1);
	IgV3d b2 = c.get<IgV3d>(B2);
	IgV3d b3 = c.get<IgV3d>(B3);
	IgV3d b4 = c.get<IgV3d>(B4);

	IgSoCrystalHit *crystalHit = new IgSoCrystalHit;
	crystalHit->energy.setValue(energy);
	crystalHit->scale.setValue(1.0);
	crystalHit->relativeWidth.setValue(1.0);
	crystalHit->drawCrystal.setValue(true);
	crystalHit->drawHit.setValue(true);

	crystalHit->front1.setValue(f1.x(), f1.y(), f1.z());
	crystalHit->front2.setValue(f2.x(), f2.y(), f2.z());
	crystalHit->front3.setValue(f3.x(), f3.y(), f3.z());
	crystalHit->front4.setValue(f4.x(), f4.y(), f4.z());

	crystalHit->back1.setValue(f1.x(), f1.y(), f1.z());
	crystalHit->back2.setValue(f2.x(), f2.y(), f2.z());
	crystalHit->back3.setValue(f3.x(), f3.y(), f3.z());
	crystalHit->back4.setValue(f4.x(), f4.y(), f4.z());

	sep->addChild(crystalHit);
      }
    }

    vertices->vertex.setNum (n);
    points->markerIndex = SoMarkerSet::PLUS_5_5;
    points->vertexProperty = vertices;
    points->numPoints = n;
    sep->addChild (points);
  }
}
#endif

static void
make3DCSCSegments(IgCollection **collections, IgAssociationSet **assocs, SoSeparator *sep)
{
  SoMaterial	*mat = new SoMaterial;
  SoDrawStyle	*sty = new SoDrawStyle;

  mat->diffuseColor = SbColor (0xC0/255., 0, 0);
  sep->addChild (mat);

  sty->style = SoDrawStyle::LINES;
  sty->lineWidth = 3;
  sep->addChild (sty);

  make3DSegmentShapes(collections, assocs, sep);
}

static void
make3DCaloTowers(IgCollection **collections, IgAssociationSet **assocs, SoSeparator *sep)
{
  SoMaterial	*emat = new SoMaterial;
  SoMaterial	*hmat = new SoMaterial;

  emat->diffuseColor = SbColor (0xB0/255., 0x50/255., 0);
  sep->addChild (emat);
  make3DEmTowerShapes(collections, assocs, sep);

  hmat->diffuseColor = SbColor (0, 0xE5/255., 0x7C/255.);
  sep->addChild (hmat);
  make3DHadTowerShapes(collections, assocs, sep);
}

static void
make3DDTDigis(IgCollection **collections, IgAssociationSet **, SoSeparator *sep)
{
  IgCollection		*c = collections[0];
  IgProperty		POS = c->getProperty("pos");
  IgProperty		AXIS = c->getProperty("axis");
  IgProperty		ANGLE = c->getProperty("angle");
  IgProperty		CELL_L = c->getProperty("cellLength");
  IgProperty		CELL_W = c->getProperty("cellWidth");
  IgProperty		CELL_H = c->getProperty("cellHeight");
  SoVertexProperty	*vertices = new SoVertexProperty;
  SoMarkerSet		*points = new SoMarkerSet;
  SoMaterial		*mat = new SoMaterial;
  int			n = 0;

  mat->diffuseColor = SbColor (0x66/255., 0xff/255., 0x00/255.);
  sep->addChild (mat);

  for (IgCollectionIterator ci = c->begin(), ce = c->end(); ci != ce; ++ci)
  {
    IgV3d pos = ci->get<IgV3d>(POS);

    double x = pos.x();
    double y = pos.y();
    double z = pos.z();
    vertices->vertex.set1Value(n++, SbVec3f (x, y, z));

    IgV3d axis = ci->get<IgV3d>(AXIS);
    double angle = ci->get<double>(ANGLE);

    SoTransform *transform = new SoTransform;
    transform->translation.setValue(x,y,z);
    transform->rotation.setValue(SbVec3f(axis.x(),axis.y(),axis.z()), angle);

    SoCube *cube = new SoCube;
    cube->width = ci->get<double>(CELL_W);
    cube->height = ci->get<double>(CELL_L);
    cube->depth = ci->get<double>(CELL_H);

    SoSeparator *separator = new SoSeparator;
    separator->addChild(transform);
    separator->addChild(cube);
    sep->addChild(separator);
  }

  vertices->vertex.setNum(n);
  points->markerIndex = SoMarkerSet::SQUARE_LINE_7_7;
  points->vertexProperty = vertices;
  points->numPoints = n;
  sep->addChild (points);
}

static void
make3DDTRecHits(IgCollection **collections, IgAssociationSet **, SoSeparator *sep)
{
  IgCollection		*c = collections[0];
  IgProperty		LPLUS_GLOBALPOS = c->getProperty("lPlusGlobalPos");
  IgProperty		LMINUS_GLOBALPOS = c->getProperty("lMinusGlobalPos");
  IgProperty		RPLUS_GLOBALPOS = c->getProperty("rPlusGlobalPos");
  IgProperty		RMINUS_GLOBALPOS = c->getProperty("rMinusGlobalPos");
  IgProperty		LGLOBALPOS = c->getProperty("lGlobalPos");
  IgProperty		RGLOBALPOS = c->getProperty("rGlobalPos");
  IgProperty		WPOS = c->getProperty("wirePos");
  IgProperty		AXIS = c->getProperty("axis");
  IgProperty		ANGLE = c->getProperty("angle");
  IgProperty		CELL_L = c->getProperty("cellLength");
  IgProperty		CELL_W = c->getProperty("cellWidth");
  IgProperty		CELL_H = c->getProperty("cellHeight");
  SoVertexProperty	*vertices = new SoVertexProperty;
  SoMarkerSet		*points = new SoMarkerSet;
  SoDrawStyle		*wdrawStyle = new SoDrawStyle;
  int			n = 0;

  vertices->materialBinding = SoVertexProperty::OVERALL;
  vertices->orderedRGBA = 0x0000FFFF;

  wdrawStyle->style = SoDrawStyle::LINES;
  wdrawStyle->lineWidth.setValue(1.0);
  sep->addChild(wdrawStyle);

  for (IgCollectionIterator ci = c->begin(), ce = c->end(); ci != ce; ++ci)
  {
    IgV3d lPlusGlobalPos = ci->get<IgV3d>(LPLUS_GLOBALPOS);
    IgV3d lMinusGlobalPos = ci->get<IgV3d>(LMINUS_GLOBALPOS);
    IgV3d rPlusGlobalPos = ci->get<IgV3d>(RPLUS_GLOBALPOS);
    IgV3d rMinusGlobalPos = ci->get<IgV3d>(RMINUS_GLOBALPOS);
    IgV3d lGlobalPos = ci->get<IgV3d>(LGLOBALPOS);
    IgV3d rGlobalPos = ci->get<IgV3d>(RGLOBALPOS);

    SoLineSet *linel = new SoLineSet;
    linel->numVertices = 2;
    SoVertexProperty* vtxl = new SoVertexProperty;
    vtxl->vertex.set1Value(0, SbVec3f(lPlusGlobalPos.x(),
				      lPlusGlobalPos.y(),
				      lPlusGlobalPos.z()));

    vtxl->vertex.set1Value(1, SbVec3f(lMinusGlobalPos.x(),
				      lMinusGlobalPos.y(),
				      lMinusGlobalPos.z()));
    linel->vertexProperty = vtxl;

    SoLineSet *liner = new SoLineSet;
    liner->numVertices = 2;
    SoVertexProperty *vtxr = new SoVertexProperty;
    vtxr->vertex.set1Value(0, SbVec3f(rPlusGlobalPos.x(),
				      rPlusGlobalPos.y(),
				      rPlusGlobalPos.z()));
    vtxr->vertex.set1Value(1, SbVec3f(rMinusGlobalPos.x(),
				      rMinusGlobalPos.y(),
				      rMinusGlobalPos.z()));
    liner->vertexProperty = vtxr;

    sep->addChild(linel);
    sep->addChild(liner);

    vertices->vertex.set1Value(n++, SbVec3f(lGlobalPos.x(),
					    lGlobalPos.y(),
					    lGlobalPos.z()));

    vertices->vertex.set1Value(n++, SbVec3f(rGlobalPos.x(),
					    rGlobalPos.y(),
					    rGlobalPos.z()));

    IgV3d pos = ci->get<IgV3d>(WPOS);
    IgV3d axis = ci->get<IgV3d>(AXIS);
    double angle = ci->get<double>(ANGLE);
    SoTransform *transform = new SoTransform;
    transform->translation.setValue(pos.x(),pos.y(),pos.z());
    transform->rotation.setValue(SbVec3f(axis.x(),axis.y(),axis.z()), angle);

    SoCube *cube = new SoCube;
    cube->width = ci->get<double>(CELL_W);
    cube->height = ci->get<double>(CELL_L);
    cube->depth = ci->get<double>(CELL_H);

    SoSeparator *separator = new SoSeparator;
    separator->addChild(transform);
    separator->addChild(cube);
    sep->addChild(separator);
  }

  points->markerIndex = SoMarkerSet::PLUS_7_7;
  points->vertexProperty = vertices;
  points->numPoints = n;
  sep->addChild (points);
}

static void
make3DDTRecSegment4D(IgCollection **collections, IgAssociationSet **assocs, SoSeparator *sep)
{
  SoMaterial	*mat = new SoMaterial;
  SoDrawStyle	*sty = new SoDrawStyle;

  mat->diffuseColor = SbColor (0xC0/255., 0x00/255., 0x00/255.);
  sep->addChild (mat);

  sty->style = SoDrawStyle::LINES;
  sty->lineWidth.setValue (3);
  sep->addChild (sty);

  make3DSegmentShapes(collections, assocs, sep);
}

static void
make3DDetector(IgCollection **collections, IgAssociationSet **assocs, SoSeparator *sep)
{
  IgCollection		*obj = collections[0];
  IgCollection		*geom = collections[1];
  IgAssociationSet	*assoc = assocs[0];
  IgProperty		SHAPE = obj->getProperty("shape");
  IgProperty		F1 = geom->getProperty("front_1");
  IgProperty		F2 = geom->getProperty("front_2");
  IgProperty		F3 = geom->getProperty("front_3");
  IgProperty		F4 = geom->getProperty("front_4");
  IgProperty		B1 = geom->getProperty("back_1");
  IgProperty		B2 = geom->getProperty("back_2");
  IgProperty		B3 = geom->getProperty("back_3");
  IgProperty		B4 = geom->getProperty("back_4");
  IgProperty		POS1 = geom->getProperty("pos_1");
  IgProperty		POS2 = geom->getProperty("pos_2");
  IgProperty		POS = geom->getProperty("pos");
  SoVertexProperty	*vertices = new SoVertexProperty;
  std::vector<int>	lineIndices;
  std::vector<SbVec3f>	corners;
  int			i = 0;

  for (IgCollectionIterator ci = obj->begin(), ce = obj->end(); ci != ce; ++ci)
  {
    std::string shape = ci->get<std::string>(SHAPE);
    for (IgAssociationSet::Iterator ai = assoc->begin(), ae = assoc->end(); ai != ae; ++ai)
    {
      if (ai->first().objectId() == ci->currentRow())
      {
	IgCollectionItem m(geom, ai->second().objectId());
	if (shape == "box")
	{
	  IgV3d p1  = m.get<IgV3d>(F1);
	  IgV3d p2  = m.get<IgV3d>(F2);
	  IgV3d p3  = m.get<IgV3d>(F3);
	  IgV3d p4  = m.get<IgV3d>(F4);
	  IgV3d p5  = m.get<IgV3d>(B1);
	  IgV3d p6  = m.get<IgV3d>(B2);
	  IgV3d p7  = m.get<IgV3d>(B3);
	  IgV3d p8  = m.get<IgV3d>(B4);

	  corners.push_back (SbVec3f (static_cast<double>(p1.x ()),
				      static_cast<double>(p1.y ()),
				      static_cast<double>(p1.z ())));
	  corners.push_back (SbVec3f (static_cast<double>(p2.x ()),
				      static_cast<double>(p2.y ()),
				      static_cast<double>(p2.z ())));
	  corners.push_back (SbVec3f (static_cast<double>(p3.x ()),
				      static_cast<double>(p3.y ()),
				      static_cast<double>(p3.z ())));
	  corners.push_back (SbVec3f (static_cast<double>(p4.x ()),
				      static_cast<double>(p4.y ()),
				      static_cast<double>(p4.z ())));
	  corners.push_back (SbVec3f (static_cast<double>(p5.x ()),
				      static_cast<double>(p5.y ()),
				      static_cast<double>(p5.z ())));
	  corners.push_back (SbVec3f (static_cast<double>(p6.x ()),
				      static_cast<double>(p6.y ()),
				      static_cast<double>(p6.z ())));
	  corners.push_back (SbVec3f (static_cast<double>(p7.x ()),
				      static_cast<double>(p7.y ()),
				      static_cast<double>(p7.z ())));
	  corners.push_back (SbVec3f (static_cast<double>(p8.x ()),
				      static_cast<double>(p8.y ()),
				      static_cast<double>(p8.z ())));

	  lineIndices.push_back (i);
	  lineIndices.push_back (i + 1);
	  lineIndices.push_back (i + 2);
	  lineIndices.push_back (i + 3);
	  lineIndices.push_back (i);
	  lineIndices.push_back (SO_END_LINE_INDEX);

	  lineIndices.push_back (i + 4);
	  lineIndices.push_back (i + 5);
	  lineIndices.push_back (i + 6);
	  lineIndices.push_back (i + 7);
	  lineIndices.push_back (i + 4);
	  lineIndices.push_back (SO_END_LINE_INDEX);

	  lineIndices.push_back (i);
	  lineIndices.push_back (i + 4);
	  lineIndices.push_back (SO_END_LINE_INDEX);
	  lineIndices.push_back (i + 1);
	  lineIndices.push_back (i + 5);
	  lineIndices.push_back (SO_END_LINE_INDEX);
	  lineIndices.push_back (i + 2);
	  lineIndices.push_back (i + 6);
	  lineIndices.push_back (SO_END_LINE_INDEX);
	  lineIndices.push_back (i + 3);
	  lineIndices.push_back (i + 7);
	  lineIndices.push_back (SO_END_LINE_INDEX);

	  i += 8;
	}
	else if (shape == "point")
	{
	  IgV3d p1 = m.get<IgV3d>(POS);
	  corners.push_back (SbVec3f (static_cast<double>(p1.x ()),
				      static_cast<double>(p1.y ()),
				      static_cast<double>(p1.z ())));
	}
	else if (shape == "line")
	{
	  IgV3d p1 = m.get<IgV3d>(POS1);
	  IgV3d p2 = m.get<IgV3d>(POS2);
	  corners.push_back (SbVec3f (static_cast<double>(p1.x ()),
				      static_cast<double>(p1.y ()),
				      static_cast<double>(p1.z ())));
	  corners.push_back (SbVec3f (static_cast<double>(p2.x ()),
				      static_cast<double>(p2.y ()),
				      static_cast<double>(p2.z ())));
	  lineIndices.push_back (i);
	  lineIndices.push_back (i + 1);
	  lineIndices.push_back (SO_END_LINE_INDEX);
	  i += 2;
	}

	vertices->vertex.setValues (0, corners.size (), &corners [0]);
	vertices->vertex.setNum (corners.size ());

	if (shape == "box" || shape == "line")
	{
	  SoIndexedLineSet *lineSet = new SoIndexedLineSet;
	  lineSet->coordIndex.setValues (0, lineIndices.size (), &lineIndices [0]);
	  lineSet->vertexProperty = vertices;
	  sep->addChild (lineSet);
	}
	else if (shape == "point")
	{
	  SoPointSet *pointSet = new SoPointSet;
	  pointSet->vertexProperty.setValue (vertices);
	  pointSet->numPoints.setValue (corners.size ());
	  sep->addChild (pointSet);
	}
      }
    }
  }
}

#if 0
static void
make3DEcalCrystalHits(IgCollection **collections, IgAssociationSet **, SoSeparator *sep)
{
  IgCollection		*c = collections[0];
  SoMaterial		*mat = new SoMaterial;

  mat->diffuseColor.setValue(1.0, 0.0, 153.0 / 255.0);
  sep->addChild(mat);

  IgProperty ENERGY = c->getProperty("energy");
  IgProperty F1 = c->getProperty("front_1");
  IgProperty F2 = c->getProperty("front_2");
  IgProperty F3 = c->getProperty("front_3");
  IgProperty F4 = c->getProperty("front_4");
  IgProperty B1 = c->getProperty("back_1");
  IgProperty B2 = c->getProperty("back_2");
  IgProperty B3 = c->getProperty("back_3");
  IgProperty B4 = c->getProperty("back_4");

  for (IgCollectionIterator ci = c->begin(), ce = c->end(); ci != ce; ++ci)
  {
    double energy = ci->get<double>(ENERGY);

    IgV3d f1 = ci->get<IgV3d>(F1);
    IgV3d f2 = ci->get<IgV3d>(F2);
    IgV3d f3 = ci->get<IgV3d>(F3);
    IgV3d f4 = ci->get<IgV3d>(F4);

    IgV3d b1 = ci->get<IgV3d>(B1);
    IgV3d b2 = ci->get<IgV3d>(B2);
    IgV3d b3 = ci->get<IgV3d>(B3);
    IgV3d b4 = ci->get<IgV3d>(B4);

    IgSoCrystalHit *crystalHit = new IgSoCrystalHit;
    crystalHit->energy = energy;
    crystalHit->scale = 1.0;
    crystalHit->relativeWidth = 1.0;
    crystalHit->drawCrystal = false;
    crystalHit->drawHit = true;

    crystalHit->front1.setValue(f1.x(), f1.y(), f1.z());
    crystalHit->front2.setValue(f2.x(), f2.y(), f2.z());
    crystalHit->front3.setValue(f3.x(), f3.y(), f3.z());
    crystalHit->front4.setValue(f4.x(), f4.y(), f4.z());

    crystalHit->back1.setValue(f1.x(), f1.y(), f1.z());
    crystalHit->back2.setValue(f2.x(), f2.y(), f2.z());
    crystalHit->back3.setValue(f3.x(), f3.y(), f3.z());
    crystalHit->back4.setValue(f4.x(), f4.y(), f4.z());

    sep->addChild(crystalHit);
  }
}
#endif

static void
make3DEcalRecHits(IgCollection **collections, IgAssociationSet **assocs, SoSeparator *sep)
{
  SoMaterial *mat = new SoMaterial;
  mat->diffuseColor.setValue(1.0, 0.0, 153.0 / 255.0);
  sep->addChild (mat);
  make3DCrystalHitShapes(collections, assocs, sep);
}

static void
make3DEvent(IgCollection **collections, IgAssociationSet **, SoSeparator *sep)
{
  char			buf [128];
  IgCollection		*c = collections[0];
  SoMaterial		*mat = new SoMaterial;
  SoAnnotation		*overlay = new SoAnnotation;
  SoOrthographicCamera	*camera = new SoOrthographicCamera;
  SoFont		*labelFont = new SoFont;
  SoTranslation		*textStartTranslation = new SoTranslation;
  SoTranslation		*nextLineTranslation  = new SoTranslation;
  IgCollectionItem	e = *c->begin();
  std::string		time  = std::string("Data recorded  ") + e.get<std::string>("time");
  std::string		run   = std::string("Run number     ") + (sprintf(buf, "%d", e.get<int>("run")), buf);
  std::string		event = std::string("Event number   ") + (sprintf(buf, "%d", e.get<int>("event")), buf);
  std::string		ls    = std::string("Lumi section   ") + (sprintf(buf, "%d", e.get<int>("ls")), buf);
  std::string		orbit = std::string("Orbit number   ") + (sprintf(buf, "%d", e.get<int>("orbit")), buf);
  std::string		bx    = std::string("Beam crossing  ") + (sprintf(buf, "%d", e.get<int>("bx")), buf);

  camera->nearDistance = 1;
  camera->farDistance = 10;
  camera->pointAt (SbVec3f(0.0, 0.0, 0.0));
  camera->scaleHeight (5.5f);
  camera->focalDistance = 1;
  overlay->addChild (camera);

  mat->diffuseColor = SbColor(1.0, 1.0, 1.0);
  overlay->addChild (mat);

  labelFont->size = 13.0;
  labelFont->name = "Courier";
  overlay->addChild (labelFont);

  textStartTranslation->translation = SbVec3f (-5.0,  5.0,  0.0);
  nextLineTranslation ->translation = SbVec3f ( 0.0, -0.25, 0.0);
  createTextLine (overlay, textStartTranslation, "-- iSpy -- http://iguana.cern.ch/ispy");
  createTextLine (overlay, nextLineTranslation, time);
  createTextLine (overlay, nextLineTranslation, run);
  createTextLine (overlay, nextLineTranslation, event);
  createTextLine (overlay, nextLineTranslation, ls);
  createTextLine (overlay, nextLineTranslation, orbit);
  createTextLine (overlay, nextLineTranslation, bx);
  sep->addChild (overlay);
}

static void
make3DHcalRecHits(IgCollection **collections, IgAssociationSet **assocs, SoSeparator *sep)
{
  SoMaterial *mat = new SoMaterial;
  mat->diffuseColor = SbColor(0.0, 0.2, 0.5);
  mat->transparency = 0.3;
  sep->addChild (mat);
  make3DCrystalHitShapes(collections, assocs, sep);
}

static void
make3DJets(IgCollection **collections, IgAssociationSet **assocs, SoSeparator *sep)
{
  SoMaterial *mat = new SoMaterial;
  mat->diffuseColor = SbColor (0xFF/255., 0xEE/255., 0xEE/255.);
  mat->transparency = 0.25;
  sep->addChild (mat);
  make3DJetShapes(collections, assocs, sep);
}

static void
make3DMET(IgCollection **collections, IgAssociationSet **, SoSeparator *sep)
{
  IgCollection		*c = collections[0];
  SoMaterial		*mat = new SoMaterial;
  SoDrawStyle		*sty = new SoDrawStyle;
  SoAnnotation		*ann = new SoAnnotation;
  SoDrawStyle		*dashed = new SoDrawStyle;
  SoVertexProperty	*vertices = new SoVertexProperty;
  SoIndexedLineSet	*lineSet = new SoIndexedLineSet;
  std::vector<int>	lineIndices;
  std::vector<SbVec3f>	points;
  int			i = 0;

  mat->diffuseColor = SbColor(0xFF/255., 0x5B/255., 0x00/255.);
  sep->addChild(mat);

  sty->style = SoDrawStyle::LINES;
  sty->lineWidth.setValue (3);
  sep->addChild (sty);

  sep->addChild (ann);

  dashed->style = SoDrawStyle::LINES;
  dashed->lineWidth = 3;
  dashed->linePattern = 0x0f0f;
  ann->addChild (dashed);

  for (IgCollectionIterator ci = c->begin(), ce = c->end(); ci != ce; ++ci)
  {
    points.push_back (SbVec3f (0., 0., 0.));
    points.push_back (SbVec3f (ci->get<double>("px"), ci->get<double>("py"), 0.));
    lineIndices.push_back (i);
    lineIndices.push_back (i + 1);
    lineIndices.push_back (SO_END_LINE_INDEX);
    i += 2;
  }

  vertices->vertex.setValues (0, points.size (), &points [0]);
  vertices->vertex.setNum (points.size ());

  lineSet->coordIndex.setValues (0, lineIndices.size (), &lineIndices [0]);
  lineSet->vertexProperty = vertices;

  sep->addChild (lineSet);
  ann->addChild (lineSet);
}

static void
make3DMuons(IgCollection **collections, IgAssociationSet **assocs, SoSeparator *sep)
{
  IgCollection		*muons = collections[0];
  IgCollection		*points = collections[1];
  IgAssociationSet	*assoc = assocs[0];
  SoMaterial		*mat = new SoMaterial;
  
  mat->diffuseColor = SbColor(0x8B/255., 0x89/255., 0x89/255.);
  sep->addChild(mat);

  for (IgCollectionIterator ci = muons->begin(), ce = muons->end(); ci != ce; ++ci)
  {
    IgSoSimpleTrajectory *track = new IgSoSimpleTrajectory;
    track->lineWidth = 3.0;

    int n = 0;
    for (IgAssociationSet::Iterator ai = assoc->begin (), ae = assoc->end(); ai != ae; ++ai)
    {
      if (ai->first ().objectId () == ci->currentRow ())
      {
	IgCollectionItem hm (points, ai->second ().objectId ());
	double x = hm.get<IgV3d>("pos").x ();
	double y = hm.get<IgV3d>("pos").y ();
	double z = hm.get<IgV3d>("pos").z ();
	track->controlPoints.set1Value (n, SbVec3f(x, y, z));
	track->markerPoints.set1Value (n, SbVec3f(x, y, z));
	n++;
      }
    }
    sep->addChild (track);
  }
}

#if 0
static void
make3DPFClusters(IgCollection **collections, IgAssociationSet **assocs, SoSeparator *sep)
{
  make3DPointSetShapes(collections, assocs, sep,
		       SbColor(0xB0 / 255., 0xE5 / 255., 0x7C / 255.),
		       SoMarkerSet::PLUS_5_5);
}

static void
make3DPFRecHits(IgCollection **collections, IgAssociationSet **assocs, SoSeparator *sep)
{
  make3DPointSetShapes(collections, assocs, sep,
		       SbColor(0xB0 / 255., 0xE5 / 255., 0x7C / 255.),
		       SoMarkerSet::PLUS_5_5);
}

static void
make3DPFRecTracks(IgCollection **collections, IgAssociationSet **assocs, SoSeparator *sep)
{
  make3DPointSetShapes(collections, assocs, sep,
		       SbColor(0xB0 / 255., 0xE5 / 255., 0x7C / 255.),
		       SoMarkerSet::PLUS_5_5);
}
#endif

static void
make3DPixelDigis(IgCollection **collections, IgAssociationSet **assocs, SoSeparator *sep)
{
  make3DPointSetShapes(collections, assocs, sep,
		       SbColor(0.0, 0.0, 1.0),
		       SoMarkerSet::SQUARE_LINE_5_5);
}

static void
make3DRPCRecHits(IgCollection **collections, IgAssociationSet **, SoSeparator *sep)
{
  IgCollection		*c = collections[0];
  IgProperty		U1 = c->getProperty("u1");
  IgProperty		U2 = c->getProperty("u2");
  IgProperty		V1 = c->getProperty("v1");
  IgProperty		V2 = c->getProperty("v2");
  IgProperty		W1 = c->getProperty("w1");
  IgProperty		W2 = c->getProperty("w2");
  SoMaterial		*mat = new SoMaterial;

  mat->diffuseColor = SbColor (0xff/255., 0xff/255., 0x00/255.);
  sep->addChild (mat);

  for (IgCollectionIterator ci = c->begin(), ce = c->end(); ci != ce; ++ci)
  {
    IgV3d u1 = ci->get<IgV3d>(U1);
    IgV3d u2 = ci->get<IgV3d>(U2);
    IgV3d v1 = ci->get<IgV3d>(V1);
    IgV3d v2 = ci->get<IgV3d>(V2);
    IgV3d w1 = ci->get<IgV3d>(W1);
    IgV3d w2 = ci->get<IgV3d>(W2);

    IgSo3DErrorBar *errorBar = new IgSo3DErrorBar;
    errorBar->u1.setValue(u1.x(),u1.y(),u1.z());
    errorBar->u2.setValue(u2.x(),u2.y(),u2.z());
    errorBar->v1.setValue(v1.x(),v1.y(),v1.z());
    errorBar->v2.setValue(v2.x(),v2.y(),v2.z());
    errorBar->w1.setValue(w1.x(),w1.y(),w1.z());
    errorBar->w2.setValue(w2.x(),w2.y(),w2.z());
    errorBar->lineWidth.setValue (2.0);
    sep->addChild(errorBar);
  }
}

static void
make3DSiPixelClusters(IgCollection **collections, IgAssociationSet **assocs, SoSeparator *sep)
{
  make3DPointSetShapes(collections, assocs, sep,
		       SbColor(0x00 / 255., 0xBF / 255., 0xFF / 255.),
		       SoMarkerSet::SQUARE_FILLED_7_7);
}

static void
make3DSiPixelRecHits(IgCollection **collections, IgAssociationSet **assocs, SoSeparator *sep)
{
  make3DPointSetShapes(collections, assocs, sep,
		       SbColor(1.0, 0.0, 0.0),
		       SoMarkerSet::PLUS_7_7);
}

static void
make3DSiStripClusters(IgCollection **collections, IgAssociationSet **assocs, SoSeparator *sep)
{
  make3DPointSetShapes(collections, assocs, sep,
		       SbColor(0x03 / 255., 0xC0 / 255., 0x3C / 255.),
		       SoMarkerSet::CIRCLE_FILLED_7_7);
}

static void
make3DSiStripDigis(IgCollection **collections, IgAssociationSet **assocs, SoSeparator *sep)
{
  make3DPointSetShapes(collections, assocs, sep,
		       SbColor(0x55 / 255., 0x1A / 255., 0x8B / 255.),
		       SoMarkerSet::CIRCLE_LINE_5_5);
}

static void
make3DTracks(IgCollection **collections, IgAssociationSet **assocs, SoSeparator *sep)
{
  IgCollection		*tracks = collections[0];
  IgCollection		*extras = collections[1];
  IgAssociationSet	*assoc = assocs[0];
  IgProperty		PT  = tracks->getProperty ("pt");
  IgProperty		POS = tracks->getProperty ("pos");
  IgProperty		DIR = tracks->getProperty ("dir");
  IgProperty		POS1 = extras->getProperty ("pos_1");
  IgProperty		DIR1 = extras->getProperty ("dir_1");
  IgProperty		POS2 = extras->getProperty ("pos_2");
  IgProperty		DIR2 = extras->getProperty ("dir_2");
  SoMaterial		*mat = new SoMaterial;
  SoDrawStyle		*sty = new SoDrawStyle;
  SoSeparator		*vsep = new SoSeparator;
  SoMaterial		*vmat = new SoMaterial;
  SoVertexProperty	*vertices = new SoVertexProperty;
  SoMarkerSet		*mpoints = new SoMarkerSet;
  int			nv = 0;
  
  mat->diffuseColor = SbColor(0x99/255., 0xCC/255., 0xFF/255.);
  sep->addChild (mat);

  sty->style = SoDrawStyle::LINES;
  sty->lineWidth = 3;
  sep->addChild (sty);

  sep->addChild (vsep);

  vmat->diffuseColor = SbColor(0x00/255., 0x33/255., 0x66/255.);
  vsep->addChild (mat);

  for (IgCollectionIterator ci = tracks->begin(), ce = tracks->end(); ci != ce; ++ci)
  {
    IgSoSplineTrack	*trackRep = new IgSoSplineTrack;
    SoVertexProperty	*tvertices = new SoVertexProperty;
    SoMarkerSet		*tpoints = new SoMarkerSet;
    int			nVtx = 0;

    IgV3d p = ci->get<IgV3d> (POS);
    IgV3d d = ci->get<IgV3d> (DIR);
    vertices->vertex.set1Value (nv++, SbVec3f (p.x(), p.y(), p.z()));
    QString trackName = QString ("Track %1 GeV (%2, %3, %4)")
			.arg (ci->get<double>(PT))
			.arg (p.x()).arg (p.y()).arg (p.z());

    for (IgAssociationSet::Iterator ai = assoc->begin (), ae = assoc->end(); ai != ae; ++ai)
    {
      if (ai->first ().objectId () == ci->currentRow ())
      {
	IgCollectionItem m (extras, ai->second ().objectId ());
	p = ci->get<IgV3d> (POS1);
	d = ci->get<IgV3d> (DIR1);
	SbVec3f diri (d.x(), d.y(), d.z());
	diri.normalize ();

	trackRep->points.set1Value (nVtx, SbVec3f (p.x(), p.y(), p.z()));
	trackRep->tangents.set1Value (nVtx, diri);
	tvertices->vertex.set1Value (nVtx, SbVec3f (p.x(), p.y(), p.z()));
	++nVtx;

	p = ci->get<IgV3d> (POS2);
	d = ci->get<IgV3d> (DIR2);
	SbVec3f diro (d.x(), d.y(), d.z());
	diro.normalize ();

	trackRep->points.set1Value (nVtx, SbVec3f (p.x(), p.y(), p.z()));
	trackRep->tangents.set1Value (nVtx, diro);
	tvertices->vertex.set1Value (nVtx, SbVec3f (p.x(), p.y(), p.z()));
	++nVtx;
      }
    }

    tvertices->vertex.setNum (nVtx);
    tpoints->markerIndex = SoMarkerSet::CIRCLE_LINE_7_7;
    tpoints->vertexProperty = tvertices;
    tpoints->numPoints.setValue (nVtx);

    sep->addChild (trackRep);
    sep->addChild (tpoints);
  }

  vertices->vertex.setNum (nv);
  mpoints->markerIndex = SoMarkerSet::CROSS_9_9;
  mpoints->vertexProperty.setValue (vertices);
  mpoints->numPoints.setValue (nv);
  vsep->addChild (mpoints);
}

static void
make3DTrackingRecHits(IgCollection **collections, IgAssociationSet **assocs, SoSeparator *sep)
{
  make3DPointSetShapes(collections, assocs, sep,
		       SbColor(0xEE/255., 0x2C/255., 0x2C/255.),
		       SoMarkerSet::CIRCLE_LINE_5_5);
}

static void
make3DAnyBox(IgCollection **collections, IgAssociationSet **, SoSeparator *sep)
{
  IgCollection		*c = collections[0];
  IgProperty		F1 = c->getProperty("front_1");
  IgProperty		F2 = c->getProperty("front_2");
  IgProperty		F3 = c->getProperty("front_3");
  IgProperty		F4 = c->getProperty("front_4");
  IgProperty		B1 = c->getProperty("back_1");
  IgProperty		B2 = c->getProperty("back_2");
  IgProperty		B3 = c->getProperty("back_3");
  IgProperty		B4 = c->getProperty("back_4");
  SoVertexProperty	*vertices = new SoVertexProperty;
  SoIndexedLineSet	*lineSet = new SoIndexedLineSet;
  std::vector<SbVec3f>	corners;
  std::vector<int>	indices;
  int			i = 0;

  for (IgCollectionIterator ci = c->begin(), ce = c->end(); ci != ce; ++ci)
  {
    IgV3d p1  = ci->get<IgV3d>(F1);
    IgV3d p2  = ci->get<IgV3d>(F2);
    IgV3d p3  = ci->get<IgV3d>(F3);
    IgV3d p4  = ci->get<IgV3d>(F4);
    IgV3d p5  = ci->get<IgV3d>(B1);
    IgV3d p6  = ci->get<IgV3d>(B2);
    IgV3d p7  = ci->get<IgV3d>(B3);
    IgV3d p8  = ci->get<IgV3d>(B4);

    corners.push_back (SbVec3f (static_cast<double>(p1.x ()),
				static_cast<double>(p1.y ()),
				static_cast<double>(p1.z ())));
    corners.push_back (SbVec3f (static_cast<double>(p2.x ()),
				static_cast<double>(p2.y ()),
				static_cast<double>(p2.z ())));
    corners.push_back (SbVec3f (static_cast<double>(p3.x ()),
				static_cast<double>(p3.y ()),
				static_cast<double>(p3.z ())));
    corners.push_back (SbVec3f (static_cast<double>(p4.x ()),
				static_cast<double>(p4.y ()),
				static_cast<double>(p4.z ())));
    corners.push_back (SbVec3f (static_cast<double>(p5.x ()),
				static_cast<double>(p5.y ()),
				static_cast<double>(p5.z ())));
    corners.push_back (SbVec3f (static_cast<double>(p6.x ()),
				static_cast<double>(p6.y ()),
				static_cast<double>(p6.z ())));
    corners.push_back (SbVec3f (static_cast<double>(p7.x ()),
				static_cast<double>(p7.y ()),
				static_cast<double>(p7.z ())));
    corners.push_back (SbVec3f (static_cast<double>(p8.x ()),
				static_cast<double>(p8.y ()),
				static_cast<double>(p8.z ())));

    indices.push_back (i);
    indices.push_back (i + 1);
    indices.push_back (i + 2);
    indices.push_back (i + 3);
    indices.push_back (i);
    indices.push_back (SO_END_LINE_INDEX);

    indices.push_back (i + 4);
    indices.push_back (i + 5);
    indices.push_back (i + 6);
    indices.push_back (i + 7);
    indices.push_back (i + 4);
    indices.push_back (SO_END_LINE_INDEX);

    indices.push_back (i);
    indices.push_back (i + 4);
    indices.push_back (SO_END_LINE_INDEX);
    indices.push_back (i + 1);
    indices.push_back (i + 5);
    indices.push_back (SO_END_LINE_INDEX);
    indices.push_back (i + 2);
    indices.push_back (i + 6);
    indices.push_back (SO_END_LINE_INDEX);
    indices.push_back (i + 3);
    indices.push_back (i + 7);
    indices.push_back (SO_END_LINE_INDEX);

    i += 8;
  }

  vertices->vertex.setValues (0, corners.size (), &corners [0]);
  vertices->vertex.setNum (corners.size ());

  lineSet->coordIndex.setValues (0, indices.size (), &indices [0]);
  lineSet->vertexProperty = vertices;

  sep->addChild (lineSet);
}

static void
make3DAnyLine(IgCollection **collections, IgAssociationSet **, SoSeparator *sep)
{
  IgCollection		*c = collections[0];
  IgProperty		P1 = c->getProperty ("pos_1");
  IgProperty		P2 = c->getProperty ("pos_2");
  SoVertexProperty	*vertices = new SoVertexProperty;
  SoIndexedLineSet	*lineSet = new SoIndexedLineSet;
  std::vector<SbVec3f>	corners;
  std::vector<int>	indices;
  int			i = 0;

  for (IgCollectionIterator ci = c->begin(), ce = c->end(); ci != ce; ++ci)
  {
    IgV3d p1 = ci->get<IgV3d> (P1);
    IgV3d p2 = ci->get<IgV3d> (P2);
    corners.push_back (SbVec3f (static_cast<double>(p1.x ()),
				static_cast<double>(p1.y ()),
				static_cast<double>(p1.z ())));
    corners.push_back (SbVec3f (static_cast<double>(p2.x ()),
				static_cast<double>(p2.y ()),
				static_cast<double>(p2.z ())));
    indices.push_back (i);
    indices.push_back (i + 1);
    indices.push_back (SO_END_LINE_INDEX);
    i += 2;
  }

  vertices->vertex.setValues (0, corners.size (), &corners [0]);
  vertices->vertex.setNum (corners.size ());

  lineSet->coordIndex.setValues (0, indices.size (), &indices [0]);
  lineSet->vertexProperty = vertices;

  sep->addChild (lineSet);
}

static void
make3DAnyPoint(IgCollection **collections, IgAssociationSet **, SoSeparator *sep)
{
  IgCollection		*c = collections[0];
  IgProperty		POS = c->getProperty ("pos");
  SoPointSet		*points = new SoPointSet;
  SoVertexProperty	*vertices = new SoVertexProperty;
  int			n = 0;

  for (IgCollectionIterator ci = c->begin(), ce = c->end(); ci != ce; ++ci)
  {
    IgV3d p1 = ci->get<IgV3d> (POS);

    double x = p1.x ();
    double y = p1.y ();
    double z = p1.z ();
    vertices->vertex.set1Value (n++, SbVec3f (x, y, z));
  }

  vertices->vertex.setNum (n);
  points->vertexProperty = vertices;
  points->numPoints = n;
  sep->addChild (points);
}

static void
make3DAnyDetId(IgCollection **, IgAssociationSet **, SoSeparator *)
{
}

//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

/** Initialise but do not yet run the application object. */
ISpyApplication::ISpyApplication (void)
  : m_argc (-1),
    m_argv (0),
    m_appname (0),
    m_eventIndex (0),
    m_tableModel (0),
    m_3DModel (0),
    m_mainWindow (0),
    m_treeWidget (0),
    m_splash (0),
    m_autoEvents (false),
    m_exiting (false),
    m_timer (new QTimer (this))
{
  m_archives[0] = m_archives[1] = 0;
  m_storages[0] = new IgDataStorage;
  m_storages[1] = new IgDataStorage;

#ifndef Q_WS_MAC
  QCoreApplication::setApplicationName ("ispy");
#else
  QCoreApplication::setApplicationName ("iSpy");
#endif
  QCoreApplication::setApplicationVersion ("1.x (beta)");
  QCoreApplication::setOrganizationDomain ("iguana");
  QCoreApplication::setOrganizationName ("iguana");

  if (QDir::home ().isReadable ())
    defaultSettings ();

  collection("Tracker",
	     "Tracker_V1:detid:shape",
	     "Geometry_V1:detid",
	     "TrackerGeometry_V1",
	     make3DDetector);

  collection("EcalBarrel",
	     "EcalBarrel_V1:detid:shape",
	     "Geometry_V1:detid",
	     "EcalBarrelGeometry_V1",
	     make3DDetector);

  collection("EcalEndcap",
	     "EcalEndcap_V1:detid:shape",
	     "Geometry_V1:detid",
	     "EcalEndcapGeometry_V1",
	     make3DDetector);

  collection("EcalPreshower",
	     "EcalPreshower_V1:detid:shape",
	     "Geometry_V1:detid",
	     "EcalPreshowerGeometry_V1",
	     make3DDetector);

  collection("HcalBarrel",
	     "HcalBarrel_V1:detid:shape",
	     "Geometry_V1:detid",
	     "HCalBarrelGeometry_V1",
	     make3DDetector);

  collection("HcalEndcap",
	     "HcalEndcap_V1:detid:shape",
	     "Geometry_V1:detid",
	     "HcalEndccapGeometry_V1",
	     make3DDetector);

  collection("HcalOuter",
	     "HcalOuter_V1:detid:shape",
	     "Geometry_V1:detid",
	     "HcalOuterGeometry_V1",
	     make3DDetector);

  collection("HcalForward",
	     "HcalForward_V1:detid:shape",
	     "Geometry_V1:detid",
	     "HcalForwardGeometry_V1",
	     make3DDetector);

  collection("DT",
	     "DTs_V1:detid:shape",
	     "Geometry_V1:detid",
	     "DTGeometry_V1",
	     make3DDetector);

  collection("CSC",
	     "CSC_V1:detid:shape",
	     "Geometry_V1:detid",
	     "CSCGeometry_V1",
	     make3DDetector);

  collection("RPC",
	     "RPC_V1:detid:shape",
	     "Geometry_V1:detid",
	     "RPCGeometry_V1",
	     make3DDetector);

#if 0
  collection("BasicCluster",
	     "BasicClusters_V1:pos:energy",
	     "CaloHits_V1:fraction:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
	     "BasicClustersCaloHits_V1",
	     make3DBasicCluster);
#endif

  collection("CSCSegment",
	     "CSCSegments_V1:pos_1:pos_2",
	     0,
	     0,
	     make3DCSCSegments);

  collection("CaloTower",
	     "CaloTowers_V1:emEnergy:hadEnergy:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
	     0,
	     0,
	     make3DCaloTowers);

  collection("DTDigi",
	     "DTDigis_V1:pos:axis:angle:cellWidth:cellLength:cellWidth:cellHeight",
	     0,
	     0,
	     make3DDTDigis);

  collection("DTRecHit",
	     "DTRecHits_V1:lPlusGlobalPos:lMinusGlobalPos:rPlusGlobalPos:rMinusGlobalPos"
	     ":lGlobalPos:rGlobalPos:wirePos:axis:angle:cellWidth:cellLength:cellHeight",
	     0,
	     0,
	     make3DDTRecHits);

  collection("DTRecSegment4D",
	     "DTRecSegment4D_V1:pos_1:pos_2",
	     0,
	     0,
	     make3DDTRecSegment4D);
#if 0
  collection("EBRecHit",
	     "EBRecHits_V1:energy:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
	     0,
	     0,
	     make3DEcalCrystalHits);

  collection("EERecHit",
	     "EERecHits_V1:energy:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
	     0,
	     0,
	     make3DEcalCrystalHits);
#endif

  collection("EcalRecHit",
	     "EcalRecHits_V1:energy:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
	     0,
	     0,
	     make3DEcalRecHits);

  collection("Event",
	     "Event_V1:time:run:event:ls:orbit:bx",
	     0,
	     0,
	     make3DEvent);

  collection("HBRecHit",
	     "HBRecHits_V1:energy:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
	     0,
	     0,
	     make3DHcalRecHits);

  collection("HERecHit",
	     "HERecHits_V1:energy:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
	     0,
	     0,
	     make3DHcalRecHits);

  collection("HFRecHit",
	     "HFRecHits_V1:energy:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
	     0,
	     0,
	     make3DHcalRecHits);

  collection("HORecHit",
	     "HORecHits_V1:energy:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
	     0,
	     0,
	     make3DHcalRecHits);

  collection("Jet",
	     "Jets_V1:et:theta:phi",
	     0,
	     0,
	     make3DJets);

  collection("MET",
	     "METs_V1:pt:px:py:phi",
	     0,
	     0,
	     make3DMET);

  collection("Muon",
	     "Muons_V1:pt:charge:rp:phi:eta",
	     "Points_V1:pos",
	     "MuonTrackerPoints_V1",
	     make3DMuons);

#if 0
  collection("PFCluster",
	     "PFClusters_V1:pos",
	     0,
	     0,
	     make3DPFClusters);

  collection("PFRecHit",
	     "PFRecHits_V1:pos",
	     0,
	     0,
	     make3DPFRecHits);

  collection("PFRecTrack",
	     "PFRecTracks_V1:pos",
	     0,
	     0,
	     make3DPFRecTracks);
#endif

  collection("PixelDigi",
	     "PixelDigis_V1:pos",
	     0,
	     0,
	     make3DPixelDigis);

  collection("RPCRecHit",
	     "RPCRecHits_V1:u1:u2:v1:v2:w2",
	     0,
	     0,
	     make3DRPCRecHits);

  collection("SiPixelCluster",
	     "SiPixelClusters_V1:pos",
	     0,
	     0,
	     make3DSiPixelClusters);

  collection("SiPixelRecHit",
	     "SiPixelRecHits_V1:pos",
	     0,
	     0,
	     make3DSiPixelRecHits);

  collection("SiStripCluster",
	     "SiStripClusters_V1:pos",
	     0,
	     0,
	     make3DSiStripClusters);

  collection("SiStripDigi",
	     "SiStripDigis_V1:pos",
	     0,
	     0,
	     make3DSiStripDigis);

  collection("Track",
	     "Tracks_V1:pt:pos:dir",
	     "Extras_V1:pos_1:dir_1:pos_2:dir_2",
	     "TrackExtras_V1",
	     make3DTracks);

  collection("TrackingRecHit",
	     "TrackingRecHits_V1:pos",
	     0,
	     0,
	     make3DTrackingRecHits);

  collection(0, ":front_1:front_2:front_3:front_4", 0, 0, make3DAnyBox);
  collection(0, ":pos_1:pos_2", 0, 0, make3DAnyLine);
  collection(0, ":pos", 0, 0, make3DAnyPoint);
  collection(0, ":detid", 0, 0, make3DAnyDetId);
}

/** Destroy the application.  A no-op since everything is done on exit. */
ISpyApplication::~ISpyApplication(void)
{
}

/** Specify a new collection.  Call this during the application
    initialisation to register known collection handlers.

    When reading input data, the information will be used to look for
    collections matching the specifications (see updateCollections())
    and create display lists accordingly.  The drawing method used
    will be invoked if and only if a non-empty collection matching all
    the requirements is found.

    The order of calls to collection() is significant.  Only the
    first matching specification is used, so less specific matches
    should be added last.

    @a friendlyName

    If non-null, use this name instead of the collection name in
    the tree view.

    @a collectionSpec

    Collection to use.  Must be non-null and non-empty, either
    NAME[:PROPERTY[:...]] or :PROPERTY[:...].  The former looks for
    a named collection, and requires the colon-separated list of
    properties in it.  The latter looks for any collection with
    those properties.

    @a otherCollectionSpec

    Another collection that is required, usually linked by an
    association.  Can be either null pointer or a string in the
    format specified for @a collectionSpec.

    @a associationSpec

    Either null pointer or the name of an association required
    to display the collection.  This would normally be non-null
    if @a otherCollectionSpec is non-null and would name the
    association linking the two together.

    @a make3D

    Method to invoke to render a collection matching these
    requirements.  */
void
ISpyApplication::collection(const char *friendlyName,
			    const char *collectionSpec,
			    const char *otherCollectionSpec,
			    const char *associationSpec,
			    Make3D make3D)
{
  ASSERT(collectionSpec);

  m_specs.resize(m_specs.size() + 1);
  CollectionSpec &spec = m_specs.back();
  StringList parts;

  if (friendlyName)
    spec.friendlyName = friendlyName;

  parts = StringOps::split(collectionSpec, ':');
  ASSERT(! parts.empty());
  spec.collection = parts[0];
  spec.requiredFields.insert(spec.requiredFields.end(),
			     parts.begin()+1, parts.end());

  if (otherCollectionSpec)
  {
    parts = StringOps::split(otherCollectionSpec, ':');
    ASSERT(! parts.empty());
    spec.otherCollection = parts[0];
    spec.otherRequiredFields.insert(spec.otherRequiredFields.end(),
				    parts.begin()+1, parts.end());
  }
  
  if (associationSpec)
  {
    parts = StringOps::split(associationSpec, ':');
    ASSERT(parts.size() == 1);
    spec.otherAssociation = parts[0];
  }

  spec.make3D = make3D;
}

/** Begin to exit application.  Clears all internal data structures
    and displays, saves settings, and exits. */
void
ISpyApplication::onExit(void)
{
  m_exiting = true;
  delete m_tableModel;
  m_tableModel = 0;
  m_3DModel->sceneGraph()->removeAllChildren();
  m_events.clear();
  m_collections.clear();
  m_specs.clear();
  for (int i = 0; i < 2; ++i)
  {
    delete m_storages[i];
    delete m_archives[i];
  }
  m_mainWindow->saveSettings ();
  exit ();
}

/** Actually exit the application.  */
void
ISpyApplication::exit(void)
{
  qApp->closeAllWindows ();
}

/** Return the number of command line arguments given to the
    application.  These have already been cleaned up from known
    options, e.g. those destined to Qt.  @sa argv */
int
ISpyApplication::argc(void) const
{ return m_argc; }


/** Return the command line arguments given to the application.  These
    have already been cleaned up from known options, e.g. those
    destined to Qt.  @sa argc */
char **
ISpyApplication::argv(void) const
{ return m_argv; }

/** Return a clean application name: argv[0] without directories. */
const char *
ISpyApplication::appname(void) const
{
  const char *name = m_appname;
  if (const char *ptr = strrchr (name, '/'))
    name = ptr + 1;
  return name;
}

/** Show instruction on how to invoke this application (unix). */
int
ISpyApplication::usage(void)
{
  const char *app = appname ();
  std::cerr << "Usage: " << app << " [OPTION-OR-FILENAME]...\n"
	    << "   or: " << app << " --help\n"
	    << "   or: " << app << " --version\n";

  return EXIT_FAILURE;
}

/** Show application version information. */
int
ISpyApplication::version(void)
{
  std::cerr << QCoreApplication::applicationName ().toStdString () << " "
	    << QCoreApplication::applicationVersion ().toStdString () << "\n";

  return EXIT_SUCCESS;
}

/** Run the application with the given command line arguments.
    Returns the exit code, but note that the application may exit
    without returning in situations such as log-out. */
int
ISpyApplication::run(int argc, char *argv[])
{
  m_appname = argv [0];
  m_argc = argc;
  m_argv = argv;

  while (*++argv)
  {
    if (!strcmp(*argv, "--help"))
      return usage();
    else if (!strcmp(*argv, "--version"))
      return version();
  }

  return doRun ();
}

/** DOCUMENT ME */
void
ISpyApplication::defaultSettings(void)
{
  QSettings settings;

  if (settings.isWritable ())
  {
    //
    // Open file dialog settings
    //
    if (! settings.contains ("igfiles/home"))
    {
      QUrl url ("file:/afs/cern.ch/user/i/iguana/www/ispy/igfiles/");
      settings.setValue ("igfiles/home", url);
    }

    //
    // Default geometry file
    //
    if (! settings.contains ("igfiles/geometry"))
    {
      QString fileName ("default-geometry.ig");
      settings.setValue ("igfiles/geometry", fileName);
    }

    //
    // Network connection configuration
    //
    if (! settings.contains ("igsource/host"))
    {
      QString hostName ("localhost");
      settings.setValue ("igsource/host", hostName);
    }
    if (! settings.contains ("igsource/port"))
    {
      int port = 9000;
      settings.setValue ("igsource/port", port);
    }
    if (! settings.contains ("igsource/debug"))
    {
      settings.setValue ("igsource/debug", false);
    }
    if (! settings.contains ("igevents/auto"))
    {
      settings.setValue ("igevents/auto", false);
    }
    if (! settings.contains ("igevents/timeout"))
    {
      int timeout = 15000;
      settings.setValue ("igevents/timeout", timeout);
    }
    if (! settings.contains ("igevents/cuts/jets/energy"))
    {
      settings.setValue ("igevents/cuts/jets/energy", 0.1);
    }
    if (! settings.contains ("igevents/cuts/calotowers/energy"))
    {
      settings.setValue ("igevents/cuts/calotowers/energy", 0.1);
    }
    if (! settings.contains ("igevents/cuts/ecal/barrel/rechits/energy"))
    {
      settings.setValue ("igevents/cuts/ecal/barrel/rechits/energy", 0.1);
    }
    if (! settings.contains ("igevents/cuts/ecal/endcap/rechits/energy"))
    {
      settings.setValue ("igevents/cuts/ecal/endcap/rechits/energy", 0.1);
    }
    if (! settings.contains ("igevents/cuts/hcal/barrel/rechits/energy"))
    {
      settings.setValue ("igevents/cuts/hcal/barrel/rechits/energy", 0.1);
    }
    if (! settings.contains ("igevents/cuts/hcal/endcap/rechits/energy"))
    {
      settings.setValue ("igevents/cuts/hcal/endcap/rechits/energy", 0.1);
    }
    if (! settings.contains ("igevents/cuts/hcal/forward/rechits/energy"))
    {
      settings.setValue ("igevents/cuts/hcal/forward/rechits/energy", 0.1);
    }
    if (! settings.contains ("igevents/cuts/hcal/outer/rechits/energy"))
    {
      settings.setValue ("igevents/cuts/hcal/outer/rechits/energy", 0.1);
    }

    //
    // Main window configuration
    //
    if (! settings.contains ("mainwindow/configuration/save"))
    {
      settings.setValue ("mainwindow/configuration/save", false);
    }
    if (! settings.contains ("mainwindow/treeview/shown"))
    {
      settings.setValue ("mainwindow/treeview/shown", true);
    }
    if (! settings.contains ("mainwindow/treeview/floating"))
    {
      settings.setValue ("mainwindow/treeview/floating", false);
    }
    if (! settings.contains ("mainwindow/tableview/shown"))
    {
      settings.setValue ("mainwindow/tableview/shown", true);
    }
    if (! settings.contains ("mainwindow/tableview/floating"))
    {
      settings.setValue ("mainwindow/tableview/floating", false);
    }

    // HLR
    if (! settings.contains ("igdisplay/hadtowers/view3d/hiddenlineremoval"))
    {
      settings.setValue ("igdisplay/hadtowers/view3d/hiddenlineremoval", false);
    }
    if (! settings.contains ("igdisplay/crystalhits/view3d/hiddenlineremoval"))
    {
      settings.setValue ("igdisplay/crystalhits/view3d/hiddenlineremoval", false);
    }
    if (! settings.contains ("igdisplay/crystals/view3d/hiddenlineremoval"))
    {
      settings.setValue ("igdisplay/crystals/view3d/hiddenlineremoval", false);
    }
    if (! settings.contains ("igdisplay/detector/view3d/hiddenlineremoval"))
    {
      settings.setValue ("igdisplay/detector/view3d/hiddenlineremoval", false);
    }
  }
}

/** DOCUMENT ME */
void
ISpyApplication::restoreSettings(void)
{
  QSettings settings;
  if (settings.contains ("igevents/auto"))
  {
    m_autoEvents = settings.value ("igevents/auto").value<bool> ();
    autoEvents ();
  }
}

/** Set up the main window. */
void
ISpyApplication::setupMainWindow(void)
{
  m_mainWindow = new ISpyMainWindow;

  QObject::connect (m_mainWindow, SIGNAL(open()),          this, SLOT(openFileDialog()));
  QObject::connect (m_mainWindow, SIGNAL(autoEvents()),    this, SLOT(autoEvents()));
  QObject::connect (m_mainWindow, SIGNAL(nextEvent()),     this, SLOT(nextEvent()));
  QObject::connect (m_mainWindow, SIGNAL(previousEvent()), this, SLOT(previousEvent()));
  QObject::connect (m_mainWindow, SIGNAL(rewind()),        this, SLOT(rewind()));
  QObject::connect (m_mainWindow, SIGNAL(print()),         this, SIGNAL(print ()));
  QObject::connect (m_mainWindow, SIGNAL(save()),          this, SIGNAL(save ()));

  m_mainWindow->actionAuto->setChecked (false);
  m_mainWindow->actionAuto->setEnabled (false);
  m_mainWindow->actionNext->setEnabled (false);
  m_mainWindow->actionPrevious->setEnabled (false);
  m_mainWindow->treeView->hide ();

  QStringList headers;
  headers << "Collection" << "Size" << "Visibility";
  m_treeWidget = new QTreeWidget(m_mainWindow->dockTreeWidgetContents);

  m_treeWidget->setHeaderLabels (headers);
  m_treeWidget->setColumnWidth (1, 50);
  m_treeWidget->setColumnWidth (2, 40);
  m_treeWidget->setAlternatingRowColors (true);
  m_mainWindow->gridLayout_3->addWidget(m_treeWidget);

  m_mainWindow->restoreSettings ();

  QObject::connect (m_treeWidget, SIGNAL(currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)),
		    this, SLOT(setCurrentItem(QTreeWidgetItem *, QTreeWidgetItem *)));
  QObject::connect (m_treeWidget, SIGNAL(itemActivated(QTreeWidgetItem *, int)),
		    this, SLOT(itemActivated(QTreeWidgetItem *, int)));
  QObject::connect (m_treeWidget, SIGNAL(itemClicked(QTreeWidgetItem *, int)),
		    this, SLOT(itemActivated(QTreeWidgetItem *, int)));
}

/** Take the splash screen down.  Called from timer signal. */
void
ISpyApplication::cleanSplash(void)
{
  ASSERT(m_splash);
  delete m_splash;
}

/** Main application run loop.  Initialises the application, shows its
    windows, opens any files requested (on command line or from
    operating system open file events) and executes the run loop.
    Returns the application exit code.  Note that this function may
    never return in certain situations such as GUI log-out. */
int
ISpyApplication::doRun(void)
{
  ISpyEventFilter filter;
  QApplication app (m_argc, m_argv);
  QWidget *mainwin = SoQt::init (m_argc, m_argv, m_argv[0]);
  QEventLoop evloop;

  setupMainWindow ();
  m_mainWindow->workspace ()->addSubWindow (mainwin);
  mainwin->setWindowTitle ("iSpy 3D");

  restoreSettings ();
  initShapes ();

  m_3DModel = new Ig3DBaseModel;
  ISpy3DView *view = new ISpy3DView (m_3DModel, mainwin);
  view->setFeedbackVisibility (true);
  m_mainWindow->addToolBar (Qt::TopToolBarArea, view->toolBar ());

  QList <QMdiSubWindow *> winList = m_mainWindow->workspace ()->subWindowList ();
  if (winList.length () > 1)
    m_mainWindow->workspace ()->tileSubWindows ();
  else
    winList.first ()->showMaximized ();

  QObject::connect (this, SIGNAL(save ()), view, SLOT(save ()));
  QObject::connect (this, SIGNAL(print ()), view, SLOT(print ()));
  QObject::connect(m_mainWindow->actionQuit, SIGNAL(triggered()), this, SLOT(onExit()));
  QObject::connect(m_mainWindow->actionClose, SIGNAL(triggered()), this, SLOT(onExit()));
  QObject::connect(this, SIGNAL(showMessage (const QString &)),
		   m_mainWindow->statusBar (), SLOT(showMessage (const QString &)));
  QObject::connect (&filter, SIGNAL(open (const QString &)),
		    this, SLOT(open (const QString &)));
  app.installEventFilter (&filter);

  // Show splash screen now.
  QTimer::singleShot (6000, this, SLOT(cleanSplash()));
  m_splash = new ISpySplashScreen;
  m_splash->show ();

  // Process pending events now to allow file open events through
  // (mac), calling "open()" on the files. Exclude user input and
  // don't wait for events to occur.  Also shows the splash screen.
  evloop.processEvents(QEventLoop::ExcludeUserInputEvents);

  // Open file names given on the command line (unix, windows).
  for (int i = 1; i < m_argc; ++i)
    open(m_argv[i]);

  // Show main window.
  mainwin->show ();
  m_mainWindow->actionSave->setEnabled (true);
  m_mainWindow->actionPrint->setEnabled (true);
  m_mainWindow->show ();

  // If we don't have any files open, show file open dialog.  Note
  // that this already enters a full event loop since it's modal.
  if (! m_archives[0] && ! m_archives[1])
    openFileDialog ();

  // Now run.
  SoQt::mainLoop();
  delete view;
  return 0;
}

/** Respond to selection changes in the tree view.  This may be due to
    a GUI action such as mouse clicking or using arrow keys, or may
    result indirectly from moving between events or opening a file.

    Forwards to itemActivated().

    @a current   The new "current" item in the tree view.  */
void
ISpyApplication::setCurrentItem(QTreeWidgetItem *current, QTreeWidgetItem *)
{
  itemActivated(current, 0);
}

/** Respond to selection changes in the tree view.  This may be due to
    a GUI action such as mouse clicking or using arrow keys, or may
    result indirectly from moving between events or opening a file.

    This is the master controller for what content is shown, and
    adjusts the 3D and table views to match.

    @a current   The new "current" item in the tree view.  */
void
ISpyApplication::itemActivated(QTreeWidgetItem *current, int)
{
  // FIXME: Do we need to do anything if current item was cleared?
  if (current)
  {

    size_t index = m_treeWidget->indexOfTopLevelItem(current);
    ASSERT(index < m_collections.size());
    Collection &c = m_collections[index];
    ASSERT(c.item == current);

    // Record visibility setting, including to qsettings.
    QSettings settings;
    settings.setValue("igtwigs/visibility/" + current->text(0),
		      c.visibility = current->checkState(2));

    // Show the contents in 3D, as appropriate.
    displayCollection(c);

    // Show table collection.
    if (! m_tableModel)
    {
      m_tableModel = new IgCollectionTableModel(c.data[0]);
      m_mainWindow->tableView->setModel(m_tableModel);
    }
    else
      m_tableModel->setCollection(c.data[0]);

#if 0
    // Clear any past 3D model selection and highlight this collection
    // (FIXME: do we want this?).
    m_3DModel->selection()->removeAllChildren();
    if (c.visibility == Qt::Checked)
      m_3DModel->selection()->addChild(c->sep);
#endif
  }
}

/** Control collection display in the 3D window (only).  Toggles the
    SoSwitch to allow the content to be seen, and if the content
    wasn't created yet, creates it first. */
void
ISpyApplication::displayCollection(Collection &c)
{
  ASSERT(c.data[0]);
  ASSERT(c.item);
  ASSERT(c.node);
  ASSERT(c.sep);

  // Show or hide 3D as appropriate.
  if (c.visibility == Qt::Unchecked)
    c.node->whichChild = SO_SWITCH_NONE;
  else
  {
    if (c.spec
	&& ! c.sep->getNumChildren()
	&& c.data[0]->size() > 0
	&& c.spec->make3D)
      (*c.spec->make3D)(c.data, &c.assoc, c.sep);
    c.node->whichChild = SO_SWITCH_ALL;
  }
}

/** Update the collection list in the tree view.

    This method should be invoked any time the list of available
    collections has changed (such as on file open), or the content of
    the collections has changed (such as on new event).

    Rebuilds the tree, table and 3D views from scratch to match the
    new contents, using collection specifications as content guide.
    The current item (selection) and visibility are restored across
    the rebuild.

    FIXME: Restore tree view, table view viewport settings? */
void
ISpyApplication::updateCollections(void)
{
  // Remember currently selected collection.
  QSettings settings;
  QString selected;
  QTreeWidgetItem *current = m_treeWidget->currentItem();
  if (current) selected = current->text(0);

  // Clear tree and table views. We clear 3D view at the end.
  delete m_tableModel;
  m_tableModel = 0;
  m_treeWidget->clear();

  // Rebuild the collections. Loop over collections in the order they
  // appear in the input storages, event first, geometry after.
  // Record these in our collections list for later reference.
  Collections oldcollections(m_storages[0]->collectionNames().size() +
			     m_storages[1]->collectionNames().size());
  m_collections.swap(oldcollections);
  for (size_t sti = 0, ste = 2, i = 0; sti < ste; ++sti)
  {
    // Try to locate a matching collection spec.  Accept the spec only
    // if it qualifies all stated requirements.  If it matches, use
    // the "friendly" collection name from the spec and its drawing
    // utility.
    StringList &names = m_storages[sti]->collectionNames();
    for (size_t ci = 0, ce = names.size(); ci != ce; ++ci, ++i)
    {
      std::string	name = names[ci];
      IgCollection	*coll = m_storages[sti]->getCollectionPtr(ci);
      IgCollection	*other = 0;
      IgAssociationSet	*assoc = 0;
      CollectionSpec	*spec = 0;

      for (size_t spi = 0, spe = m_specs.size(); spi != spe && !spec; ++spi)
      {
	CollectionSpec &cand = m_specs[spi];
	if (cand.collection.empty() || cand.collection == name)
	{
	  bool hasFields = true;
	  bool hasOther = true;
	  bool hasOtherFields = true;
	  bool hasAssoc = true;
	  
	  for (size_t fi = 0, fe = cand.requiredFields.size(); fi != fe && hasFields; ++fi)
	    hasFields = coll->hasProperty(cand.requiredFields[fi]);

	  if (! cand.otherCollection.empty())
	  {
	    if (! (other = m_storages[sti]->getCollectionPtr(cand.otherCollection)))
	      hasOther = hasOtherFields = false;

	    for (size_t fi = 0, fe = cand.otherRequiredFields.size(); fi != fe && hasOtherFields; ++fi)
	      hasOtherFields = other->hasProperty(cand.otherRequiredFields[fi]);
	  }

	  if (! cand.otherAssociation.empty())
	  {
	    if (! (assoc = m_storages[sti]->getAssociationSetPtr(cand.otherAssociation)))
	      hasAssoc = false;
	  }

	  if (hasFields && hasOther && hasOtherFields && hasAssoc)
	  {
	    if (! cand.friendlyName.empty())
	      name = cand.friendlyName;
	    spec = &cand;
	  }
	}
      }
      
      // Create new items in the tree view and a placeholder content
      // node in the 3D model. The latter will be filled in on first
      // display, and directly here if the visibility is on.
      int visibility = settings.value("igtwigs/visibility/" + QString(name.c_str()),
				      Qt::Checked).value<int>();
      QTreeWidgetItem *item = new QTreeWidgetItem(m_treeWidget);
      item->setText(0, name.c_str());
      item->setText(1, QString("%1").arg(coll->size()));
      item->setCheckState(2, Qt::CheckState(visibility));

      SoSwitch *sw = new SoSwitch;
      SoSeparator *sep = new SoSeparator;
      sw->setName(SbName(names[ci].c_str()));
      sw->addChild(sep);

      // If this is geometry, and we had the same geometry, transfer
      // it over to the new collection to avoid recomputing it.  Note
      // that reading files in deliberately clears the .sep fields so
      // we won't be fooled here to think a new collection is the same
      // as the old one just because it has the same pointer.
      if (sti == 1)
      {
	for (size_t oi = 0, oe = oldcollections.size(); oi != oe; ++oi)
	  if (oldcollections[oi].data[0] == coll && oldcollections[oi].sep)
	    for (int zi = 0, ze = oldcollections[oi].sep->getNumChildren(); zi != ze; ++zi)
	      sep->addChild(oldcollections[oi].sep->getChild(zi));
      }

      // Fill in the collection item. This needs to be in place before
      // we modify the tree as our slots on tree notifications use it.
      m_collections[i].spec = spec;
      m_collections[i].data[0] = coll;
      m_collections[i].data[1] = other;
      m_collections[i].assoc = assoc;
      m_collections[i].item = item;
      m_collections[i].node = sw;
      m_collections[i].sep = sep;
      m_collections[i].visibility = visibility;

      // Set current item. Updates table and 3D views too.
      // If this is not current and visible, show in 3D.
      if (! selected.isEmpty () && selected == item->text(0))
	m_treeWidget->setCurrentItem(item);
      else
	displayCollection(m_collections[i]);
    }
  }

  // Clear and re-fill the 3D now that we don't need old data.
  m_3DModel->contents()->removeAllChildren();
  for (size_t i = 0, e = m_collections.size(); i != e; ++i)
    m_3DModel->contents()->addChild(m_collections[i].node);

  // Finally adjust buttons to what can be done here.
  m_mainWindow->actionAuto->setEnabled(! m_events.empty());
  m_mainWindow->actionNext->setEnabled(! m_events.empty() && m_eventIndex < m_events.size()-1);
  m_mainWindow->actionPrevious->setEnabled(m_eventIndex > 0);
  if (m_eventIndex >= (m_events.empty() ? 0 : m_events.size()-1))
  {
    m_timer->stop ();
    m_timer->disconnect ();
    m_mainWindow->actionAuto->setChecked (false);
  }
}

/** Read in and display a new event. */
void
ISpyApplication::newEvent(void)
{
  ASSERT(m_eventIndex < m_events.size());
  delete m_storages[0];
  readData(m_storages[0] = new IgDataStorage,
	   m_events[m_eventIndex].archive,
	   m_events[m_eventIndex].contents);
  updateCollections();
}

/** Prompt for a new file to be openend. */
void
ISpyApplication::openFileDialog(void)
{
  QString fileName;
  QFileDialog f(m_mainWindow->centralwidget, tr("Open File"), ".",
		tr("Ig Files (*.ig *.iguana)"));
  f.setFileMode(QFileDialog::ExistingFiles);

  QList<QUrl> shortcuts = f.sidebarUrls();
  shortcuts.append(QSettings().value ("igfiles/home").value<QUrl>());
  shortcuts.append(QUrl("file:/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/EventDisplay"));
  f.setSidebarUrls(shortcuts);

  if (f.exec())
    open(f.selectedFiles().front());
}

/** Open a new file.  Auto-detects file contents to judge what to do
    with the file.  Supports opening a geometry-only file, an
    events-only file, or mixed files.  If the file contains geometry,
    shows it.  If the file contains events, shows the first event.
    Apart from those, only the zip index is read and is used to
    rebuild the in-memory event list.  */
void
ISpyApplication::open(const QString &fileName)
{
  if (fileName.isEmpty())
    return;

  qDebug () << "Try to open " << fileName;
  showMessage(QString("Opening ") + fileName + tr("..."));

  // Read the file in.
  ZipArchive *file = loadFile(fileName);
  if (! file)
    return;

  // See what the file contains.
  Events			events;
  lat::ZipMember		*geometry = 0;
  lat::ZipArchive::Iterator	zi, ze;
  size_t			index = 0;

  events.reserve(file->size());
  for (zi = file->begin(), ze = file->end(); zi != ze; ++zi, ++index)
    if (! strncmp((*zi)->name(), "Geometry/", 9))
    {
      if (geometry)
	qDebug() << "Oopsla, multiple geometries, keeping last one.";
      geometry = *zi;
    }
    else if (! strncmp((*zi)->name(), "Events/", 7))
    {
      events.resize(events.size()+1);
      Event &e = events.back();
      e.index = index;
      e.archive = file;
      e.contents = *zi;
    }

  // If the file had only geometry, take it as the geometry file.  If
  // the file has events and geometry, take both from it.  If the file
  // has no geometry, take event list from it, whether it had any
  // events or not.
  bool update = false;
  if (geometry)
  {
    // Invalidate all collections so updateCollections() won't think
    // it has valid geometry if happen to get back identical pointers.
    for (size_t ci = 0, ce = m_collections.size(); ci != ce; ++ci)
      m_collections[ci].sep = 0;

    if (m_archives[1])
      m_archives[1]->close();
    delete m_storages[1];
    delete m_archives[1];
    m_storages[1] = new IgDataStorage;
    m_archives[1] = file;
    readData(m_storages[1], file, geometry);
    update = true;
  }

  if (! events.empty() || ! geometry)
  {
    if (m_archives[0])
      m_archives[0]->close();
    delete m_storages[0];
    delete m_archives[0];
    events.swap(m_events);
    m_storages[0] = new IgDataStorage;
    m_archives[0] = file;
    m_eventIndex = 0;
    if (m_events.empty())
      update = true;
    else
    {
      newEvent();
      update = false;
    }
  }

  if (update)
    updateCollections();
}

/** Helper function to load zip archive index contents. */
ZipArchive *
ISpyApplication::loadFile(const QString &filename)
{
  ZipArchive *file = 0;
  try
  {
    file = new ZipArchive (Filename(filename.toStdString())
			   .substitute(ShellEnvironment()),
			   IOFlags::OpenRead);
  }
  catch (lat::Error &e)
  {
    std::cerr << (const char *) filename.toAscii()
	      << ": error: cannot read: "
	      << e.explain() << std::endl;
  }
  return file;
}

/** Read an object out of a zip file: an individual event or geometry.

    @a to	Storage to fill in with the file contents.  This should
    		be a new IgDataStorage created for this call.
    @a archive  Archive file containing @a source.
    @a source   The source data file to read in. */
void
ISpyApplication::readData(IgDataStorage *to, ZipArchive *archive, ZipMember *source)
{
  qDebug () << QString("Reading ") << source->name () << tr("...");
  showMessage(QString("Reading ") + source->name () + tr("..."));

  try
  {
    InputStream *is = archive->input(source);
    IOSize length = source->size(ZipMember::UNCOMPRESSED);
    std::vector<char> buffer(length+1, 0);
    is->xread(&buffer[0], length);
    is->close();
    delete is;

    IgParser parser(to);
    parser.parse(&buffer[0]);
  }
  catch (ParseError &e)
  {
    std::cerr << source->name() << ": parse error at char " << e.position() << std::endl;
  }
  catch (lat::Error &e)
  {
    std::cerr << source->name() << ": error: " << e.explain() << std::endl;
  }
  catch (std::exception &e)
  {
    std::cerr << source->name() << ": error: " << e.what() << std::endl;
  }

  showMessage (source->name().name());
}

/** DOCUMENT ME */
void
ISpyApplication::connect(void)
{
  qDebug() << "Connect not implemented.";
}

/** Toggle events auto-play. */
void
ISpyApplication::autoEvents(void)
{
  int timeout = QSettings().value("igevents/timeout", 15000).value<int>();
  m_autoEvents = m_mainWindow->actionAuto->isChecked ();

  if (m_autoEvents)
  {
    QObject::connect(m_timer, SIGNAL(timeout()), SLOT(nextEvent()));
    m_timer->start(timeout);
  }
  else
  {
    m_timer->stop();
    m_timer->disconnect();
  }
}

/** Go to the next event. */
void
ISpyApplication::nextEvent(void)
{
  if (++m_eventIndex < m_events.size())
  {
    showMessage(m_events[m_eventIndex].contents->name().name());
    newEvent();
  }
  else
    m_eventIndex = (m_events.empty() ? 0 : m_events.size()-1);
}

/** Go to the preveious event. */
void
ISpyApplication::previousEvent(void)
{
  if (m_eventIndex > 0 && --m_eventIndex < m_events.size())
  {
    showMessage(m_events[m_eventIndex].contents->name().name());
    newEvent();
  }
  else
    m_eventIndex = 0;
}

/** Go to the first event. */
void
ISpyApplication::rewind (void)
{
  m_eventIndex = 0;
  if (m_eventIndex < m_events.size())
  {
    showMessage(m_events[m_eventIndex].contents->name().name());
    newEvent();
  }
}
