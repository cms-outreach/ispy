#ifndef IG_TABLE_MODEL_IG_TABLE_MODEL_H
# define IG_TABLE_MODEL_IG_TABLE_MODEL_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgTableModel/interface/config.h"
# include "Ig_Framework/IgObjectBrowser/interface/IgModel.h"
# include <classlib/utils/Callback.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgTableModelEvent;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_TABLE_MODEL_API IgTableModel : public IgModel
{
public:
    typedef lat::Callback1<IgTableModelEvent> Listener;
    enum EventType { TableChanged };

    ~IgTableModel (void);
    // default constructor
    // default copy constructor
    // default assignment operator

    virtual void		listen (EventType event, const Listener &listener) = 0;
    virtual void		unlisten (EventType event, const Listener &listener) = 0;
    virtual void		changed (unsigned row, unsigned column) = 0;
    // FIXME: deal with all possibilities of IgTableModelEvent!

    // FIXME: cell access methods
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_TABLE_MODEL_IG_TABLE_MODEL_H
