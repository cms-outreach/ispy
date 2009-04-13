#ifndef VIEW_IG_3D_REP_H
# define VIEW_IG_3D_REP_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Framework/interface/IgRep.h"
# include "Iguana/Framework/interface/config.h"
# include <QGraphicsItemGroup>
# include <vector>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class Ig3DModel;
class Ig3DRep;
class QGraphicsItem;
class QString;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IGUANA_FRAMEWORK_API Ig3DRep : public QGraphicsItemGroup, public IgRep
{
public:
    Ig3DRep (Ig3DModel *model, Ig3DRep *parent, Ig3DRep *prev, QGraphicsItemGroup *node);
    ~Ig3DRep (void);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    virtual IgRepContext *	context (void) const;
    virtual IgModel *		model (void) const;

    virtual QGraphicsItemGroup *magic (void) const;
    virtual QGraphicsItemGroup *magic (QGraphicsItemGroup *node);
    virtual QGraphicsItem *	findMagic (QString name) const;

    virtual QGraphicsItemGroup *node (void) const;
    virtual QGraphicsItemGroup *node (QGraphicsItemGroup *node);

    virtual int			children (void) const;
    virtual Ig3DRep *		child (int n) const;
    virtual Ig3DRep *		child (QString name) const;

    virtual void		clear (void);
    virtual std::string	&	name (void);
    virtual void		name (std::string &name);

    // FIXME: virtual void	pick (path, detail);
    // FIXME: remember last pick point/path detail?

    static Ig3DRep *		asRep (QGraphicsItem *node);

protected:
    virtual void		context (IgRepContext *context);

    virtual void		zap (bool search = false, bool kill = true);

private:
    IgRepContext		*m_context;
    Ig3DModel			*m_model;
    std::string 		m_name;

    // undefined semantics
    Ig3DRep (const Ig3DRep &);
    Ig3DRep &operator= (const Ig3DRep &);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIEW_IG_3D_REP_H
