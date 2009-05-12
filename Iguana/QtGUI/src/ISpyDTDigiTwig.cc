#define QT_NO_EMIT

#include "Iguana/QtGUI/interface/ISpyDTDigiTwig.h"
#include "Iguana/QtGUI/interface/ISpyReadService.h"
#include "Iguana/Inventor/interface/IgSbColorMap.h"
#include "Iguana/Framework/interface/IgCollection.h"
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoMarkerSet.h>
#include <Inventor/nodes/SoPointSet.h>
#include <Inventor/nodes/SoVertexProperty.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoTransform.h>
#include <QSettings>

ISpyDTDigiTwig::ISpyDTDigiTwig (IgState *state, IgTwig *parent, 
				      const std::string &name)
    : ISpyQueuedTwig (state, parent, name)
{}

void
ISpyDTDigiTwig::onNewEvent (ISpyEventMessage& message) 
{    
    ISpyQueuedTwig::onNewEvent (message);

    QSettings settings;    
    QString visSettings ("igtwigs/visibility/");
    visSettings.append ("DTDigis_V1");

    if (settings.contains (visSettings) && 
	Qt::CheckState (settings.value (visSettings).value<int> ()) == Qt::Unchecked)
	return;

    if (ISpyReadService *readService = ISpyReadService::get (state ()))
    {	
	IgDataStorage *storage = readService->dataStorage ();

	if (storage->hasCollection ("DTDigis_V1"))
	{	    
	    IgCollection &digis = storage->getCollection ("DTDigis_V1");

	    if ( digis.size () > 0 && 
		 digis.hasProperty("pos") &&
		 digis.hasProperty("axis") && 
		 digis.hasProperty("angle") &&
		 digis.hasProperty("cellWidth") &&
		 digis.hasProperty("cellLength") &&
		 digis.hasProperty("cellHeight") )
	    {
		SoSeparator *sep = dynamic_cast<SoSeparator *>(ISpyQueuedTwig::rep ());
		sep->setName(SbName("DTDigis_V1"));

		SoMaterial *mat = new SoMaterial;
		float rgbcomponents [4];
		IgSbColorMap::unpack (0x66ff0000, rgbcomponents);
		mat->diffuseColor.setValue (SbColor (rgbcomponents));
		sep->addChild (mat);

		IgProperty POS = digis.getProperty("pos");
		IgProperty AXIS = digis.getProperty("axis");
		IgProperty ANGLE = digis.getProperty("angle");
		IgProperty CELL_L = digis.getProperty("cellLength");
		IgProperty CELL_W = digis.getProperty("cellWidth");
		IgProperty CELL_H = digis.getProperty("cellHeight");
		
		int n = 0;
		SoVertexProperty *vertices = new SoVertexProperty;

		IgCollectionIterator cit = digis.begin ();
		IgCollectionIterator cend = digis.end ();
		for (; cit != cend ; cit++, n++) 
		{
		    IgCollectionItem m = *cit;

		    IgV3d pos = m.get<IgV3d>(POS);
		
		    double x = pos.x();
		    double y = pos.y();
		    double z = pos.z();
		    vertices->vertex.set1Value(n, SbVec3f (x, y, z));

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

		vertices->vertex.setNum(n);

		SoMFInt32 tmarkerIndex;
		tmarkerIndex.setValue(SoMarkerSet::SQUARE_LINE_7_7);
		
		SoMarkerSet *sopoints = new SoMarkerSet;
		sopoints->markerIndex = tmarkerIndex;
		sopoints->vertexProperty.setValue (vertices);
		sopoints->numPoints.setValue (n);
	  
		sep->addChild (sopoints);
	    }
	}
    }
}
