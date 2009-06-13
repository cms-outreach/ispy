//<<<<<< INCLUDES                                                       >>>>>>
#define QT_NO_EMIT
#include "Iguana/QtGUI/interface/ISpyApplication.h"
#include "Iguana/QtGUI/interface/ISpy3DView.h"
#include "Iguana/QtGUI/interface/ISpyEventFilter.h"
#include "Iguana/QtGUI/interface/ISpyMainWindow.h"
#include "Iguana/QtGUI/interface/ISpySplashScreen.h"
#include "Iguana/QtGUI/interface/Ig3DBaseModel.h"
#include "Iguana/QtGUI/interface/IgCollectionTableModel.h"
#include "Iguana/QtGUI/interface/IgNetworkReplyHandler.h"
#include "Iguana/QtGUI/interface/IgDrawTowerHelper.h"

#include "Iguana/Framework/interface/IgCollection.h"
#include "Iguana/Framework/interface/IgParser.h"

// FIXME : these should be migrated from shapes into draw functions

#include "Iguana/Inventor/interface/IgSoJet.h"
#include "Iguana/Inventor/interface/IgSoSimpleTrajectory.h"
#include "Iguana/Inventor/interface/IgSoSplineTrack.h"

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
#include <QtNetwork>
#include <iostream>


using namespace lat;

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>

static void initShapes (void)
{
   IgSoShapeKit::initClass ();
   IgSoJet::initClass ();
   IgSoSimpleTrajectory::initClass ();
   IgSoSplineTrack::initClass ();
}




// ------------------------------------------------------
// Draw Text Overlays 
// ------------------------------------------------------


