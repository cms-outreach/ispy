#define QT_NO_EMIT

#include "Iguana/QtGUI/interface/ISpyRPCRecHitTwig.h"
#include "Iguana/QtGUI/interface/ISpyReadService.h"

#include "Iguana/Inventor/interface/IgSbColorMap.h"
#include "Iguana/Inventor/interface/IgSo3DErrorBar.h"

#include "Iguana/Framework/interface/IgCollection.h"

#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoSeparator.h>

#include <QSettings>

ISpyRPCRecHitTwig::ISpyRPCRecHitTwig (IgState *state, IgTwig *parent, 
				      const std::string &name)
    : ISpyQueuedTwig (state, parent, name)
{}

void
ISpyRPCRecHitTwig::onNewEvent (ISpyEventMessage& message) 
{
    ISpyQueuedTwig::onNewEvent (message);

    QSettings settings;    
    QString visSettings ("igtwigs/visibility/");
    visSettings.append ("RPCRecHits_V1");

    if (settings.contains (visSettings) && 
	Qt::CheckState (settings.value (visSettings).value<int> ()) == Qt::Unchecked)
	return;

    if (ISpyReadService *readService = ISpyReadService::get (state ()))
    {	
	IgDataStorage *storage = readService->dataStorage ();
	if (storage->hasCollection ("RPCRecHits_V1"))
	{	    
	    IgCollection &rechits = storage->getCollection ("RPCRecHits_V1");
	
	    if (rechits.size () > 0 && 
		rechits.hasProperty("u1") &&
		rechits.hasProperty("u2") &&
		rechits.hasProperty("v1") &&
		rechits.hasProperty("v2") &&
		rechits.hasProperty("w1") &&
		rechits.hasProperty("w2"))
	    {

		SoSeparator *sep = dynamic_cast<SoSeparator *>(ISpyQueuedTwig::rep ());
		sep->setName (SbName ("RPCRecHits_V1"));

		SoMaterial *mat = new SoMaterial;
		float rgbcomponents [4];
		//0xffff0000
		//IgSbColorMap::unpack (0xB0E57C00, rgbcomponents);
		IgSbColorMap::unpack(0xffff0000, rgbcomponents);
		mat->diffuseColor.setValue (SbColor (rgbcomponents));
		sep->addChild (mat);

		IgProperty U1 = rechits.getProperty("u1");
		IgProperty U2 = rechits.getProperty("u2");
		IgProperty V1 = rechits.getProperty("v1");
		IgProperty V2 = rechits.getProperty("v2");
		IgProperty W1 = rechits.getProperty("w1");
		IgProperty W2 = rechits.getProperty("w2");

		IgCollectionIterator cit = rechits.begin ();
		IgCollectionIterator cend = rechits.end ();

		for (; cit != cend ; cit++) 
		{
		    IgCollectionItem m = *cit;

		    IgV3d u1 = m.get<IgV3d>(U1);
		    IgV3d u2 = m.get<IgV3d>(U2);
		    IgV3d v1 = m.get<IgV3d>(V1);
		    IgV3d v2 = m.get<IgV3d>(V2);
		    IgV3d w1 = m.get<IgV3d>(W1);
		    IgV3d w2 = m.get<IgV3d>(W2);

		    IgSo3DErrorBar* errorBar = new IgSo3DErrorBar;
		    
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
	}
    }
}
