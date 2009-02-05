#ifndef IG_TEXT_MODEL_IG_TEXT_MODEL_H
# define IG_TEXT_MODEL_IG_TEXT_MODEL_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgTextModel/interface/config.h"
# include "Ig_Framework/IgObjectBrowser/interface/IgModel.h"
# include <classlib/utils/Callback.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgTextRep;
class IgTextModelEvent;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_TEXT_MODEL_API IgTextModel : public IgModel
{
public:
    typedef lat::Callback1<IgTextModelEvent> Listener;
    enum EventType { TextChanged };

    ~IgTextModel (void);
    // default constructor
    // default copy constructor
    // default assignment operator

    virtual void        listen (EventType event, const Listener &listener) = 0;
    virtual void        unlisten (EventType event, const Listener &listener) = 0;
    virtual void        changed (void) = 0;

    virtual IgTextRep *	text (void) const = 0;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_TEXT_MODEL_IG_TEXT_MODEL_H
