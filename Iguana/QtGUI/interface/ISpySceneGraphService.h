#ifndef QT_GUI_ISPY_SCENE_GRAPH_SERVICE_H
# define QT_GUI_ISPY_SCENE_GRAPH_SERVICE_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Framework/interface/config.h"
# include "Iguana/Framework/interface/IgStateElement.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class SoNode;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class ISpySceneGraphService : public IgStateElement
{
    IG_DECLARE_STATE_ELEMENT (ISpySceneGraphService);
public:
    ISpySceneGraphService (IgState *state, SoNode *sceneGraph, SoNode *overlaySceneGraph);
    ~ISpySceneGraphService (void);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    virtual SoNode *		sceneGraph (void);
    virtual SoNode *		overlaySceneGraph (void);

private:
    IgState			*m_state;
    SoNode			*m_sceneGraph;
    SoNode			*m_overlaySceneGraph;

    // undefined semantics
    ISpySceneGraphService (const ISpySceneGraphService &);
    ISpySceneGraphService &operator= (const ISpySceneGraphService &);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // QT_GUI_ISPY_SCENE_GRAPH_SERVICE_H
