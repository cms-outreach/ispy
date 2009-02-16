#ifndef VIEW_IVIEW_SCENE_GRAPH_SERVICE_H
# define VIEW_IVIEW_SCENE_GRAPH_SERVICE_H

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

class IViewSceneGraphService : public IgStateElement
{
    IG_DECLARE_STATE_ELEMENT (IViewSceneGraphService);
public:
    IViewSceneGraphService (IgState *state, SoNode *sceneGraph, SoNode *overlaySceneGraph);
    ~IViewSceneGraphService (void);
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
    IViewSceneGraphService (const IViewSceneGraphService &);
    IViewSceneGraphService &operator= (const IViewSceneGraphService &);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIEW_IVIEW_SCENE_GRAPH_SERVICE_H
