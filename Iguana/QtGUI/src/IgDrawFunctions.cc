//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/QtGUI/interface/IgDrawFunctions.h"
#include "Iguana/QtGUI/interface/IgDrawFactoryService.h"
#include "Iguana/QtGUI/interface/ISpyReadService.h"
#include "Iguana/Framework/interface/IgCollection.h"
#include "Iguana/Inventor/interface/IgSbColorMap.h"
#include "Iguana/Inventor/interface/IgSoJet.h"
#include <Inventor/nodes/SoAnnotation.h>
#include <Inventor/nodes/SoIndexedFaceSet.h>
#include <Inventor/nodes/SoIndexedLineSet.h>
#include <Inventor/nodes/SoLineSet.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <QSettings>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

static const double cCrystalHitScale = 0.3;
static const double cCaloTowerScale = 0.03;



namespace 
{
    SoNode *
    drawJets (IgState *state, const std::string &collection,
	      const std::string &name,
	      const std::string &rep)
    {
	double ecut = 0.1;
	
	QSettings settings;
	if (settings.contains ("igevents/cuts/jets/energy"))
	{
	    ecut = settings.value ("igevents/cuts/jets/energy").value<double> ();
	}
	
	SoSeparator *sep = new SoSeparator;
	sep->setName (SbName (collection.c_str ()));

	ISpyReadService* readService = ISpyReadService::get (state);
	ASSERT (readService);

	IgDataStorage *storage = readService->dataStorage ();
	if (storage->hasCollection (collection.c_str ()))
	{	    
	    IgCollection &jets = storage->getCollection (collection.c_str ());
	
	    if (jets.size () > 0 && 
		jets.hasProperty ("et") && 
		jets.hasProperty ("eta") &&  
		jets.hasProperty ("theta") &&
		jets.hasProperty ("phi"))
	    {
		IgCollectionIterator cit = jets.begin ();
		IgCollectionIterator cend = jets.end ();
		for (; cit != cend ; cit++) 
		{
		    IgCollectionItem item = *cit;
		    double energy = item.get<double> ("et");
		    if (energy > ecut)
		    {
			IgSoJet *recoJet = new IgSoJet;
			recoJet->theta.setValue (item.get<double> ("theta"));
			recoJet->phi.setValue (item.get<double> ("phi"));
			recoJet->energy.setValue (energy);
			sep->addChild (recoJet);
		    }
		}
	    }
	}
	
	return sep;
    }





    
    SoNode *
    drawEmTowers (IgState *state, const std::string &collection,
		  const std::string &name,
		  const std::string &rep)
    {	    
	double ecut = 0.1;
	
	QSettings settings;
	if (settings.contains ("igevents/cuts/calotowers/energy"))
	{
	    ecut = settings.value ("igevents/cuts/calotowers/energy").value<double> ();
	}

	SoSeparator *sep = new SoSeparator;
	sep->setName (SbName (collection.c_str ()));

	ISpyReadService* readService = ISpyReadService::get (state);
	ASSERT (readService);

	IgDataStorage *storage = readService->dataStorage ();
	if (storage->hasCollection (collection.c_str ()))
	{	    
	    IgCollection &recHits = storage->getCollection (collection.c_str ());
	
	    if (recHits.size () > 0 && 
		recHits.hasProperty ("emEnergy") && 
		recHits.hasProperty ("front_1") &&  
		recHits.hasProperty ("front_2") &&
		recHits.hasProperty ("front_3") &&
		recHits.hasProperty ("front_4") &&
		recHits.hasProperty ("back_1") &&
		recHits.hasProperty ("back_2") &&
		recHits.hasProperty ("back_3") &&
		recHits.hasProperty ("back_4"))
	    {
		SoVertexProperty *vertices = new SoVertexProperty;
	    
		int i = 0;
		std::vector<int> lineIndices;
		std::vector<int> indices;
		std::vector<SbVec3f> corners;		

		float epsilon = 0.0001;
		//float scale = 1.0;
		// NOTE: Rough estimate
		float scale = cCaloTowerScale;
		
		IgCollectionIterator cit = recHits.begin ();
		IgCollectionIterator cend = recHits.end ();
		for (; cit != cend ; cit++) 
		{
		    IgCollectionItem item = *cit;
		    double energy = item.get<double> ("emEnergy");
		    if (energy > ecut)
		    {			
			float eScale = scale * energy;

			IgV3d p1  = item.get<IgV3d> ("front_1");
			IgV3d p2  = item.get<IgV3d> ("front_2");
			IgV3d p3  = item.get<IgV3d> ("front_3");
			IgV3d p4  = item.get<IgV3d> ("front_4");
			IgV3d p5  = item.get<IgV3d> ("back_1");
			IgV3d p6  = item.get<IgV3d> ("back_2");
			IgV3d p7  = item.get<IgV3d> ("back_3");
			IgV3d p8  = item.get<IgV3d> ("back_4");

			SbVec3f front1(static_cast<double>(p1.x ()), 
				       static_cast<double>(p1.y ()),
				       static_cast<double>(p1.z ()));		    
			//corners.push_back (front1);
		    
			SbVec3f front2(static_cast<double>(p2.x ()), 
				       static_cast<double>(p2.y ()),
				       static_cast<double>(p2.z ()));
			//corners.push_back (front2);

			SbVec3f front3(static_cast<double>(p3.x ()), 
				       static_cast<double>(p3.y ()),
				       static_cast<double>(p3.z ()));
			//corners.push_back (front3);
		    
			SbVec3f front4(static_cast<double>(p4.x ()), 
				       static_cast<double>(p4.y ()),
				       static_cast<double>(p4.z ()));
			//corners.push_back (front4);
		    
			SbVec3f back1(static_cast<double>(p5.x ()), 
				      static_cast<double>(p5.y ()),
				      static_cast<double>(p5.z ()));
			//corners.push_back (back1);

			SbVec3f back2(static_cast<double>(p6.x ()), 
				      static_cast<double>(p6.y ()),
				      static_cast<double>(p6.z ()));
			//corners.push_back (back2);

			SbVec3f back3(static_cast<double>(p7.x ()), 
				      static_cast<double>(p7.y ()),
				      static_cast<double>(p7.z ()));
			//corners.push_back (back3);

			SbVec3f back4(static_cast<double>(p8.x ()), 
				      static_cast<double>(p8.y ()),
				      static_cast<double>(p8.z ()));	    
			//corners.push_back (back4);

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
			// 		    corners.push_back (corners[i + 8]  + (eBack1 - eFront1) * eScale);
			// 		    corners.push_back (corners[i + 9]  + (eBack2 - eFront2) * eScale);
			// 		    corners.push_back (corners[i + 10] + (eBack3 - eFront3) * eScale);
			// 		    corners.push_back (corners[i + 11] + (eBack4 - eFront4) * eScale);
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
		
		SoIndexedLineSet *lineSet = new SoIndexedLineSet;
		lineSet->coordIndex.setValues (0, lineIndices.size (), &lineIndices [0]);
		lineSet->vertexProperty = vertices;

		sep->addChild (lineSet);
		
		SoIndexedFaceSet *faces = new SoIndexedFaceSet;
		faces->coordIndex.setValues (0, indices.size (), &indices [0]);
		faces->vertexProperty = vertices;
		sep->addChild (faces);
	    }
	}
	
	return sep;
    }







    SoNode *
    drawHadTowers (IgState *state, const std::string &collection,
		   const std::string &name,
		   const std::string &rep)
    {	    
	double ecut = 0.1;
	bool hlrMode = false;
	
	QSettings settings;
	if (settings.contains ("igevents/cuts/calotowers/energy"))
	{
	    ecut = settings.value ("igevents/cuts/calotowers/energy").value<double> ();
	}
	if (settings.contains ("igdisplay/hadtowers/view3d/hiddenlineremoval"))
	{
	    hlrMode = settings.value ("igdisplay/hadtower/view3d/hiddenlineremoval").value<bool> ();
	}

	SoSeparator *sep = new SoSeparator;
	sep->setName (SbName (collection.c_str ()));
	SoAnnotation *ann = 0;
	
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
	
	ISpyReadService* readService = ISpyReadService::get (state);
	ASSERT (readService);

	IgDataStorage *storage = readService->dataStorage ();
	if (storage->hasCollection (collection.c_str ()))
	{	    
	    IgCollection &recHits = storage->getCollection (collection.c_str ());
	
	    if (recHits.size () > 0 && 
		recHits.hasProperty ("emEnergy") && 
		recHits.hasProperty ("hadEnergy") && 
		recHits.hasProperty ("front_1") &&  
		recHits.hasProperty ("front_2") &&
		recHits.hasProperty ("front_3") &&
		recHits.hasProperty ("front_4") &&
		recHits.hasProperty ("back_1") &&
		recHits.hasProperty ("back_2") &&
		recHits.hasProperty ("back_3") &&
		recHits.hasProperty ("back_4"))
	    {
		SoVertexProperty *vertices = new SoVertexProperty;
	    
		int i = 0;
		std::vector<int> lineIndices;
		std::vector<int> indices;
		std::vector<SbVec3f> corners;		

		float epsilon = 0.0001;
		//float scale = 1.0;		
		float scale = cCaloTowerScale;
		
		IgCollectionIterator cit = recHits.begin ();
		IgCollectionIterator cend = recHits.end ();
		for (; cit != cend ; cit++) 
		{
		    IgCollectionItem item = *cit;
		    double energy = item.get<double> ("emEnergy") + item.get<double> ("hadEnergy");
		    if (energy > ecut)
		    {			
			float eScale = scale * energy;

			IgV3d p1  = item.get<IgV3d> ("front_1");
			IgV3d p2  = item.get<IgV3d> ("front_2");
			IgV3d p3  = item.get<IgV3d> ("front_3");
			IgV3d p4  = item.get<IgV3d> ("front_4");
			IgV3d p5  = item.get<IgV3d> ("back_1");
			IgV3d p6  = item.get<IgV3d> ("back_2");
			IgV3d p7  = item.get<IgV3d> ("back_3");
			IgV3d p8  = item.get<IgV3d> ("back_4");

			SbVec3f front1(static_cast<double>(p1.x ()), 
				       static_cast<double>(p1.y ()),
				       static_cast<double>(p1.z ()));		    
			//corners.push_back (front1);
		    
			SbVec3f front2(static_cast<double>(p2.x ()), 
				       static_cast<double>(p2.y ()),
				       static_cast<double>(p2.z ()));
			//corners.push_back (front2);

			SbVec3f front3(static_cast<double>(p3.x ()), 
				       static_cast<double>(p3.y ()),
				       static_cast<double>(p3.z ()));
			//corners.push_back (front3);
		    
			SbVec3f front4(static_cast<double>(p4.x ()), 
				       static_cast<double>(p4.y ()),
				       static_cast<double>(p4.z ()));
			//corners.push_back (front4);
		    
			SbVec3f back1(static_cast<double>(p5.x ()), 
				      static_cast<double>(p5.y ()),
				      static_cast<double>(p5.z ()));
			//corners.push_back (back1);

			SbVec3f back2(static_cast<double>(p6.x ()), 
				      static_cast<double>(p6.y ()),
				      static_cast<double>(p6.z ()));
			//corners.push_back (back2);

			SbVec3f back3(static_cast<double>(p7.x ()), 
				      static_cast<double>(p7.y ()),
				      static_cast<double>(p7.z ()));
			//corners.push_back (back3);

			SbVec3f back4(static_cast<double>(p8.x ()), 
				      static_cast<double>(p8.y ()),
				      static_cast<double>(p8.z ()));	    
			//corners.push_back (back4);

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
			// 		    corners.push_back (corners[i + 8]  + (eBack1 - eFront1) * eScale);
			// 		    corners.push_back (corners[i + 9]  + (eBack2 - eFront2) * eScale);
			// 		    corners.push_back (corners[i + 10] + (eBack3 - eFront3) * eScale);
			// 		    corners.push_back (corners[i + 11] + (eBack4 - eFront4) * eScale);
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
		
		SoIndexedLineSet *lineSet = new SoIndexedLineSet;
		lineSet->coordIndex.setValues (0, lineIndices.size (), &lineIndices [0]);
		lineSet->vertexProperty = vertices;

		sep->addChild (lineSet);
		
		if (hlrMode)
		{
		    ann->addChild (lineSet);
		}
		
		SoIndexedFaceSet *faces = new SoIndexedFaceSet;
		faces->coordIndex.setValues (0, indices.size (), &indices [0]);
		faces->vertexProperty = vertices;
		sep->addChild (faces);
	    }
	}
	
	return sep;
    }





    SoNode *
    drawCrystalHits (IgState *state, const std::string &collection,
		     const std::string &name,
		     const std::string &rep)

    // FIXME LT: rename if used also for HCAL   

    {	    

	double ecut = 0.1;
	bool hlrMode = false;
	
	QSettings settings;
	if (settings.contains ("igevents/cuts/ecal/barrel/rechits/energy"))
	{
	    ecut = settings.value ("igevents/cuts/ecal/barrel/rechits/energy").value<double> ();
	}
	if (settings.contains ("igdisplay/crystalhits/view3d/hiddenlineremoval"))
	{
	    hlrMode = settings.value ("igdisplay/crystalhits/view3d/hiddenlineremoval").value<bool> ();
	}
	
	SoSeparator *sep = new SoSeparator;
	sep->setName (SbName (collection.c_str ()));


	SoAnnotation *ann = 0;
	
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
	
	ISpyReadService* readService = ISpyReadService::get (state);
	ASSERT (readService);

	IgDataStorage *storage = readService->dataStorage ();
	if (storage->hasCollection (collection.c_str ()))
	{	    
	    IgCollection &recHits = storage->getCollection (collection.c_str ());
	
	    if (recHits.size () > 0 && 
		recHits.hasProperty ("energy") && 
		recHits.hasProperty ("front_1") &&  
		recHits.hasProperty ("front_2") &&
		recHits.hasProperty ("front_3") &&
		recHits.hasProperty ("front_4") &&
		recHits.hasProperty ("back_1") &&
		recHits.hasProperty ("back_2") &&
		recHits.hasProperty ("back_3") &&
		recHits.hasProperty ("back_4"))
	    {
		SoVertexProperty *vertices = new SoVertexProperty;
	    
		int i = 0;
		std::vector<int> lineIndices;
		std::vector<int> indices;
		std::vector<SbVec3f> corners;		

		float epsilon = 0.0001;
		//float scale = 1.0;
		// NOTE Rough estimate
		float scale = cCrystalHitScale;
		

		IgCollectionIterator cit = recHits.begin ();
		IgCollectionIterator cend = recHits.end ();
		for (; cit != cend ; cit++) 
		{
		    IgCollectionItem item = *cit;
		    double energy = item.get<double> ("energy");
		    if (energy > ecut)
		    {			
			float eScale = scale * energy;

			IgV3d p1  = item.get<IgV3d> ("front_1");
			IgV3d p2  = item.get<IgV3d> ("front_2");
			IgV3d p3  = item.get<IgV3d> ("front_3");
			IgV3d p4  = item.get<IgV3d> ("front_4");
			IgV3d p5  = item.get<IgV3d> ("back_1");
			IgV3d p6  = item.get<IgV3d> ("back_2");
			IgV3d p7  = item.get<IgV3d> ("back_3");
			IgV3d p8  = item.get<IgV3d> ("back_4");

			SbVec3f front1(static_cast<double>(p1.x ()), 
				       static_cast<double>(p1.y ()),
				       static_cast<double>(p1.z ()));		    
			//corners.push_back (front1);
		    
			SbVec3f front2(static_cast<double>(p2.x ()), 
				       static_cast<double>(p2.y ()),
				       static_cast<double>(p2.z ()));
			//corners.push_back (front2);

			SbVec3f front3(static_cast<double>(p3.x ()), 
				       static_cast<double>(p3.y ()),
				       static_cast<double>(p3.z ()));
			//corners.push_back (front3);
		    
			SbVec3f front4(static_cast<double>(p4.x ()), 
				       static_cast<double>(p4.y ()),
				       static_cast<double>(p4.z ()));
			//corners.push_back (front4);
		    
			SbVec3f back1(static_cast<double>(p5.x ()), 
				      static_cast<double>(p5.y ()),
				      static_cast<double>(p5.z ()));
			//corners.push_back (back1);

			SbVec3f back2(static_cast<double>(p6.x ()), 
				      static_cast<double>(p6.y ()),
				      static_cast<double>(p6.z ()));
			//corners.push_back (back2);

			SbVec3f back3(static_cast<double>(p7.x ()), 
				      static_cast<double>(p7.y ()),
				      static_cast<double>(p7.z ()));
			//corners.push_back (back3);

			SbVec3f back4(static_cast<double>(p8.x ()), 
				      static_cast<double>(p8.y ()),
				      static_cast<double>(p8.z ()));	    
			//corners.push_back (back4);

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
			// 		    corners.push_back (corners[i + 8]  + (eBack1 - eFront1) * eScale);
			// 		    corners.push_back (corners[i + 9]  + (eBack2 - eFront2) * eScale);
			// 		    corners.push_back (corners[i + 10] + (eBack3 - eFront3) * eScale);
			// 		    corners.push_back (corners[i + 11] + (eBack4 - eFront4) * eScale);
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
		
		SoIndexedLineSet *lineSet = new SoIndexedLineSet;
		lineSet->coordIndex.setValues (0, lineIndices.size (), &lineIndices [0]);
		lineSet->vertexProperty = vertices;

		sep->addChild (lineSet);
		
		if (hlrMode)
		{	    
		    ann->addChild (lineSet);
		}
		
		SoIndexedFaceSet *faces = new SoIndexedFaceSet;
		faces->coordIndex.setValues (0, indices.size (), &indices [0]);
		faces->vertexProperty = vertices;
		sep->addChild (faces);
	    }
	}

	
	return sep;
    }

    SoNode *
    drawCrystals (IgState *state, const std::string &collection,
		  const std::string &name,
		  const std::string &rep)
    {	    
	SoSeparator *sep = new SoSeparator;
	sep->setName (SbName (collection.c_str ()));

	bool hlrMode = false;

	QSettings settings;
	if (settings.contains ("igdisplay/crystals/view3d/hiddenlineremoval"))
	{
	    hlrMode = settings.value ("igdisplay/crystals/view3d/hiddenlineremoval").value<bool> ();
	}

	SoAnnotation *ann = 0;
	
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
	
	ISpyReadService* readService = ISpyReadService::get (state);
	ASSERT (readService);

	IgDataStorage *storage = readService->dataStorage ();
	if (storage->hasCollection (collection.c_str ()))
	{	    
	    IgCollection &crystals = storage->getCollection (collection.c_str ());
	
	    if (crystals.size () > 0 && 
		crystals.hasProperty ("front_1") &&  
		crystals.hasProperty ("front_2") &&
		crystals.hasProperty ("front_3") &&
		crystals.hasProperty ("front_4") &&
		crystals.hasProperty ("back_1") &&
		crystals.hasProperty ("back_2") &&
		crystals.hasProperty ("back_3") &&
		crystals.hasProperty ("back_4"))
	    {
		SoVertexProperty *vertices = new SoVertexProperty;
	    
		int i = 0;
		std::vector<int> lineIndices;
		std::vector<int> indices;
		std::vector<SbVec3f> corners;		

		IgCollectionIterator cit = crystals.begin ();
		IgCollectionIterator cend = crystals.end ();
		for (; cit != cend ; cit++) 
		{
		    IgCollectionItem item = *cit;

		    IgV3d p1  = item.get<IgV3d> ("front_1");
		    IgV3d p2  = item.get<IgV3d> ("front_2");
		    IgV3d p3  = item.get<IgV3d> ("front_3");
		    IgV3d p4  = item.get<IgV3d> ("front_4");
		    IgV3d p5  = item.get<IgV3d> ("back_1");
		    IgV3d p6  = item.get<IgV3d> ("back_2");
		    IgV3d p7  = item.get<IgV3d> ("back_3");
		    IgV3d p8  = item.get<IgV3d> ("back_4");

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
		
		SoIndexedLineSet *lineSet = new SoIndexedLineSet;
		lineSet->coordIndex.setValues (0, lineIndices.size (), &lineIndices [0]);
		lineSet->vertexProperty = vertices;

		sep->addChild (lineSet);
		if (hlrMode)
		{	    
		    ann->addChild (lineSet);
		}
		
		SoIndexedFaceSet *faces = new SoIndexedFaceSet;
		faces->coordIndex.setValues (0, indices.size (), &indices [0]);
		faces->vertexProperty = vertices;
		sep->addChild (faces);
	    }
	}
	
	return sep;
    }

    SoNode *
    drawSegments (IgState *state, const std::string &collection,
		  const std::string &name,
		  const std::string &rep)
    {	    
	SoSeparator *sep = new SoSeparator;
	sep->setName (SbName (collection.c_str ()));

	ISpyReadService* readService = ISpyReadService::get (state);
	ASSERT (readService);

	IgDataStorage *storage = readService->dataStorage ();
	if (storage->hasCollection (collection.c_str ()))
	{	    
	    IgCollection &segments = storage->getCollection (collection.c_str ());
	
	    if (segments.size () > 0 && 
		segments.hasProperty ("pos_1") &&  
		segments.hasProperty ("pos_2"))
	    {
		SoVertexProperty *vertices = new SoVertexProperty;
	    
		int i = 0;
		std::vector<int> lineIndices;
		std::vector<SbVec3f> points;		

		IgCollectionIterator cit = segments.begin ();
		IgCollectionIterator cend = segments.end ();
		for (; cit != cend ; cit++) 
		{
		    IgCollectionItem item = *cit;

		    IgV3d p1  = item.get<IgV3d> ("pos_1");
		    IgV3d p2  = item.get<IgV3d> ("pos_2");

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

		SoIndexedLineSet *lineSet = new SoIndexedLineSet;
		lineSet->coordIndex.setValues (0, lineIndices.size (), &lineIndices [0]);
		lineSet->vertexProperty = vertices;

		sep->addChild (lineSet);
	    }
	}
	return sep;
    }
}

IgDrawFunctions::IgDrawFunctions (IgState *state)
{
    init (state);
}

void
IgDrawFunctions::init (IgState *state) 
{
    IgDrawFactoryService *drawService = IgDrawFactoryService::get (state);
    if (! drawService)
    {
        drawService = new IgDrawFactoryService (state);
    }
    drawService->registerDrawFunction ("CrystalHits", &drawCrystalHits);
    drawService->registerDrawFunction ("Crystals", &drawCrystals);
    drawService->registerDrawFunction ("Segments", &drawSegments);
    drawService->registerDrawFunction ("EmTowers", &drawEmTowers);
    drawService->registerDrawFunction ("HadTowers", &drawHadTowers);
    drawService->registerDrawFunction ("Jets", &drawJets);
}