static void
createTextLine(SoGroup *group, SoTranslation *trans, const std::string &text)
{
  SoText2 *label = new SoText2;
  label->string = text.c_str ();
  group->addChild (trans);
  group->addChild (label);
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

  // FIXME LT: make text positioning independent of window resize
  // FIXME LT: make visibilty of each line switchable in the interface (and settings) e.g. as for ig collections

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



// ------------------------------------------------------
// Draw Generic shapes  
// ------------------------------------------------------


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
make3DAnyBox(IgCollection **collections, IgAssociationSet **, SoSeparator *sep)
{
  IgCollection *c = collections[0];
  IgDrawTowerHelper drawTowerHelper (sep);
  
  for (IgCollectionIterator ci = c->begin(), ce = c->end(); ci != ce; ++ci)
    {  
      IgV3d f1  = ci->get<IgV3d> ("front_1");
      IgV3d f2  = ci->get<IgV3d> ("front_2");
      IgV3d f3  = ci->get<IgV3d> ("front_3");
      IgV3d f4  = ci->get<IgV3d> ("front_4");
      
      IgV3d b1  = ci->get<IgV3d> ("back_1");
      IgV3d b2  = ci->get<IgV3d> ("back_2");
      IgV3d b3  = ci->get<IgV3d> ("back_3");
      IgV3d b4  = ci->get<IgV3d> ("back_4");
      
      drawTowerHelper.addTower (f1,f2,f3,f4, b1,b2,b3,b4);
    }
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
    corners.push_back (SbVec3f (p1.x(), p1.y(), p1.z () ));
    corners.push_back (SbVec3f (p2.x(), p2.y(), p2.z () ));
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






// ------------------------------------------------------
// Draw Tracker data  
// ------------------------------------------------------

// For hits drawn digis, clusters and rechits as different shapes
// Make them shades of same colour (more populous ones are darker)
// Do Si Strips one colour and pixels another  

// Tons of these, rarely drawn, make very unobtrusive
static void
make3DPixelDigis(IgCollection **collections, IgAssociationSet **assocs, SoSeparator *sep)
{
  make3DPointSetShapes(collections, assocs, sep, SbColor(0.2,0.0, 0.0), SoMarkerSet::PLUS_5_5);
}
static void
make3DSiStripDigis(IgCollection **collections, IgAssociationSet **assocs, SoSeparator *sep)
{
  make3DPointSetShapes(collections, assocs, sep, SbColor(0.2, 0.1, 0.0),SoMarkerSet::PLUS_5_5);
}


// Quite a lot of these, not often drawn, make quite unobtrusive
static void
make3DSiPixelClusters(IgCollection **collections, IgAssociationSet **assocs, SoSeparator *sep)
{
  make3DPointSetShapes(collections, assocs, sep, SbColor(0.4, 0.0, 0.0), SoMarkerSet::CROSS_5_5);
}
static void
make3DSiStripClusters(IgCollection **collections, IgAssociationSet **assocs, SoSeparator *sep)
{
  make3DPointSetShapes(collections, assocs, sep, SbColor(0.4, 0.2, 0.0), SoMarkerSet::CROSS_5_5);
}


// If you draw any tracekr hits its usually these ones so make more visible
static void
make3DSiPixelRecHits(IgCollection **collections, IgAssociationSet **assocs, SoSeparator *sep)
{
  make3DPointSetShapes(collections, assocs, sep, SbColor(0.7, 0.0, 0.0), SoMarkerSet::SQUARE_LINE_5_5);
}

static void
make3DTrackingRecHits(IgCollection **collections, IgAssociationSet **assocs, SoSeparator *sep)
{
  make3DPointSetShapes(collections, assocs, sep, SbColor(0.7, 0.4, 0.0), SoMarkerSet::SQUARE_LINE_5_5);
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
  
  mat->diffuseColor = SbColor(0.8, 0.8, 0.5);
  sty->style = SoDrawStyle::LINES;
  sty->lineWidth = 2;

  sep->addChild (mat);
  sep->addChild (sty);
  sep->addChild (vsep);

  vmat->diffuseColor = SbColor(0.9, 1.0, 0.3);
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
    tpoints->markerIndex = SoMarkerSet::SQUARE_LINE_5_5;
    tpoints->vertexProperty = tvertices;
    tpoints->numPoints.setValue (nVtx);

    sep->addChild (trackRep);
    sep->addChild (tpoints);
  }

  vertices->vertex.setNum (nv);
  mpoints->markerIndex = SoMarkerSet::CROSS_7_7;
  mpoints->vertexProperty.setValue (vertices);
  mpoints->numPoints.setValue (nv);
  vsep->addChild (mpoints);
}



// ------------------------------------------------------
// Draw Calorimeter data  
// ------------------------------------------------------


// FIXME LT: can still generalise the following a bit more 
// FIXME LT: make single box 3Dbox draw function with a bunch of switches for 
// FIXME LT: -- render styles: e.g. draw faces, draw outlines, draw both, front face only, etc.
// FIXME LT: -- shape styles:  e.g. scaled box, scaled tower, stacked tower, no scaling 
// FIXME LT:                   centred box, from centre towards front [/back], etc.


static void
make3DEnergyBoxes(IgCollection **collections, IgAssociationSet **, SoSeparator *sep)
{
  IgCollection		*c = collections[0];
  float minEnergy     = 0.2;   // GeV  FIXME LT: should get it from some service
  float maxEnergy     = 5.0;  // GeV  Not a cut -- just used to set max box size
  

  // FIXME: can compress the following code

  for (IgCollectionIterator ci = c->begin(), ce = c->end(); ci != ce; ++ci)
    {
      double energy = ci->get<double> ("energy");
      if (energy > maxEnergy)
	{ 
	  maxEnergy = energy;
	    }
    }
  

  IgDrawTowerHelper drawTowerHelper (sep);

  for (IgCollectionIterator ci = c->begin(), ce = c->end(); ci != ce; ++ci)
    {
      double energy = ci->get<double> ("energy");
      
      if (energy > minEnergy)
	{ 
	  IgV3d f1  = ci->get<IgV3d> ("front_1");
	  IgV3d f2  = ci->get<IgV3d> ("front_2");
	  IgV3d f3  = ci->get<IgV3d> ("front_3");
	  IgV3d f4  = ci->get<IgV3d> ("front_4");
	  
	  IgV3d b1  = ci->get<IgV3d> ("back_1");
	  IgV3d b2  = ci->get<IgV3d> ("back_2");
	  IgV3d b3  = ci->get<IgV3d> ("back_3");
	  IgV3d b4  = ci->get<IgV3d> ("back_4");
	  
 	  drawTowerHelper.addScaledBox (f1,f2,f3,f4, b1,b2,b3,b4, energy/maxEnergy);
	}
    }
}


static void
make3DEnergyTowers(IgCollection **collections, IgAssociationSet **, SoSeparator *sep)
{
  IgCollection		*c = collections[0];
  float energyScaleFactor = 0.03;  // m/GeV    FIXME LT: should get it from some service
  float minEnergy     = 0.2;  // GeV      FIXME LT: should get it from some service
  
  IgDrawTowerHelper drawTowerHelper (sep);

  for (IgCollectionIterator ci = c->begin(), ce = c->end(); ci != ce; ++ci)
    {
      double energy = ci->get<double> ("energy");
      
      if (energy > minEnergy)
	{ 
	  IgV3d f1  = ci->get<IgV3d> ("front_1");
	  IgV3d f2  = ci->get<IgV3d> ("front_2");
	  IgV3d f3  = ci->get<IgV3d> ("front_3");
	  IgV3d f4  = ci->get<IgV3d> ("front_4");
	  
	  IgV3d b1  = ci->get<IgV3d> ("back_1");
	  IgV3d b2  = ci->get<IgV3d> ("back_2");
	  IgV3d b3  = ci->get<IgV3d> ("back_3");
	  IgV3d b4  = ci->get<IgV3d> ("back_4");
	  
	  drawTowerHelper.addTower (f1,f2,f3,f4, 
				     b1,b2,b3,b4, 
				     energy, 
				     energyScaleFactor);
	}
    }
}





static void
make3DEmCaloTowerShapes(IgCollection **collections, IgAssociationSet **, SoSeparator *sep)
{
  IgCollection		*c = collections[0];
  float energyScaleFactor = 0.04; // m/GeV    FIXME LT: should get it from some service
  float minimumEnergy     = 0.5;  // GeV      FIXME LT: should get it from some service
  
  IgDrawTowerHelper drawTowerHelper (sep);

  for (IgCollectionIterator ci = c->begin(), ce = c->end(); ci != ce; ++ci)
    {
      double energy = ci->get<double> ("emEnergy");
      
      if (energy > minimumEnergy)
	{ 
	  IgV3d f1  = ci->get<IgV3d> ("front_1");
	  IgV3d f2  = ci->get<IgV3d> ("front_2");
	  IgV3d f3  = ci->get<IgV3d> ("front_3");
	  IgV3d f4  = ci->get<IgV3d> ("front_4");
	  
	  IgV3d b1  = ci->get<IgV3d> ("back_1");
	  IgV3d b2  = ci->get<IgV3d> ("back_2");
	  IgV3d b3  = ci->get<IgV3d> ("back_3");
	  IgV3d b4  = ci->get<IgV3d> ("back_4");
	  
	  drawTowerHelper.addTower (f1,f2,f3,f4, 
				    b1,b2,b3,b4, 
				    energy, 
				    energyScaleFactor);
	}
    }
}

static void
make3DEmPlusHadCaloTowerShapes(IgCollection **collections, IgAssociationSet **, SoSeparator *sep)
{
  IgCollection		*c = collections[0];
  float energyScaleFactor = 0.04; // m/GeV    FIXME LT: should get it from some service
  float minimumEnergy     = 0.5;  // GeV      FIXME LT: should get it from some service
  
  IgDrawTowerHelper drawTowerHelper (sep);
  
  for (IgCollectionIterator ci = c->begin(), ce = c->end(); ci != ce; ++ci)
    {
      double energy = ci->get<double> ("emEnergy") + ci->get<double> ("hadEnergy");
      
      if (energy > minimumEnergy)
	{ 
	  IgV3d f1  = ci->get<IgV3d> ("front_1");
	  IgV3d f2  = ci->get<IgV3d> ("front_2");
	  IgV3d f3  = ci->get<IgV3d> ("front_3");
	  IgV3d f4  = ci->get<IgV3d> ("front_4");
	  
	  IgV3d b1  = ci->get<IgV3d> ("back_1");
	  IgV3d b2  = ci->get<IgV3d> ("back_2");
	  IgV3d b3  = ci->get<IgV3d> ("back_3");
	  IgV3d b4  = ci->get<IgV3d> ("back_4");
	  
	  drawTowerHelper.addTower (f1,f2,f3,f4, 
				    b1,b2,b3,b4, 
				    energy, 
				    energyScaleFactor);
	}
    }
}


static void
make3DEcalRecHits(IgCollection **collections, IgAssociationSet **assocs, SoSeparator *sep)
{
  SoMaterial *mat = new SoMaterial;
  mat->diffuseColor.setValue(1.0, 0.2, 0.7);
  sep->addChild (mat);
  make3DEnergyTowers(collections, assocs, sep);
}

static void
make3DHcalRecHits(IgCollection **collections, IgAssociationSet **assocs, SoSeparator *sep)
{
  SoMaterial *mat = new SoMaterial;
  mat->diffuseColor = SbColor(0.3, 0.8, 1.0);
  mat->transparency = 0.0;
  sep->addChild (mat);
  make3DEnergyBoxes(collections, assocs, sep);
}

static void
make3DCaloTowers(IgCollection **collections, IgAssociationSet **assocs, SoSeparator *sep)
{
  SoMaterial	*emat = new SoMaterial;
  SoMaterial	*hmat = new SoMaterial;

  // FIXME LT: now we draw EM+Had tower first then Em tower after (over the top of first)
  // FIXME LT: but sides are co-planar so can get funny rendering effects
  // FIXME LT: fix it by drawing properly 2 stacked towers instead (first Em then Had on top)
  // FIXME LT: *** think how to render so not confused with rec hits *** e.g. wireframe?

  hmat->diffuseColor.setValue(0.1, 0.5, 0.5);
  hmat->transparency = 0.5;
  sep->addChild (hmat);
  make3DEmPlusHadCaloTowerShapes(collections, assocs, sep);

  emat->diffuseColor.setValue(0.5, 0.1, 0.35);
  emat->transparency = 0.5;
  sep->addChild (emat);
  make3DEmCaloTowerShapes(collections, assocs, sep);
}

static void 
make3DJet (SoGroup* sep, double et, double theta, double phi)
{
  // FIXME LT: this code not yet used. 
  // FIXME LT: it is a first attempt to merge in IgSoJet because
  // FIXME LT: (1) it has a bug and (2) 


  // FIXME LT: this jet drawing is utter crap (both physics and graphics)

  std::cout << "et=" << et << " theta=" << theta << " phi=" << phi << "\n" << std::endl;
  
  SoSeparator	*body = new SoSeparator;
  SoTransform	*bodyTrans = new SoTransform;
  SoCone	*bodyCone = new SoCone;
  SoSeparator	*hat = new SoSeparator;
  SoTransform	*hatTrans = new SoTransform;
  SoCone	*hatCone = new SoCone;

  float thrust =1.0;
  float maxZ=      4.0;               // set these to something more sensible ...
  float maxR=      2.0 ;
  float maxEnergy=100.0 ;

  
  // private data members

  double	ct = cos (theta);
  double	st = sin (theta);
  double	cp = cos (phi);
  double	sp = sin (phi);
  
  // Define cone rotations and translations; rotVec is the normal of
  // the thrust-y plane and alpha the angle between the y and thrust
  // axes.
  SbVec3f	rotVec (ct, 0.0, -st * cp);
  float	alpha = acos (st * sp);
  
  // The body cone of the jet (FIXME: Set radius to something
  // meaningful; first attempt to make bodyRadius somehow related to
  // thrust.)  Move cone on its head (M_PI rotation) and vertex at
  // the origin
  float	length1 = ct ? maxZ / fabs (ct) : maxZ;
  float	length2 = st ? maxR / fabs (st) : maxR;
  float	bodyHeight = length1 < length2 ? length1 : length2;
  float	bodyRadius = 0.3 * (1.0 / (thrust + 0.001));
  
  bodyCone->bottomRadius = bodyRadius;
  bodyCone->height = bodyHeight;
  bodyTrans->rotation.setValue (rotVec, alpha + M_PI);
  bodyTrans->translation = SbVec3f (bodyHeight * st * cp / 2.0,
				    bodyHeight * st * sp / 2.0,
				    bodyHeight * ct / 2.0);
  
  // The pointy "hat" on top of the jet.  (FIXME: Set height to
  // something meaningful; first attempt to relate hat height to
  // energy.  For now, hat height varies from 0 up to maxR,
  // depending on what max energy is set to.)
  float	hatRadius = 1.4 * bodyRadius;
  float	hatHeight = maxR * et / maxEnergy ;
  
  hatCone->bottomRadius = hatRadius;
  hatCone->height = hatHeight;
  hatTrans->rotation.setValue (rotVec, alpha);
  hatTrans->translation = SbVec3f ((bodyHeight + hatHeight / 2.0) * st * cp,
				   (bodyHeight + hatHeight / 2.0) * st * sp,
				   (bodyHeight + hatHeight / 2.0) * ct);
  
  sep->addChild (body);
  sep->addChild (bodyTrans);
  sep->addChild (bodyCone);

  sep->addChild (hat);
  sep->addChild (hatTrans);
  sep->addChild (hatCone);

}



static void
make3DJetShapes(IgCollection **collections, IgAssociationSet **, SoSeparator *sep)
{
  IgCollection		*c = collections[0];
  double		ecut = 5.0;  // FIXME LT: get value from some service 

  for (IgCollectionIterator ci = c->begin(), ce = c->end(); ci != ce; ++ci)
  {
    double et    = ci->get<double> ("et");
    double theta = ci->get<double> ("theta");
    double phi   = ci->get<double> ("phi");

    if (et > ecut)
    {
      IgSoJet *recoJet = new IgSoJet;
      recoJet->theta.setValue  (theta);
      recoJet->phi.setValue    (phi);
      recoJet->energy.setValue (et);
      sep->addChild (recoJet);

//    make3DJet (sep, et, theta, phi);   // FIXME LT: this should replace above lines but does not yet work

    }
  }
}


static void
make3DJets(IgCollection **collections, IgAssociationSet **assocs, SoSeparator *sep)
{
  SoMaterial *mat = new SoMaterial;
  mat->diffuseColor = SbColor (1.0, 1.0, 1.0);
  mat->transparency = 0.8;
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
  float etRadius = 8.0; // radius in x,y, to draw Etmiss vectors --- FIXME: calculate based on scene ??? 


  mat->diffuseColor = SbColor(0.8, 0.8, 0.8);
  sep->addChild(mat);

  sty->style = SoDrawStyle::LINES;
  sty->lineWidth = 3;
  dashed->linePattern = 0x0f0f;
  sep->addChild (sty);

  sep->addChild (ann);

  dashed->style = SoDrawStyle::LINES;
  dashed->lineWidth = 3;
  dashed->linePattern = 0x0f0f;
  ann->addChild (dashed);

  SbVec3f direction (0.,0.,0.);
  float etMiss = -999.; 

  for (IgCollectionIterator ci = c->begin(), ce = c->end(); ci != ce; ++ci)
  {
    points.push_back (SbVec3f (0., 0., 0.));

    float px = ci->get<double>("px"); 
    float py = ci->get<double>("py"); 
    float pz = 0.0; 

    direction.setValue (px, py, pz);
    etMiss = direction.length();    
    direction.normalize();
    direction *=etRadius;

    points.push_back (direction);
    lineIndices.push_back (i);
    lineIndices.push_back (i + 1);
    lineIndices.push_back (SO_END_LINE_INDEX);
    i += 2;
  }

  if (etMiss > 0) 
    {
      vertices->vertex.setValues (0, points.size (), &points [0]);
      vertices->vertex.setNum (points.size ());
      
      lineSet->coordIndex.setValues (0, lineIndices.size (), &lineIndices [0]);
      lineSet->vertexProperty = vertices;
      
      sep->addChild (lineSet);
      ann->addChild (lineSet);
      
      // Add text label a bit past the end of the line

      direction *=1.05; 
      
      SoTranslation *textPos = new SoTranslation;
      textPos->translation = direction;
      
      SoText2 *label = new SoText2;
      label->justification.setValue (SoText2::CENTER);

      char buf [128];

      //      std::string run  = std::string("Run number") + (sprintf(buf, "%d", e.get<int>("run")), buf);

      std::string text = std::string("Et miss=")   + (sprintf(buf, "%4.1f", etMiss),buf);
      label->string = text.c_str ();
      
      sep->addChild (textPos);
      sep->addChild (label);
    }
}



// ------------------------------------------------------
// Draw Muon data  
// ------------------------------------------------------




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

    points.push_back (SbVec3f (p1.x(), p1.y(), p1.z() ));
    points.push_back (SbVec3f (p2.x(), p2.y(), p2.z() ));
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
  points->markerIndex = SoMarkerSet::SQUARE_LINE_5_5;
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

  //  vertices->materialBinding = SoVertexProperty::OVERALL;
  //  vertices->orderedRGBA = 0x0000FFFF;

  wdrawStyle->style = SoDrawStyle::LINES;
  wdrawStyle->lineWidth.setValue(2.0);
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
make3DRPCRecHits(IgCollection **collections, IgAssociationSet **, SoSeparator *sep)
{

  IgCollection		*c = collections[0];

  SoDrawStyle		*drawStyle  = new SoDrawStyle;
  SoVertexProperty	*vertices = new SoVertexProperty;
  SoIndexedLineSet	*lineSet = new SoIndexedLineSet;
  std::vector<int>	lineIndices;
  std::vector<SbVec3f>	points;
  int			i = 0;

  SoMaterial		*mat = new SoMaterial;

  mat->diffuseColor = SbColor (0xff/255., 0xff/255., 0x00/255.);
  sep->addChild (mat);
  drawStyle->lineWidth   = 3;
  drawStyle->linePattern = 0xffff;    // 0xffff = solid
  sep->addChild (drawStyle);



  for (IgCollectionIterator ci = c->begin(), ce = c->end(); ci != ce; ++ci)
  {
    IgV3d u1 = ci->get<IgV3d>("u1");
    IgV3d u2 = ci->get<IgV3d>("u2");
    IgV3d v1 = ci->get<IgV3d>("v1");
    IgV3d v2 = ci->get<IgV3d>("v2");
    IgV3d w1 = ci->get<IgV3d>("w1");
    IgV3d w2 = ci->get<IgV3d>("w2");

    points.push_back (SbVec3f ( u1.x(), u1.y(), u1.z() ));
    points.push_back (SbVec3f ( u2.x(), u2.y(), u2.z() ));
    points.push_back (SbVec3f ( v1.x(), v1.y(), v1.z() ));
    points.push_back (SbVec3f ( v2.x(), v2.y(), v2.z() ));
    points.push_back (SbVec3f ( w1.x(), w1.y(), w1.z() ));
    points.push_back (SbVec3f ( w2.x(), w2.y(), w2.z() ));

    lineIndices.push_back (i);
    lineIndices.push_back (i + 1);
    lineIndices.push_back (SO_END_LINE_INDEX);
    lineIndices.push_back (i + 2);
    lineIndices.push_back (i + 3);
    lineIndices.push_back (SO_END_LINE_INDEX);
    lineIndices.push_back (i + 4);
    lineIndices.push_back (i + 5);
    lineIndices.push_back (SO_END_LINE_INDEX);

    i += 6;
  }

  vertices->vertex.setValues (0, points.size (), &points [0]);
  vertices->vertex.setNum (points.size ());

  lineSet->coordIndex.setValues (0, lineIndices.size (), &lineIndices [0]);
  lineSet->vertexProperty = vertices;

  sep->addChild (lineSet);
}


static void
make3DMuons(IgCollection **collections, IgAssociationSet **assocs, SoSeparator *sep)
{
  IgCollection		*muons = collections[0];
  IgCollection		*points = collections[1];
  IgAssociationSet	*assoc = assocs[0];
  SoMaterial		*mat = new SoMaterial;
  
  mat->diffuseColor = SbColor(1.0, 0.2, 0.0);
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
    m_timer (new QTimer (this)),
    m_networkManager (new QNetworkAccessManager),
    m_progressDialog (0)
{
  m_archives[0] = m_archives[1] = 0;
  m_storages[0] = new IgDataStorage;
  m_storages[1] = new IgDataStorage;

#ifndef Q_WS_MAC
  QCoreApplication::setApplicationName ("ispy");
#else
  QCoreApplication::setApplicationName ("iSpy");
#endif
  QCoreApplication::setApplicationVersion ("1.1");
  QCoreApplication::setOrganizationDomain ("iguana");
  QCoreApplication::setOrganizationName ("iguana");

  if (QDir::home ().isReadable ())
    defaultSettings ();

// For now, draw the detector with the default "make3DAnyBox" code but explicit state 
// this in each case so lables in visibility widget a nice than default ones 

  collection("CMS Tracker",
	     "Tracker_V1:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
	     0,
	     0,
	     make3DAnyBox);

  collection("CMS ECAL Barrel",
	     "EcalBarrel_V1:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
	     0,
	     0,
	     make3DAnyBox);

  collection("CMS ECAL Endcap",
	     "EcalEndcap_V1:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
	     0,
	     0,
	     make3DAnyBox);

  collection("CMS Preshower",
	     "EcalPreshower_V1:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
	     0,
	     0,
	     make3DAnyBox);

  collection("CMS HCAL Barrel",
	     "HcalBarrel_V1:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
	     0,
	     0,
	     make3DAnyBox);

  collection("CMS HCAL Endcap",
	     "HcalEndcap_V1:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
	     0,
	     0,
	     make3DAnyBox);

  collection("CMS HCAL Outer",
	     "HcalOuter_V1:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
	     0,
	     0,
	     make3DAnyBox);

  collection("CMS HCAL Forward",
	     "HcalForward_V1:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
	     0,
	     0,
	     make3DAnyBox);

  collection("CMS Drift Tubes",
	     "DTs_V1:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
	     0,
	     0,
	     make3DAnyBox);

  collection("CMS Cathode Strip Chambers",
	     "CSC_V1:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
	     0,
	     0,
	     make3DAnyBox);

  collection("CMS Resistive Plate Chambers",
	     "RPC_V1:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
	     0,
	     0,
	     make3DAnyBox);


  collection("Event information",
	     "Event_V1:time:run:event:ls:orbit:bx",
	     0,
	     0,
	     make3DEvent);

  collection("Pixel Digis",
	     "PixelDigis_V1:pos",
	     0,
	     0,
	     make3DPixelDigis);

  collection("Si Pixel Clusters",
	     "SiPixelClusters_V1:pos",
	     0,
	     0,
	     make3DSiPixelClusters);

  collection("Si Pixel Rec. Hits",
	     "SiPixelRecHits_V1:pos",
	     0,
	     0,
	     make3DSiPixelRecHits);

  collection("SiStripCluster",
	     "SiStripClusters_V1:pos",
	     0,
	     0,
	     make3DSiStripClusters);

  collection("Si Strip Digis",
	     "SiStripDigis_V1:pos",
	     0,
	     0,
	     make3DSiStripDigis);

  collection("Tracks",
	     "Tracks_V1:pt:pos:dir",
	     "Extras_V1:pos_1:dir_1:pos_2:dir_2",
	     "TrackExtras_V1",
	     make3DTracks);

  collection("Tracking Rec. Hits",
	     "TrackingRecHits_V1:pos",
	     0,
	     0,
	     make3DTrackingRecHits);

  collection("ECAL Rec. Hits",
	     "EcalRecHits_V1:energy:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
	     0,
	     0,
	     make3DEcalRecHits);

  collection("HB Rec. Hits",
	     "HBRecHits_V1:energy:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
	     0,
	     0,
	     make3DHcalRecHits);

  collection("HE Rec. Hits",
	     "HERecHits_V1:energy:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
	     0,
	     0,
	     make3DHcalRecHits);

  collection("HF Rec. Hits",
	     "HFRecHits_V1:energy:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
	     0,
	     0,
	     make3DHcalRecHits);

  collection("HO Rec. Hits",
	     "HORecHits_V1:energy:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
	     0,
	     0,
	     make3DHcalRecHits);

  collection("Calorimeter Energy Towers",
	     "CaloTowers_V1:emEnergy:hadEnergy:front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4",
	     0,
	     0,
	     make3DCaloTowers);

  collection("Jets",
	     "Jets_V1:et:theta:phi",
	     0,
	     0,
	     make3DJets);

  collection("Et Missing",
	     "METs_V1:pt:px:py:phi",
	     0,
	     0,
	     make3DMET);


  collection("DT Digis",
	     "DTDigis_V1:pos:axis:angle:cellWidth:cellLength:cellWidth:cellHeight",
	     0,
	     0,
	     make3DDTDigis);

  collection("DT Rec. Hits",
	     "DTRecHits_V1:lPlusGlobalPos:lMinusGlobalPos:rPlusGlobalPos:rMinusGlobalPos"
	     ":lGlobalPos:rGlobalPos:wirePos:axis:angle:cellWidth:cellLength:cellHeight",
	     0,
	     0,
	     make3DDTRecHits);

  collection("DT Rec. Segments (4D)",
	     "DTRecSegment4D_V1:pos_1:pos_2",
	     0,
	     0,
	     make3DDTRecSegment4D);

  collection("CSC Segments",
	     "CSCSegments_V1:pos_1:pos_2",
	     0,
	     0,
	     make3DCSCSegments);

  collection("RPC Rec. Hits",
	     "RPCRecHits_V1:u1:u2:v1:v2:w2",
	     0,
	     0,
	     make3DRPCRecHits);

  collection("Muon Tracks",
	     "Muons_V1:pt:charge:rp:phi:eta",
	     "Points_V1:pos",
	     "MuonTrackerPoints_V1",
	     make3DMuons);

  // Don't draw the following 

  collection("Not drawn: Extras_V1","Extras_V1",0,0,NULL);
  collection("Not drawn: Hits_V1","Hits_V1",0,0,NULL);
  collection("Not drawn: Points_V1","Points_V1",0,0,NULL);
  collection("Not drawn: DetIds_V1","DetIds_V1",0,0,NULL);

  
  // Default drawing operations if none of the above explicitly matched
  
  collection(0, ":front_1:front_2:front_3:front_4:back_1:back_2:back_3:back_4", 0, 0, make3DAnyBox);
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
  m_mainWindow = new ISpyMainWindow (this);

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
  headers << "Collection" << "Items" << "Show";
  m_treeWidget = new QTreeWidget(m_mainWindow->dockTreeWidgetContents);

  m_treeWidget->setHeaderLabels (headers);
  m_treeWidget->setColumnWidth (1, 90);
  m_treeWidget->setColumnWidth (2, 30);
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

void
ISpyApplication::showAbout(void)
{
  m_splash->showAbout();
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
  QObject::connect(this, SIGNAL(showMessage (const QString &)), m_mainWindow->statusBar (), SLOT(showMessage (const QString &)));
  QObject::connect (&filter, SIGNAL(open (const QString &)),this, SLOT(open (const QString &)));
  app.installEventFilter (&filter);

  // Activate but do not show the main window yet. We want to show
  // it only once we know what to do with the splash screen.
  mainwin->show ();
  m_mainWindow->actionSave->setEnabled (true);
  m_mainWindow->actionPrint->setEnabled (true);

  // Process pending events now to allow file open events through
  // (mac), calling "open()" on the files. Exclude user input and
  // don't wait for events to occur.
  evloop.processEvents(QEventLoop::ExcludeUserInputEvents);

  // Open file names given on the command line (unix, windows).
  for (int i = 1; i < m_argc; ++i)
    open(m_argv[i]);

  // If we don't have any files open, show the splash screen as a file
  // opening wizard. The file open dialog will eventually show the main
  // window. If we opened files above just make the "About iSpy" splash
  // screen visible for a few seconds, show the main window and proceed
  // immediatly to the main app event loop.
  m_splash = new ISpySplashScreen(this);
  QObject::connect(m_mainWindow->actionOpenWizard, SIGNAL(triggered()), 
                   m_splash, SLOT(showWizard()));
  
  if (! m_archives[0] && ! m_archives[1])
    m_splash->showWizard();
  else
    m_mainWindow->show();

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

  // If we didn't show the main window yet, show it now
  if (! m_mainWindow->isVisible())
    m_mainWindow->show ();

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
      if ((*zi)->isDirectory() || (*zi)->size(ZipMember::UNCOMPRESSED) == 0)
	continue;
      else if (! strncmp((*zi)->name(), "Geometry/", 9))
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

  if (m_splash && m_splash->isVisible())
  {
    m_mainWindow->show();
    m_splash->hide();
  }
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

void 
ISpyApplication::handleWizardLinks (const QUrl &link)
{
    QString linkString = link.toString();
    
    if (linkString == "OpenUrl")
    {
        m_splash->hide ();
        m_mainWindow->show ();
        m_mainWindow->raise ();
        openUrlDialog();
    }
    else if (linkString == "OpenWeb")
    {
        m_splash->setRightPage(QUrl("http://iguana.web.cern.ch/iguana/ispy/igfiles/"));
    }
    else if (linkString == "OpenFile")
    {
        m_splash->hide ();
        m_mainWindow->show ();
        this->openFileDialog ();
    }
    else if (linkString.endsWith (".ig"))
    {
        m_splash->hide ();
        m_mainWindow->show ();
        this->downloadFile (m_splash->rightPage()
                                     .toString()
                                     .replace(QRegExp("/[?].*"), "/") + linkString);
    }
    else if (linkString.startsWith ("mailto:"))
    {
        QDesktopServices::openUrl (link);
    }
    else if (linkString.startsWith("http://"))
    {
        QDesktopServices::openUrl (link);
    }
    else
    {
        QUrl baseUrl = m_splash->rightPage();
        if (linkString[0] != '?' && !linkString.endsWith("/"))
        {
            std::cerr << linkString.replace(QRegExp("/[?].*"), "/").toStdString()<< std::endl;
            QDesktopServices::openUrl (baseUrl
                                       .toString()
                                       .replace(QRegExp("/[?].*"), "/")
                                       + linkString);
            return;
        }
        if (linkString[0] == '/')
        {
            baseUrl.setPath (linkString);
            linkString = baseUrl.toString().replace(QRegExp("/[?].*"), "/");   
        }
        else
            linkString = baseUrl
                        .toString()
                        .replace(QRegExp("/[?].*"), "/") + linkString;
        m_splash->setRightPage(linkString);
    }
}

void
ISpyApplication::downloadFile (const QUrl &link)
{
  if (m_splash)
  {
    m_splash->hide ();
  }

  QNetworkReply *reply = getUrl(link);

  // FIXME: pickup download location from QSettings, hardcode to Desktop for the time being.
  QString saveDir = QDir::homePath();
  if (QDir::home().exists("Desktop"))
  {
    saveDir = QDir::home().filePath("Desktop");
  }
  
  QString savedFileName = link.toString().replace(QRegExp(".*/"), saveDir + "/");
  IgNetworkReplyHandler *handler = new IgNetworkReplyHandler (reply,
                                                              new QFile (savedFileName));
  QObject::connect (handler, SIGNAL(done(IgNetworkReplyHandler *)), 
                    this, SLOT(fileDownloaded(IgNetworkReplyHandler *)));
  
  if (!m_progressDialog)
     m_progressDialog = new QProgressDialog;
  m_progressDialog->setLabelText ("Downloading " + savedFileName + "...");
  m_progressDialog->show();
  // FIXME: handle case for files larger than 2GB.
  QObject::connect (reply, SIGNAL(downloadProgress(qint64,qint64)), 
                    this, SLOT(setProgress(qint64,qint64)));
  QObject::connect (m_progressDialog, SIGNAL(canceled()),
                    handler, SLOT(abort()));
  QObject::connect (handler, SIGNAL(downloadAborted(IgNetworkReplyHandler *)),
                    this, SLOT(handleAbortedDownload(IgNetworkReplyHandler *)));
  QObject::connect (handler, SIGNAL(downloadError(IgNetworkReplyHandler *)),
                    this, SLOT(handleDownloadError(IgNetworkReplyHandler *)));
}

void
ISpyApplication::setProgress (qint64 current, qint64 final)
{
  if (current == final)
  {
     m_progressDialog->reset();
     m_progressDialog->hide();
     return;
  }  
    
  if (final > 0)
    m_progressDialog->setRange (0, final/1024);
  m_progressDialog->setValue (current/1024);
}

/** Helper functon to create QNetworkReplies which have iSpy user agent set*/
QNetworkReply *
ISpyApplication::getUrl (const QUrl &url)
{
  QNetworkRequest request;
  request.setUrl(url);
  request.setRawHeader("User-Agent", "iSpy 1.1");
  return m_networkManager->get(request);
}

/** Handles the case a file has been downloaded.*/
void
ISpyApplication::fileDownloaded (IgNetworkReplyHandler *handler)
{
  m_progressDialog->disconnect();
  QFile *file = static_cast<QFile *>(handler->device());
  try
  {
    open(file->fileName());
  }catch (...)
  {
    QMessageBox::critical(m_mainWindow, "Error while downloading file.", 
                          "Unable to open downloaded file: " + file->fileName ());
    showMessage("");
    //FIXME: handle various errors and in particula lat::ZError.
  }
}

/** Handles when a user clicks on the cancel button in the download dialog*/
void
ISpyApplication::handleAbortedDownload(IgNetworkReplyHandler *handler)
{
  handler->reply()->disconnect();
  handler->disconnect();
  m_progressDialog->reset();
  m_progressDialog->hide();
  showMessage("");
  if (handler->reply()->error())
  {
    QFile *file = static_cast<QFile *>(handler->device());
    QMessageBox::critical(m_mainWindow, "Error while downloading file.", 
                          "Cannot download file" + file->fileName());
  }
  delete handler;
}

void
ISpyApplication::handleDownloadError(IgNetworkReplyHandler *handler)
{
  m_progressDialog->reset();
  m_progressDialog->hide();
  showMessage("");
  QMessageBox::critical(m_mainWindow, "Error while downloading file.", 
                        "Cannot download from url:" + handler->reply()->request().url().toString());
}

/** Handles the open url action */
void
ISpyApplication::openUrlDialog (void)
{
    QInputDialog dialog;
    dialog.setLabelText("Specify an ig-file url:");
    dialog.setTextValue("http://iguana.web.cern.ch/iguana/ispy/igfiles/RelValWjet.ig");
    dialog.resize(430,72);
    // FIXME: use the latest file downloaded as default.
    if (!dialog.exec())
        return;

    this->downloadFile(dialog.textValue());
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
