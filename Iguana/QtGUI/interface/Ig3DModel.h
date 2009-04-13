#ifndef VIEW_IG_3D_MODEL_H
# define VIEW_IG_3D_MODEL_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Framework/interface/config.h"
# include "Iguana/Framework/interface/IgModel.h"
# include <classlib/utils/Callback.h>
# include <vector>
# include <string>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgState;
class Ig3DModelEvent;
class Ig3DRep;
class QGraphicsItem;
class QGraphicsItemGroup;
class QGraphicsScene;
class QString;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IGUANA_FRAMEWORK_API Ig3DModel : public IgModel
{
public:
    typedef lat::Callback1<Ig3DModelEvent> Listener;
    enum EventType { ModelChanged };

    Ig3DModel (IgState *state);
    ~Ig3DModel (void);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor


    virtual void	listen (EventType event, const Listener &listener);
    virtual void	unlisten (EventType event, const Listener &listener);
    virtual void	changed (void);

    virtual void	add (Ig3DRep *rep);
    virtual void	remove (Ig3DRep *rep, bool search = false);
    virtual void	change (Ig3DRep *rep);
    virtual Ig3DRep 	*lookup (QGraphicsItemGroup *node) const;

    virtual QGraphicsScene *scene (void) const;
    virtual Ig3DRep 	*attachPoint (void) const;

    virtual IgState *	state (void) const;

    static QString	encode (const std::string &name);
    static std::string	decode (const std::string &name);
    static std::string	decode (const QString &name);

private:
    typedef std::vector<Listener> Listeners;

    IgState		*m_state;
    Listeners		m_listeners;
    QGraphicsScene	*m_scene;
    Ig3DRep		*m_attachPoint;

    // undefined semantics
    Ig3DModel (const Ig3DModel &);
    Ig3DModel &operator= (const Ig3DModel &);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIEW_IG_3D_MODEL_H
