#ifndef IG_TREE_MODEL_IG_TREE_MODEL_H
# define IG_TREE_MODEL_IG_TREE_MODEL_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgTreeModel/interface/config.h"
# include "Ig_Framework/IgObjectBrowser/interface/IgModel.h"
# include <classlib/utils/Callback.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgTreeNode;
class IgTreeModelEvent;
class IgTreePath;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_TREE_MODEL_API IgTreeModel : public IgModel
{
public:
    typedef lat::Callback1<IgTreeModelEvent> Listener;
    enum EventType { NodesChanged, NodesInserted, NodesRemoved, StructureChanged };

    IgTreeModel (IgTreeNode *root = 0);
    ~IgTreeModel (void);
    // default constructor
    // default copy constructor
    // default assignment operator

    virtual void                listen (EventType event, const Listener &listener);
    virtual void                unlisten (EventType event, const Listener &listener);
    virtual void                changed (const IgTreePath *path, IgTreeNode *value);

    // FIXME: Should we be able to deal with objects other than IgTreeNode?
    //        It would be nice to allow the tree model allow any underlying data,
    //        but then we need *something* in the argument types, and IgTreeNode
    //        is just as good as anything else...  Or should we make a template
    //        argument?  But then we must choose some standard template parameter
    //        to be able to communicate with model clients!
    virtual IgTreeNode *	root (void) const = 0;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_TREE_MODEL_IG_TREE_MODEL_H
