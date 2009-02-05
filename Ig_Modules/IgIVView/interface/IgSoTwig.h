#ifndef IG_IV_VIEW_IG_SO_TWIG_H
# define IG_IV_VIEW_IG_SO_TWIG_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgIVView/interface/config.h"
# include "Ig_Modules/IgCommonData/interface/IgTwig.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class SoGroup;
class SoNode;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_IV_VIEW_API IgSoTwig : public IgSimpleTwig
{
public:
    IgSoTwig (IgTwig *parent,
	      const std::string &name,
	      bool selectable = false,
	      bool traverse = true,
	      bool visibility = true,
	      bool cascade = true);
    explicit
    IgSoTwig (const std::string &name,
	      bool selectable = false,
	      bool traverse = true,
	      bool visibility = true,
	      bool cascade = true);
    ~IgSoTwig (void);

    // twig tree protocol
    virtual void	destroy (void);

    // inventor interface
    virtual SoGroup *	node (void) const;
    virtual SoGroup *	node (SoGroup *node);
    virtual SoGroup *	magic (void) const;
    virtual SoGroup *	magic (SoGroup *node);

    static IgSoTwig *	buildFrom (SoNode *from);

    // static SoPath *	onPick (void *, SoPickedPoint *pick);
    // static void	onSelect (void *, SoPath *selectionPath);

protected:
    // virtual void	preDraw (void);
    // virtual void	draw (void);
    // virtual bool	okToRedraw (void) const;

    // virtual const SoPickedPoint *	currentPick (void) const;
    // virtual void			currentPick (const SoPickedPoint *);

    // virtual IgSoTwig *findMatch (const SoNode *node);

    virtual void	cleared (IgTwig *parent);
    static IgSoTwig *	buildFromReps (SoNode *from);
    static IgSoTwig *	buildFromVis (SoNode *from, bool top);
    static IgSoTwig *	buildFromOther (SoNode *from);

private:
    SoGroup		*m_magic;
    SoGroup		*m_node;
    bool		m_selectable;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_IV_VIEW_IG_SO_TWIG_H
