#define QT_NO_EMIT

//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/QtGUI/interface/ISpyMETTwig.h"
#include "Iguana/QtGUI/interface/ISpyReadService.h"
#include "Iguana/Inventor/interface/IgSbColorMap.h"
#include "Iguana/Framework/interface/IgCollection.h"
#include <Inventor/nodes/SoIndexedLineSet.h>
#include <Inventor/nodes/SoMaterial.h>
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

ISpyMETTwig::ISpyMETTwig (IgState *state, IgTwig *parent, 
			  const std::string &name)
    : ISpyQueuedTwig (state, parent, name)
{}

void
ISpyMETTwig::onNewEvent (ISpyEventMessage& message) 
{
    ISpyQueuedTwig::onNewEvent (message);

    QSettings settings;    
    QString visSettings ("igtwigs/visibility/");
    visSettings.append ("METs_V1");

    if (settings.contains (visSettings) && 
	Qt::CheckState (settings.value (visSettings).value<int> ()) == Qt::Unchecked)
	return;

    if (ISpyReadService *readService = ISpyReadService::get (state ()))
    {	
	SoSeparator *sep = dynamic_cast<SoSeparator *>(ISpyQueuedTwig::rep ());
	sep->setName (SbName ("ISpyMETTwig"));

	SoMaterial *mat = new SoMaterial;
	float rgbcomponents [4];
	IgSbColorMap::unpack (0xFF5B0000, rgbcomponents);
	mat->diffuseColor.setValue (SbColor (rgbcomponents));
	sep->addChild (mat);

	SoDrawStyle *sty = new SoDrawStyle;
	sty->style = SoDrawStyle::LINES;
	sty->lineWidth.setValue (3);
	sep->addChild (sty);

	IgDataStorage *storage = readService->dataStorage ();
	if (storage->hasCollection ("METs_V1"))
	{	    
	    IgCollection &mets = storage->getCollection ("METs_V1");
	    if (mets.size () > 0 && mets.hasProperty ("pt") &&
		mets.hasProperty ("px") && 
		mets.hasProperty ("py") &&
		mets.hasProperty ("phi"))
	    {
		SoVertexProperty *vertices = new SoVertexProperty;
		int i = 0;
		std::vector<int> lineIndices;
		std::vector<SbVec3f> points;		

		IgCollectionIterator cit = mets.begin ();
		IgCollectionIterator cend = mets.end ();
		for (; cit != cend ; cit++) 
		{
		    IgCollectionItem item = *cit;
		    points.push_back (SbVec3f (0., 0., 0.));
		    points.push_back (SbVec3f (item.get<double>("px"), item.get<double>("py"), 0.));

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
    }
}

