#define QT_NO_EMIT

#include "Iguana/QtGUI/interface/ISpyDTRecHitTwig.h"
#include "Iguana/QtGUI/interface/ISpyReadService.h"
#include "Iguana/Inventor/interface/IgSbColorMap.h"
#include "Iguana/Framework/interface/IgCollection.h"
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoMarkerSet.h>
#include <Inventor/nodes/SoPointSet.h>
#include <Inventor/nodes/SoVertexProperty.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoLineSet.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoTransform.h>
#include <QSettings>

ISpyDTRecHitTwig::ISpyDTRecHitTwig (IgState *state, IgTwig *parent, 
				      const std::string &name)
    : ISpyQueuedTwig (state, parent, name)
{}

void
ISpyDTRecHitTwig::onNewEvent (ISpyEventMessage& message) 
{    
    ISpyQueuedTwig::onNewEvent (message);

    QSettings settings;    
    QString visSettings ("igtwigs/visibility/");
    visSettings.append ("DTRecHits_V1");

    if (settings.contains (visSettings) && 
	Qt::CheckState (settings.value (visSettings).value<int> ()) == Qt::Unchecked)
	return;

    if (ISpyReadService *readService = ISpyReadService::get (state ()))
    {	
	IgDataStorage *storage = readService->dataStorage ();
	if (storage->hasCollection ("DTRecHits_V1"))
	{	    
	    IgCollection &recHits = storage->getCollection ("DTRecHits_V1");
	   
	    if ( recHits.size () > 0 && 
		 recHits.hasProperty("lPlusGlobalPos") &&
		 recHits.hasProperty("lMinusGlobalPos") &&
		 recHits.hasProperty("rPlusGlobalPos") &&
		 recHits.hasProperty("rMinusGlobalPos") &&	
		 recHits.hasProperty("lGlobalPos") &&
		 recHits.hasProperty("rGlobalPos") &&
		 recHits.hasProperty("wirePos") &&
		 recHits.hasProperty("axis") && 
		 recHits.hasProperty("angle") &&	 
		 recHits.hasProperty("cellWidth") &&
		 recHits.hasProperty("cellLength") &&
		 recHits.hasProperty("cellHeight") )
	    {
		SoSeparator *sep = dynamic_cast<SoSeparator *>(ISpyQueuedTwig::rep ());
		sep->setName (SbName ("DTRecHits_V1"));

		//SoMaterial *mat = new SoMaterial;
		//float rgbcomponents [4];
		//IgSbColorMap::unpack (0x0000FFFF, rgbcomponents);
		//mat->diffuseColor.setValue (SbColor (rgbcomponents));
		//sep->addChild(mat);
		
		SoVertexProperty* vtx = new SoVertexProperty;
		vtx->materialBinding = SoVertexProperty::OVERALL;
		vtx->orderedRGBA = 0x0000FFFF;

		SoDrawStyle* wdrawStyle = new SoDrawStyle;
		wdrawStyle->style = SoDrawStyle::LINES;
		wdrawStyle->lineWidth.setValue(1.0);
		sep->addChild(wdrawStyle);
		
		IgProperty LPLUS_GLOBALPOS = recHits.getProperty("lPlusGlobalPos");
		IgProperty LMINUS_GLOBALPOS = recHits.getProperty("lMinusGlobalPos");
		IgProperty RPLUS_GLOBALPOS = recHits.getProperty("rPlusGlobalPos");
		IgProperty RMINUS_GLOBALPOS = recHits.getProperty("rMinusGlobalPos");
		IgProperty LGLOBALPOS = recHits.getProperty("lGlobalPos");
		IgProperty RGLOBALPOS = recHits.getProperty("rGlobalPos");

		IgProperty WPOS = recHits.getProperty("wirePos");
		IgProperty AXIS = recHits.getProperty("axis");
		IgProperty ANGLE = recHits.getProperty("angle");
		IgProperty CELL_L = recHits.getProperty("cellLength");
		IgProperty CELL_W = recHits.getProperty("cellWidth");
		IgProperty CELL_H = recHits.getProperty("cellHeight");

		int nVtx = 0;
		
		
		IgCollectionIterator cit = recHits.begin();
		IgCollectionIterator cend = recHits.end();

		for (; cit != cend ; cit++ ) 
		{
		    IgCollectionItem m = *cit;

		    IgV3d lPlusGlobalPos = m.get<IgV3d>("lPlusGlobalPos");
		    IgV3d lMinusGlobalPos = m.get<IgV3d>("lMinusGlobalPos");
		    IgV3d rPlusGlobalPos = m.get<IgV3d>("rPlusGlobalPos");
		    IgV3d rMinusGlobalPos = m.get<IgV3d>("rMinusGlobalPos");
               
		    IgV3d lGlobalPos = m.get<IgV3d>("lGlobalPos");
		    IgV3d rGlobalPos = m.get<IgV3d>("rGlobalPos");

		    
		    SoLineSet* linel = new SoLineSet;
		    linel->numVertices = 2;
		    SoVertexProperty* vtxl = new SoVertexProperty;


		    vtxl->vertex.set1Value(0, SbVec3f(lPlusGlobalPos.x(), 
						      lPlusGlobalPos.y(), 
						      lPlusGlobalPos.z()));

		    vtxl->vertex.set1Value(1, SbVec3f(lMinusGlobalPos.x(), 
						      lMinusGlobalPos.y(), 
						      lMinusGlobalPos.z()));

		    linel->vertexProperty = vtxl;
		    
		    
		    SoLineSet* liner = new SoLineSet;
		    liner->numVertices = 2;
		    SoVertexProperty* vtxr = new SoVertexProperty;
		   

		    vtxr->vertex.set1Value(0, SbVec3f(rPlusGlobalPos.x(), 
						      rPlusGlobalPos.y(), 
						      rPlusGlobalPos.z()));

		    vtxr->vertex.set1Value(1, SbVec3f(rMinusGlobalPos.x(), 
						      rMinusGlobalPos.y(), 
						      rMinusGlobalPos.z()));

		    liner->vertexProperty = vtxr;

		    sep->addChild(linel);
		    sep->addChild(liner);
		    
		    vtx->vertex.set1Value(nVtx, SbVec3f(lGlobalPos.x(), 
							lGlobalPos.y(), 
							lGlobalPos.z()));
		    nVtx++;

		    vtx->vertex.set1Value(nVtx, SbVec3f(rGlobalPos.x(), 
							rGlobalPos.y(), 
							rGlobalPos.z()));
		    nVtx++;

		    
		    IgV3d pos = m.get<IgV3d>(WPOS);
		
		    double x = pos.x();
		    double y = pos.y();
		    double z = pos.z();

		    IgV3d axis = m.get<IgV3d>(AXIS);
		    double angle = m.get<double>(ANGLE);
			
		    SoTransform* transform = new SoTransform;
		    transform->translation.setValue(x,y,z);
		    transform->rotation.setValue(SbVec3f(axis.x(),axis.y(),axis.z()), angle);
		        
		    SoCube* cube = new SoCube;
		    cube->width = m.get<double>(CELL_W);
		    cube->height = m.get<double>(CELL_L);
		    cube->depth = m.get<double>(CELL_H);
		    
		    SoSeparator* separator = new SoSeparator;
		    separator->addChild(transform);
		    separator->addChild(cube);
		    sep->addChild(separator);
		}  

		
		SoMFInt32 tmarkerIndex;
		tmarkerIndex.setValue (SoMarkerSet::PLUS_7_7);
		
		SoMarkerSet *sopoints = new SoMarkerSet;
		sopoints->markerIndex = tmarkerIndex;
		sopoints->vertexProperty.setValue(vtx);
		sopoints->numPoints.setValue(nVtx);
	  
		sep->addChild (sopoints);
		
	
	    }
	}
    }
}
