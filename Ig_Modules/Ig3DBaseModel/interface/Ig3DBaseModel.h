#ifndef IG_3D_MODEL_IG_3D_BASE_MODEL_H
# define IG_3D_MODEL_IG_3D_BASE_MODEL_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/Ig3DBaseModel/interface/config.h"
# include "Ig_Framework/IgObjectBrowser/interface/IgModel.h"
# include <classlib/utils/Callback.h>
# include <vector>
# include <string>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgState;
class Ig3DBaseModelEvent;
class Ig3DBaseRep;
class SoGroup;
class SbName;
class SbString;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_3D_BASE_MODEL_API Ig3DBaseModel : public IgModel
{
public:
    typedef lat::Callback1<Ig3DBaseModelEvent> Listener;
    enum EventType { ModelChanged };

    Ig3DBaseModel (IgState *state);
    ~Ig3DBaseModel (void);

    virtual void	listen (EventType event, const Listener &listener);
    virtual void	unlisten (EventType event, const Listener &listener);
    virtual void	changed (void);

    virtual void	add (Ig3DBaseRep *rep);
    virtual void	remove (Ig3DBaseRep *rep, bool search = false);
    virtual void	change (Ig3DBaseRep *rep);
    virtual Ig3DBaseRep *lookup (SoGroup *node) const;

    virtual SoGroup *	sceneGraph (void) const;
    virtual Ig3DBaseRep *attachPoint (void) const;

    virtual IgState *	state (void) const;

    static SbString	encode (const std::string &name);
    static std::string	decode (const std::string &name);
    static std::string	decode (const SbName &name);

private:
    typedef std::vector<Listener> Listeners;

    IgState		*m_state;
    Listeners		m_listeners;
    SoGroup		*m_sceneGraph;
    Ig3DBaseRep		*m_attachPoint;

    // undefined semantics
    Ig3DBaseModel (const Ig3DBaseModel &);
    Ig3DBaseModel &operator= (const Ig3DBaseModel &);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_3D_MODEL_IG_3D_BASE_MODEL_H
