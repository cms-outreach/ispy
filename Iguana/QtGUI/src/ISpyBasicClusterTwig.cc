#define QT_NO_EMIT

//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/QtGUI/interface/ISpyBasicClusterTwig.h"
#include "Iguana/QtGUI/interface/ISpyReadService.h"
#include "Iguana/Inventor/interface/IgSbColorMap.h"
#include "Iguana/Framework/interface/IgCollection.h"
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoMarkerSet.h>
#include <Inventor/nodes/SoPointSet.h>
#include <Inventor/nodes/SoVertexProperty.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include "Iguana/Inventor/interface/IgSoCrystalHit.h"
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

ISpyBasicClusterTwig::ISpyBasicClusterTwig (IgState *state, IgTwig *parent, 
					    const std::string &name)
    : ISpyQueuedTwig (state, parent, name)
{}

void
ISpyBasicClusterTwig::onNewEvent (ISpyEventMessage& message) 
{
    ISpyQueuedTwig::onNewEvent (message);
    
    QSettings settings;    
    QString visSettings ("igtwigs/visibility/");
    visSettings.append ("BasicClusters_V1");

    if (settings.contains (visSettings) && 
	Qt::CheckState (settings.value (visSettings).value<int> ()) == Qt::Unchecked)
	return;
    
    if (ISpyReadService *readService = ISpyReadService::get (state ()))
    {	
	IgDataStorage *storage = readService->dataStorage ();
	
	SoSeparator *sep = dynamic_cast<SoSeparator *>(ISpyQueuedTwig::rep ());
	sep->setName (SbName ("BasicClusters_V1"));
	
	if (storage->hasCollection ("BasicClusters_V1") && storage->hasCollection("CaloHits_V1"))
	{   
	    IgCollection& clusters = storage->getCollection("BasicClusters_V1");
	    IgCollection& calohits = storage->getCollection("CaloHits_V1");
	    IgAssociationSet assoc = storage->getAssociationSet("BasicClustersCaloHits_V1");

	    if ( clusters.size() > 0 && 
		 clusters.hasProperty("pos") &&
		 clusters.hasProperty("energy") &&
		 calohits.size() > 0 && 
		 calohits.hasProperty("fraction") &&
		 calohits.hasProperty("front_1") &&
		 calohits.hasProperty("front_2") &&
		 calohits.hasProperty("front_3") &&
		 calohits.hasProperty("front_4") &&
		 calohits.hasProperty("back_1") &&
		 calohits.hasProperty("back_2") &&
		 calohits.hasProperty("back_3") &&
		 calohits.hasProperty("back_4") )
	    {
		IgProperty POS = clusters.getProperty("pos");
		IgProperty ENERGY = clusters.getProperty("energy");
		
		IgProperty FRACTION = calohits.getProperty("fraction");
		IgProperty F1 = calohits.getProperty("front_1");
		IgProperty F2 = calohits.getProperty("front_2");
		IgProperty F3 = calohits.getProperty("front_3");
		IgProperty F4 = calohits.getProperty("front_4");
		IgProperty B1 = calohits.getProperty("back_1");
		IgProperty B2 = calohits.getProperty("back_2");
		IgProperty B3 = calohits.getProperty("back_3");
		IgProperty B4 = calohits.getProperty("back_4");
     
		SoMaterial* mat = new SoMaterial;
		float rgbcomponents [4];
		IgSbColorMap::unpack (0xB0E57C00, rgbcomponents);
		mat->diffuseColor.setValue (rgbcomponents);
		sep->addChild (mat);

		int n = 0;
		SoVertexProperty *vertices = new SoVertexProperty;

		IgCollectionIterator cit = clusters.begin ();
		IgCollectionIterator cend = clusters.end ();

		for (; cit != cend ; cit++, n++) 
		{
		    IgCollectionItem m = *cit;

		    IgV3d p1 = m.get<IgV3d> (POS);
		
		    double x = p1.x ();
		    double y = p1.y ();
		    double z = p1.z ();
		    vertices->vertex.set1Value (n, SbVec3f (x, y, z));

		    double energy = m.get<double>(ENERGY);
		    
		    for ( IgAssociationSet::Iterator a = assoc.begin();
			  a != assoc.end(); a++ )
		    {
			if ( a->first().objectId() == m.currentRow() )
			{
			    IgCollectionItem c(&calohits, a->second().objectId());
			    
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
		    

		}

	

		vertices->vertex.setNum (n);

		SoMFInt32 tmarkerIndex;
		tmarkerIndex.setValue (SoMarkerSet::PLUS_5_5);
		
		SoMarkerSet *sopoints = new SoMarkerSet;
		sopoints->markerIndex = tmarkerIndex;
		sopoints->vertexProperty.setValue (vertices);
		sopoints->numPoints.setValue (n);
	  
		sep->addChild (sopoints);
	    }
	}
    }
}
