//<<<<<< INCLUDES                                                       >>>>>>

#include "Iguana/QtGUI/interface/ISpyQueuedTwig.h"
#include "Iguana/QtGUI/interface/ISpySceneGraphService.h"
#include <Inventor/nodes/SoSeparator.h>
#include <QDebug>

ISpyQueuedTwig::ISpyQueuedTwig (IgState *state, IgTwig *parent,
				const std::string &name /* = "" */)
    : IgSimpleTwig (parent, name, true, false, true),
      ISpyObserver (state),
      m_rep (new SoSeparator),
      m_state (state)
{
    ASSERT (m_state); 
    m_rep->ref ();
    
    ISpySceneGraphService *sceneGraphService = ISpySceneGraphService::get (state);
    ASSERT (sceneGraphService);
    SoSeparator *mainScene = dynamic_cast<SoSeparator *>(sceneGraphService->sceneGraph ());
    ASSERT (mainScene);	
    mainScene->addChild (m_rep);	
}

ISpyQueuedTwig::~ISpyQueuedTwig (void)
{
    ASSERT (m_rep);
    m_rep->unref ();
}

void
ISpyQueuedTwig::clear (void)
{
    ASSERT (m_rep);
    dynamic_cast<SoSeparator *>(m_rep)->removeAllChildren ();
    m_rep->unref ();
    m_rep = new SoSeparator;
    m_rep->ref ();

    ISpySceneGraphService *sceneGraphService = ISpySceneGraphService::get (state ());
    ASSERT (sceneGraphService);
    SoSeparator *mainScene = dynamic_cast<SoSeparator *>(sceneGraphService->sceneGraph ());
    ASSERT (mainScene);	
    mainScene->addChild (m_rep);
}

SoNode*
ISpyQueuedTwig::rep (void)
{
    ASSERT (m_rep);
    return m_rep;
}

void
ISpyQueuedTwig::onNewEvent (ISpyEventMessage& message) 
{
    qDebug () << "ISpyQueuedTwig[" << QString::fromStdString (name ())
	      << "]::onNewEvent(" << QString::fromStdString (message.message) << ")\n";
    
    clear ();
}
