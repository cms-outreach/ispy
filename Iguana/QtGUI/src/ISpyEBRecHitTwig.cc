#include <QSettings>
#include "Iguana/QtGUI/interface/ISpyEBRecHitTwig.h"
#include "Iguana/QtGUI/interface/ISpyReadService.h"
#include "Iguana/Inventor/interface/IgSbColorMap.h"
#include "Iguana/Framework/interface/IgCollection.h"
#include "Iguana/Inventor/interface/IgSoCrystalHit.h"
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoSeparator.h>

ISpyEBRecHitTwig::ISpyEBRecHitTwig (IgState *state, IgTwig *parent, 
					const std::string &name)
    : ISpyQueuedTwig (state, parent, name)
{}

void
ISpyEBRecHitTwig::onNewEvent (ISpyEventMessage& message)
{
    ISpyQueuedTwig::onNewEvent (message);

    QSettings settings;    
    QString visSettings ("igtwigs/visibility/");
    visSettings.append ("EBRecHits_V1");

    if (settings.contains (visSettings) && 
	Qt::CheckState (settings.value (visSettings).value<int> ()) == Qt::Unchecked)
	return;
    
    if (ISpyReadService *readService = ISpyReadService::get (state ()))
    {    	
	IgDataStorage* storage = readService->dataStorage();
	
	if ( storage->hasCollection("EBRecHits_V1") )
	{
	    IgCollection& rechits = storage->getCollection("EBRecHits_V1");
	    
	    if ( rechits.size() > 0 &&
		 rechits.hasProperty("energy") &&
		 rechits.hasProperty("front_1") &&
		 rechits.hasProperty("front_2") &&
		 rechits.hasProperty("front_3") &&
		 rechits.hasProperty("front_4") &&
		 rechits.hasProperty("back_1") &&
		 rechits.hasProperty("back_2") &&
		 rechits.hasProperty("back_3") &&
		 rechits.hasProperty("back_4") )
	    {
		SoSeparator *sep = dynamic_cast<SoSeparator *>(ISpyQueuedTwig::rep ());	
		sep->setName (SbName ("ISpyEBRecHitTwig"));
	
		SoMaterial *mat = new SoMaterial;
		mat->diffuseColor.setValue(1.0, 0.0, 153.0 / 255.0);
		sep->addChild(mat);

		IgProperty ENERGY = rechits.getProperty("energy");
		IgProperty F1 = rechits.getProperty("front_1");
		IgProperty F2 = rechits.getProperty("front_2");
		IgProperty F3 = rechits.getProperty("front_3");
		IgProperty F4 = rechits.getProperty("front_4");
		IgProperty B1 = rechits.getProperty("back_1");
		IgProperty B2 = rechits.getProperty("back_2");
		IgProperty B3 = rechits.getProperty("back_3");
		IgProperty B4 = rechits.getProperty("back_4");

		for ( IgCollectionIterator rit = rechits.begin();
		      rit != rechits.end(); rit++ )
		{
		    IgCollectionItem r = *rit;
		    
		    double energy = r.get<double>(ENERGY);
		    
		    IgV3d f1 = r.get<IgV3d>(F1);
		    IgV3d f2 = r.get<IgV3d>(F2);
		    IgV3d f3 = r.get<IgV3d>(F3);
		    IgV3d f4 = r.get<IgV3d>(F4);
		   
		    IgV3d b1 = r.get<IgV3d>(B1);
		    IgV3d b2 = r.get<IgV3d>(B2);
		    IgV3d b3 = r.get<IgV3d>(B3);
		    IgV3d b4 = r.get<IgV3d>(B4);

		    IgSoCrystalHit *crystalHit = new IgSoCrystalHit;
		    crystalHit->energy.setValue(energy);                   
		    crystalHit->scale.setValue(1.0);
		    crystalHit->relativeWidth.setValue(1.0);
		    crystalHit->drawCrystal.setValue(false);
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
    }
}
