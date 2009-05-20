#define QT_NO_EMIT

//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/QtGUI/interface/ISpySiStripDigiTwig.h"
#include "Iguana/QtGUI/interface/ISpyReadService.h"
#include "Iguana/Inventor/interface/IgSbColorMap.h"
#include "Iguana/Framework/interface/IgCollection.h"
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoMarkerSet.h>
#include <Inventor/nodes/SoPointSet.h>
#include <Inventor/nodes/SoVertexProperty.h>
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

ISpySiStripDigiTwig::ISpySiStripDigiTwig (IgState *state, IgTwig *parent,
					  const std::string &name /* = "" */)
    : ISpyQueuedTwig (state, parent, name)
{}

void
ISpySiStripDigiTwig::onNewEvent (ISpyEventMessage& message)
{
    ISpyQueuedTwig::onNewEvent (message);

    QSettings settings;    
    QString visSettings ("igtwigs/visibility/");
    visSettings.append ("SiStripDigis_V1");

    if (settings.contains (visSettings) && 
	Qt::CheckState (settings.value (visSettings).value<int> ()) == Qt::Unchecked)
	return;

    if (ISpyReadService *readService = ISpyReadService::get (state ()))
    {	
	IgDataStorage *storage = readService->dataStorage ();
	if (storage->hasCollection ("SiStripDigis_V1"))
	{	    
	    IgCollection &digis = storage->getCollection ("SiStripDigis_V1");
	    if (digis.size () > 0 && digis.hasProperty ("pos"))
	    {
		IgProperty POS = digis.getProperty ("pos");

		int n = 0;
		SoVertexProperty *vertices = new SoVertexProperty;

		IgCollectionIterator cit = digis.begin ();
		IgCollectionIterator cend = digis.end ();
		for (; cit != cend ; cit++, n++) 
		{
		    IgCollectionItem m = *cit;

		    IgV3d p1 = m.get<IgV3d> (POS);
		
		    double x = p1.x ();
		    double y = p1.y ();
		    double z = p1.z ();
		    vertices->vertex.set1Value (n, SbVec3f (x, y, z));
		}
		vertices->vertex.setNum (n);

		SoSeparator *sep = dynamic_cast<SoSeparator *>(ISpyQueuedTwig::rep ());
		sep->setName (SbName ("SiStripDigis_V1"));

		SoMaterial *mat = new SoMaterial;
		float rgbcomponents [4];
		//IgSbColorMap::unpack (0x6600FF00, rgbcomponents);
		IgSbColorMap::unpack (0x551A8B, rgbcomponents);
		mat->diffuseColor.setValue (SbColor (rgbcomponents));
		sep->addChild (mat);

		SoMFInt32 tmarkerIndex;
		tmarkerIndex.setValue (SoMarkerSet::CIRCLE_LINE_5_5);
		
		SoMarkerSet *sopoints = new SoMarkerSet;
		sopoints->markerIndex = tmarkerIndex;
		sopoints->vertexProperty.setValue (vertices);
		sopoints->numPoints.setValue (n);
	  
		sep->addChild (sopoints);
	    }
	}
    }
}


