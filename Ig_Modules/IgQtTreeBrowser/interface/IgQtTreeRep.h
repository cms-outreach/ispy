#ifndef IG_QT_TREE_BROWSER_IG_QT_TREE_REP_H
# define IG_QT_TREE_BROWSER_IG_QT_TREE_REP_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgQtTreeBrowser/interface/config.h"
# include "Ig_Framework/IgObjectBrowser/interface/IgRep.h"

# include <qlistview.h>
# include <qstring.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
class IgQtTreeBrowser;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_QT_TREE_BROWSER_API IgQtTreeRep : public QListViewItem, public IgRep
{
public:
    static const int		VISIBLE_FIELD  = 1;
    static const int		TRAVERSE_FIELD = 2;
    static const int		CASCADE_FIELD  = 3;

    static IgQtTreeRep *	create (IgQtTreeBrowser *in,
					IgQtTreeRep *parent,
					IgQtTreeRep *before,
					QString name, bool traverse,
					bool visible, bool opened);

    IgQtTreeRep (IgQtTreeRep *parent, IgQtTreeRep *before,
		 QString name, bool traverse, bool visible, bool opened);
    IgQtTreeRep (IgQtTreeRep *parent,
		 QString name, bool traverse, bool visible, bool opened);
    IgQtTreeRep (IgQtTreeBrowser *in, IgQtTreeRep *before,
		 QString name, bool traverse, bool visible, bool opened);
    IgQtTreeRep (IgQtTreeBrowser *in,
		 QString name, bool traverse, bool visible, bool opened);
    ~IgQtTreeRep (void);

    virtual IgRepContext *	context (void) const;
    virtual IgModel *		model (void) const;

    bool			bold (void) const;
    void			bold (bool value);

    bool			italic (void) const;
    void			italic (bool value);

    bool			magic (void) const;
    void			magic (bool value);

    virtual void		setOpen (bool opened);
    void                        setCheck (int column, bool value, bool known);

protected:
    virtual void		context (IgRepContext *context);

    void			init (bool traverse, bool visible, bool opened);
    bool			traversed (void) const;
    void			repaintChildren (QListViewItem *item);
    QColorGroup			columnColor (int column, const QColorGroup &cg) const;

    virtual void		paintCell (QPainter *painter,
					   const QColorGroup &cg,
					   int column, int width, int align);
    virtual int			width (const QFontMetrics &,
				       const QListView *lv,
				       int c) const;

private:
    bool			flag (unsigned n) const;
    void			flag (unsigned n, bool value);

    static const unsigned int	CHECKABLE = 1;
    static const unsigned int	BOLD	  = 2;
    static const unsigned int	ITALIC	  = 4;
    static const unsigned int	MAGIC	  = 8;
    static const unsigned int	EXPANDED  = 16;
    static const unsigned int	TRAVERSE  = 32;
    static const unsigned int	VISIBLE	  = 64;
    static const unsigned int	AUTO_OPEN = 128;

    IgRepContext		*m_context;
    unsigned			m_flags;

    // undefined semantics
    IgQtTreeRep (const IgQtTreeRep &);
    IgQtTreeRep &operator= (const IgQtTreeRep &);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

inline bool
IgQtTreeRep::flag (unsigned n) const
{ return m_flags & n; }

inline void
IgQtTreeRep::flag (unsigned n, bool value)
{ m_flags = (m_flags & ~n) | (n * value); }

inline bool
IgQtTreeRep::bold (void) const
{ return flag (BOLD); }

inline void
IgQtTreeRep::bold (bool value)
{ flag (BOLD, value); }

inline bool
IgQtTreeRep::italic (void) const
{ return flag (ITALIC); }

inline void
IgQtTreeRep::italic (bool value)
{ flag (ITALIC, value); }

inline bool
IgQtTreeRep::magic (void) const
{ return flag (MAGIC); }

inline void
IgQtTreeRep::magic (bool value)
{ flag (MAGIC, value); }

#endif // IG_QT_TREE_BROWSER_IG_QT_TREE_REP_H
