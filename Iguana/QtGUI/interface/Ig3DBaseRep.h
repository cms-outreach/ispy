#ifndef QT_GUI_IG3DBASE_REP_H
# define QT_GUI_IG3DBASE_REP_H

//<<<<<< INCLUDES                                                       >>>>>>

# include <Inventor/nodes/SoSeparator.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class Ig3DBaseModel;
class SoGroup;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class Ig3DBaseRep : public SoSeparator
{
public:
    Ig3DBaseRep (Ig3DBaseModel *model, Ig3DBaseRep *parent, Ig3DBaseRep *prev, SoGroup *node);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    ~Ig3DBaseRep (void);

    virtual Ig3DBaseModel *     model (void) const;
    
    virtual SoGroup *           magic (void) const;
    virtual SoGroup *           magic (SoGroup *node);
    virtual SoNode *            findMagic (SbName name) const;
    
    virtual SoGroup *           node (void) const;
    virtual SoGroup *           node (SoGroup *node);
    
    virtual int                 children (void) const;
    virtual Ig3DBaseRep *       child (int n) const;
    virtual Ig3DBaseRep *       child (SbString name) const;
    
    virtual void                clear (void);
    
    // FIXME: virtual void      pick (path, detail);
    // FIXME: remember last pick point/path detail?
    
    static Ig3DBaseRep *        asRep (SoNode *node);
    
protected:

    virtual void                zap (bool search = false, bool kill = true);
    
private:
    Ig3DBaseModel               *m_model;
    
    // undefined semantics
    Ig3DBaseRep (const Ig3DBaseRep &);
    Ig3DBaseRep &operator= (const Ig3DBaseRep &);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // QT_GUI_IG3DBASE_REP_H
