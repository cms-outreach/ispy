#ifndef QT_GUI_IG3DBASE_MODEL_H
# define QT_GUI_IG3DBASE_MODEL_H

//<<<<<< INCLUDES                                                       >>>>>>

# include <QObject>
# include "Iguana/Framework/interface/IgModel.h"
# include <string>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class SbString;
class SbName;
class SoGroup;
class IgState;
class Ig3DBaseRep;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class Ig3DBaseModel : public QObject, public IgModel
{
    Q_OBJECT
public:
    Ig3DBaseModel (IgState *state);
    ~Ig3DBaseModel (void);
    
    virtual void        add (Ig3DBaseRep *rep);
    virtual void        remove (Ig3DBaseRep *rep, bool search = false);
    virtual void        change (Ig3DBaseRep *rep);
    virtual Ig3DBaseRep *lookup (SoGroup *node) const;

    virtual SoGroup *   sceneGraph (void) const;
    virtual Ig3DBaseRep *attachPoint (void) const;

    virtual IgState *   state (void) const;

    static SbString     encode (const std::string &name);
    static std::string  decode (const std::string &name);
    static std::string  decode (const SbName &name);

signals:
    void        	changed (void);

private:
    void 		initScene (SoGroup *root);
    
    IgState            *m_state;
    SoGroup            *m_sceneGraph;
    Ig3DBaseRep        *m_attachPoint;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // QT_GUI_IG3DBASE_MODEL_H
