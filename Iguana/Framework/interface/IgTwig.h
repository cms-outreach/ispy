#ifndef IGUANA_FRAMEWORK_IG_TWIG_H
# define IGUANA_FRAMEWORK_IG_TWIG_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Framework/interface/config.h"
# include "Iguana/Framework/interface/IgRepresentable.h"
# include <classlib/utils/Callback.h>
# include <string>
# include <utility>
# include <vector>
# include <map>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgDatum;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IGUANA_FRAMEWORK_API IgTwig : public IgRepresentable
{
public:
    /** Flag for #IgBrowserMethods::update() @c field indicating that
	the substructure of the twig has changed.  This means usually
	that either the children have changed completely or that the
	#visible() or #traverse() flags have changed in a way that
	the children of this twig should be updated.  */
    static const int STRUCTURE_MASK	= 1;

    /** Flag for #IgBrowserMethods::update() @c field indicating that
	the twig itself has changed: its name or any other content
	that is restricted to the twig itself but not affecting its
	children.  */
    static const int SELF_MASK		= 2;

    /** Flag for #IgBrowserMethods::update() @c field indicating that
	the flags for this twig have changed in a way that affects
	the whether this twig should have a representation or not,
	but not its children or (in general) what the representation
	should be like.  This is an optimisation flag that some
	reps can use to quickly turn themselves on or off without
	full recreation.  */
    static const int FLAGS_MASK		= 4;

    typedef lat::Callback1<IgDatum *&>		Getter;
    typedef lat::Callback1<const IgDatum *>	Setter;
    typedef std::pair<Getter,Setter>		GetSetPair;
    typedef std::map<std::string, GetSetPair>	Attributes;

    IgTwig (IgTwig *parent = 0);
    ~IgTwig (void);

    // access to hierarchy management
    virtual IgTwig *		root (void) const;
    virtual IgTwig *		parent (void) const;
    virtual unsigned		children (void);
    virtual IgTwig *		child (unsigned index);
    virtual unsigned		indexOf (const IgTwig *child) const;
    IgTwig *			lookup (const std::string &path);
    
    virtual bool		expand (void);
    virtual void		add (IgTwig *child);
    virtual void		remove (IgTwig *child);
    virtual void		destroy (void);
    virtual void		clear (void);

    // access to well-known attributes
    virtual std::string		name (void) const = 0;
    virtual void		name (const std::string &name) = 0;

    virtual std::string		fullName (void);    

    virtual bool		traverse (void) const;
    virtual bool		selfTraverse (void) const = 0;
    virtual void		selfTraverse (bool value) = 0;

    virtual bool		visible (void) const;
    virtual bool		selfVisible (void) const = 0;
    virtual void		selfVisible (bool value) = 0;

    virtual bool		cascade (void) const = 0;
    virtual void		cascade (bool value) = 0;

    // generic access to attributes
    virtual void		attributes (Attributes &attrs);

public:
    // for internal use only! (protected -- fails with gcc?)
    virtual void		added (IgTwig *parent);
    virtual void		removed (IgTwig *parent);
    virtual void		cleared (IgTwig *parent);

private:
    void			getNameAttribute (IgDatum *&result);
    void			getTraverseAttribute (IgDatum *&result);
    void			getVisibleAttribute (IgDatum *&result);
    void			getCascadeAttribute (IgDatum *&result);

    void			setNameAttribute (const IgDatum *value);
    void			setTraverseAttribute (const IgDatum *value);
    void			setVisibleAttribute (const IgDatum *value);
    void			setCascadeAttribute (const IgDatum *value);

    IgTwig			*m_parent;

    static const char		s_name[];
    static const char		s_traverse[];
    static const char		s_visible[];
    static const char		s_cascade[];

    // undefined semantics
    IgTwig (const IgTwig &);
    IgTwig &operator= (const IgTwig &);
};

class IGUANA_FRAMEWORK_API IgCompoundTwig : public IgTwig
{
public:
    IgCompoundTwig (IgTwig *parent = 0, unsigned sizeEstimate = 0);
    ~IgCompoundTwig (void);

    virtual unsigned		children (void);
    virtual IgTwig *		child (unsigned index);
    virtual unsigned		indexOf (const IgTwig *child) const;

    virtual bool		expand (void);
    virtual void		add (IgTwig *child);
    virtual void		remove (IgTwig *child);
    virtual void		destroy (void);
    virtual void		clear (void);

protected:
    static const unsigned int	EXPANDED = 1;

    unsigned			flags (void) const;
    void			flags (unsigned value);
    bool			flag (unsigned n) const;
    void			flag (unsigned n, bool value);

private:
    std::vector<IgTwig *>	m_children;
    unsigned			m_flags;
};

class IGUANA_FRAMEWORK_API IgSimpleTwig : public IgCompoundTwig
{
public:
    explicit
    IgSimpleTwig (IgTwig *parent, const std::string &name = "",
		  bool traverse = true, bool visible = true,
		  bool cascade = true);

    explicit
    IgSimpleTwig (const std::string &name,
		  bool traverse = true, bool visible = true,
		  bool cascade = true);
    // implicit destructor

    virtual std::string		name (void) const;
    virtual void		name (const std::string &name);

    virtual bool		selfTraverse (void) const;
    virtual void		selfTraverse (bool value);

    virtual bool		selfVisible (void) const;
    virtual void		selfVisible (bool value);

    virtual bool		cascade (void) const;
    virtual void		cascade (bool value);

protected:
    static const unsigned int	TRAVERSE = 2;
    static const unsigned int	VISIBLE = 4;
    static const unsigned int	CASCADE = 8;

private:
    std::string			m_name;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

inline bool
IgCompoundTwig::flag (unsigned n) const
{ return m_flags & n; }

inline void
IgCompoundTwig::flag (unsigned n, bool value)
{ m_flags = (m_flags & ~n) | (value * n); }

inline unsigned
IgCompoundTwig::flags (void) const
{ return m_flags; }

inline void
IgCompoundTwig::flags (unsigned value)
{ m_flags = value; }

#endif // IGUANA_FRAMEWORK_IG_TWIG_H
