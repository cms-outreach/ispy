#ifndef CLASSLIB_XTYPEINFO_H
# define CLASSLIB_XTYPEINFO_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/utils/DebugAids.h"
# include <typeinfo>
# include <iterator>
# include <map>
# include <vector>
# include <cstddef>

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>

/** Get type information.  A macro to allow both types and objects.  */
#  define xtypeid(arg)		(lat::XTypeInfo::find (typeid (arg)))

/** Get lazy type information.  Uses our internal mechanism to bind to
    the object's data address; however the information may not have
    been completed yet!  Please avoid using this one.  */
#  define xtypedata(arg)	(&lat::XTypeInfoData<arg>::s_type)

//usage:
//  class X					{ public: virtual ~X (); };
//  class Y : public X				{ };
//  class Z : public virtual X			{ };
//  class W : public virtual X, public Z	{ };
//  class V : public W				{ };
//
//  XTYPEINFO_DECLARE(API,X);
//  XTYPEINFO_DECLARE(API,Y);
//  XTYPEINFO_DECLARE(API,Z);
//  XTYPEINFO_DECLARE(API,W);
//  XTYPEINFO_DECLARE(API,V);
//
//  XTYPEINFO_DEF_0(X);
//  XTYPEINFO_DEF_1(Y, X,false);
//  XTYPEINFO_DEF_1(Z, X,true);
//  XTYPEINFO_DEF_2(W, X,true, Z,false);
//  XTYPEINFO_DEF_1(V, W,false);
//
// or, for templated names
//  XTYPEINFO_DECLARE(XTYPEINFO_T2(Type<X,Y>))
//  XTYPEINFO_DEF_0(XTYPEINFO_T2(Type<X,Y>))

#  define XTYPEINFO_T2(a,b)			a,b
#  define XTYPEINFO_T3(a,b,c)			a,b,c
#  define XTYPEINFO_T4(a,b,c,d)			a,b,c,d
#  define XTYPEINFO_T5(a,b,c,d,e)		a,b,c,d,e
#  define XTYPEINFO_T6(a,b,c,d,e,f)		a,b,c,d,e,f
#  define XTYPEINFO_T7(a,b,c,d,e,f,g)		a,b,c,d,e,f,g
#  define XTYPEINFO_T8(a,b,c,d,e,f,g,h)		a,b,c,d,e,f,g,h
#  define XTYPEINFO_T9(a,b,c,d,e,f,g,h,i)	a,b,c,d,e,f,g,h,i

/* FIXME: XTypeInfoCast does not work with Sun CC-5.0 -- it does not
   instantiate the static functions and there does not seem to be any
   way to force it to do so.  So just disable it for now, we are not
   using those fields anyway.  */

#  define XTYPEINFO_DECLARE(api,type)					\
    namespace lat {                                                       \
        template <> struct api XTypeInfoData< :: type>; \
    }                                                   \
    template <> struct api lat::XTypeInfoData< :: type> {     \
        static lat::XTypeInfo::ClassDef       s_type;             \
        static lat::XTypeInfo::BaseDef        s_bases[];              \
        static lat::XTypeInfoInit< :: type>   s_init;         \
    }

#  define XTYPEINFO_PDEF_0(type)						\
    lat::XTypeInfo::BaseDef lat::XTypeInfoData< type >::s_bases [] = {\
	{ 0, 0, 0, 0, 0 },							\
    };										\
    lat::XTypeInfo::ClassDef lat::XTypeInfoData< type >::s_type =	\
	{ 0, &s_bases[0], &s_bases[0], 0, 0, 0, 0, 0 }

#  define XTYPEINFO_DEF_0(type)							\
    lat::XTypeInfo::BaseDef lat::XTypeInfoData< type >::s_bases [] = {\
	{ 0, 0, 0, 0, 0 },							\
    };										\
    lat::XTypeInfo::ClassDef lat::XTypeInfoData< type >::s_type =	\
	{ 0, &s_bases[0], &s_bases[0], 0, 0, 0, 0, 0 };				\
    lat::XTypeInfoInit< type > lat::XTypeInfoData< type >::s_init (&s_type, s_bases)


#  define XTYPEINFO_PDEF_1(type,base1,mode1)					\
    lat::XTypeInfo::BaseDef lat::XTypeInfoData< type >::s_bases [] = {\
	{ 1, &lat::XTypeInfoData< base1 >::s_type, mode1,			\
	  0 /*&lat::XTypeInfoCast< type,base1 >::down*/,			\
	  0 /*&lat::XTypeInfoCast< type,base1 >::up*/},			\
	{ 0, 0, 0, 0, 0 },							\
    };										\
    lat::XTypeInfo::ClassDef lat::XTypeInfoData< type >::s_type =	\
	{ 0, &s_bases[0], &s_bases[1], 0, 0, 0, 0, 0 }

#  define XTYPEINFO_DEF_1(type,base1,mode1)					\
    lat::XTypeInfo::BaseDef lat::XTypeInfoData< type >::s_bases [] = {\
	{ 1, &lat::XTypeInfoData< base1 >::s_type, mode1,			\
	  0 /*&lat::XTypeInfoCast< type,base1 >::down */,			\
	  0 /*&lat::XTypeInfoCast< type,base1 >::up */},			\
	{ 0, 0, 0, 0, 0 },							\
    };										\
    lat::XTypeInfo::ClassDef lat::XTypeInfoData< type >::s_type =	\
	{ 0, &s_bases[0], &s_bases[1], 0, 0, 0, 0, 0 };				\
    lat::XTypeInfoInit< type > lat::XTypeInfoData< type >::s_init (&s_type, s_bases)

#  define XTYPEINFO_PDEF_2(type,base1,mode1,base2,mode2)			\
    lat::XTypeInfo::BaseDef lat::XTypeInfoData< type >::s_bases [] = {\
	{ 1, &lat::XTypeInfoData< base1>::s_type, mode1,			\
	  0 /*&lat::XTypeInfoCast< type,base1 >::down*/,			\
	  0 /*&lat::XTypeInfoCast< type,base1 >::up*/},			\
	{ 1, &lat::XTypeInfoData< base2 >::s_type, mode2,			\
	  0 /*&lat::XTypeInfoCast< type,base2 >::down*/,			\
	  0 /*&lat::XTypeInfoCast< type,base2 >::up*/},			\
	{ 0, 0, 0, 0, 0 },							\
    };										\
    lat::XTypeInfo::ClassDef lat::XTypeInfoData< type >::s_type =	\
	{ 0, &s_bases[0], &s_bases[2], 0, 0, 0, 0, 0 }

#  define XTYPEINFO_DEF_2(type,base1,mode1,base2,mode2)				\
    lat::XTypeInfo::BaseDef lat::XTypeInfoData< type >::s_bases [] = {\
	{ 1, &lat::XTypeInfoData< base1 >::s_type, mode1,			\
	  0 /*&lat::XTypeInfoCast< type,base1 >::down*/,			\
	  0 /*&lat::XTypeInfoCast< type,base1 >::up*/},			\
	{ 1, &lat::XTypeInfoData< base2 >::s_type, mode2,			\
	  0 /*&lat::XTypeInfoCast< type,base2 >::down*/,			\
	  0 /*&lat::XTypeInfoCast< type,base2 >::up*/},			\
	{ 0, 0, 0, 0, 0 },							\
    };										\
    lat::XTypeInfo::ClassDef lat::XTypeInfoData< type >::s_type =	\
	{ 0, &s_bases[0], &s_bases[2], 0, 0, 0, 0, 0 };				\
    lat::XTypeInfoInit< type > lat::XTypeInfoData< type >::s_init (&s_type, s_bases)

#  define XTYPEINFO_PDEF_3(type,base1,mode1,base2,mode2,base3,mode3)		\
    lat::XTypeInfo::BaseDef lat::XTypeInfoData< type >::s_bases [] = {\
	{ 1, &lat::XTypeInfoData< base1 >::s_type, mode1,			\
	  0 /*&lat::XTypeInfoCast< type,base1 >::down*/,			\
	  0 /*&lat::XTypeInfoCast< type,base1 >::up*/},			\
	{ 1, &lat::XTypeInfoData< base2 >::s_type, mode2,			\
	  0 /*&lat::XTypeInfoCast< type,base2 >::down*/,			\
	  0 /*&lat::XTypeInfoCast< type,base2 >::up*/},			\
	{ 1, &lat::XTypeInfoData< base3 >::s_type, mode3,			\
	  0 /*&lat::XTypeInfoCast< type,base3 >::down*/,			\
	  0 /*&lat::XTypeInfoCast< type,base3 >::up*/},			\
	{ 0, 0, 0, 0, 0 },							\
    };										\
    lat::XTypeInfo::ClassDef lat::XTypeInfoData< type >::s_type =	\
	{ 0, &s_bases[0], &s_bases[3], 0, 0, 0, 0, 0 }

#  define XTYPEINFO_DEF_3(type,base1,mode1,base2,mode2,base3,mode3)		\
    lat::XTypeInfo::BaseDef lat::XTypeInfoData< type >::s_bases [] = {\
	{ 1, &lat::XTypeInfoData< base1 >::s_type, mode1,			\
	  0 /*&lat::XTypeInfoCast< type,base1 >::down*/,			\
	  0 /*&lat::XTypeInfoCast< type,base1 >::up*/},			\
	{ 1, &lat::XTypeInfoData< base2 >::s_type, mode2,			\
	  0 /*&lat::XTypeInfoCast< type,base2 >::down*/,			\
	  0 /*&lat::XTypeInfoCast< type,base2 >::up*/},			\
	{ 1, &lat::XTypeInfoData< base3 >::s_type, mode3,			\
	  0 /*&lat::XTypeInfoCast< type,base3 >::down*/,			\
	  0 /*&lat::XTypeInfoCast< type,base3 >::up*/},			\
	{ 0, 0, 0, 0, 0 },							\
    };										\
    lat::XTypeInfo::ClassDef lat::XTypeInfoData< type >::s_type =	\
	{ 0, &s_bases[0], &s_bases[3], 0, 0, 0, 0, 0 };				\
    lat::XTypeInfoInit< type > lat::XTypeInfoData< type >::s_init (&s_type, s_bases)

#  define XTYPEINFO_PDEF_4(type,base1,mode1,base2,mode2,base3,mode3,base4,mode4)\
    lat::XTypeInfo::BaseDef lat::XTypeInfoData< type >::s_bases [] = {\
	{ 1, &lat::XTypeInfoData< base1 >::s_type, mode1,			\
	  0 /*&lat::XTypeInfoCast< type,base1 >::down*/,			\
	  0 /*&lat::XTypeInfoCast< type,base1 >::up*/},			\
	{ 1, &lat::XTypeInfoData< base2 >::s_type, mode2,			\
	  0 /*&lat::XTypeInfoCast< type,base2 >::down*/,			\
	  0 /*&lat::XTypeInfoCast< type,base2 >::up*/},			\
	{ 1, &lat::XTypeInfoData< base3 >::s_type, mode3,			\
	  0 /*&lat::XTypeInfoCast< type,base3 >::down*/,			\
	  0 /*&lat::XTypeInfoCast< type,base3 >::up*/},			\
	{ 1, &lat::XTypeInfoData< base4 >::s_type, mode4,			\
	  0 /*&lat::XTypeInfoCast< type,base4 >::down*/,			\
	  0 /*&lat::XTypeInfoCast< type,base4 >::up*/},			\
	{ 0, 0, 0, 0, 0 },							\
    };										\
    lat::XTypeInfo::ClassDef lat::XTypeInfoData< type >::s_type =	\
	{ 0, &s_bases[0], &s_bases[4], 0, 0, 0, 0, 0 }

#  define XTYPEINFO_DEF_4(type,base1,mode1,base2,mode2,base3,mode3,base4,mode4)	\
    lat::XTypeInfo::BaseDef lat::XTypeInfoData< type >::s_bases [] = {\
	{ 1, &lat::XTypeInfoData< base1 >::s_type, mode1,			\
	  0 /*&lat::XTypeInfoCast< type,base1 >::down*/,			\
	  0 /*&lat::XTypeInfoCast< type,base1 >::up*/},			\
	{ 1, &lat::XTypeInfoData< base2 >::s_type, mode2,			\
	  0 /*&lat::XTypeInfoCast< type,base2 >::down*/,			\
	  0 /*&lat::XTypeInfoCast< type,base2 >::up*/},			\
	{ 1, &lat::XTypeInfoData< base3 >::s_type, mode3,			\
	  0 /*&lat::XTypeInfoCast< type,base3 >::down*/,			\
	  0 /*&lat::XTypeInfoCast< type,base3 >::up*/},			\
	{ 1, &lat::XTypeInfoData< base4 >::s_type, mode4,			\
	  0 /*&lat::XTypeInfoCast< type,base4 >::down*/,			\
	  0 /*&lat::XTypeInfoCast< type,base4 >::up*/},			\
	{ 0, 0, 0, 0, 0 },							\
    };										\
    lat::XTypeInfo::ClassDef lat::XTypeInfoData< type >::s_type =	\
	{ 0, &s_bases[0], &s_bases[4], 0, 0, 0, 0, 0 };				\
    lat::XTypeInfoInit< type > lat::XTypeInfoData< type >::s_init (&s_type, s_bases)

//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class XTypeInfo;
template <class T> struct XTypeInfoInit;
template <class T> struct XTypeInfoData;
template <class D, class B> struct XTypeInfoCast;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class XTypeInfo
{
public:
    struct BaseDef;
    struct ClassDef;
    class BaseInfo;
    class DerivedIterator;
    class BaseIterator;
    class Monitor;

    class Holder
    {
    public:
        Holder (const std::type_info &info);
	
	bool			operator== (const Holder &x) const;
	bool			operator<  (const Holder &x) const;

        const std::type_info	&m_std;
    };

    typedef unsigned int	ExtensionKey;
    typedef void *		Extension;
    typedef std::map<Holder, ClassDef *> TypeTable;

    struct ClassDef
    {
        XTypeInfo	*m_types;
	
	BaseDef		*m_basesBegin;
	BaseDef		*m_basesEnd;

	ClassDef	**m_derivedBegin;
	ClassDef	**m_derivedEnd;
	ClassDef	**m_derivedLimit;
	
	Extension	*m_extensionsBegin;
	Extension	*m_extensionsEnd;
    };

    struct BaseDef
    {
	int		m_distance;
    	ClassDef	*m_basedef;
	bool		m_virtual;
	void *		(*m_downcast) (void *object);
	void *		(*m_upcast) (void *object);
    };

    class BaseInfo
    {
    public:
	// implicit copy constructor
	// implicit assignment operator
	// implicit destructor

	BaseInfo *		operator-> (void);

	int			distance (void) const;
	XTypeInfo *		base (void) const;
	bool			isVirtual (void) const;

	void *			downcast (void *object) const;
	void *			upcast (void *object) const;

    private:
	friend class BaseIterator;

	BaseInfo (const BaseDef *def);
	const BaseDef		*m_def;
    };

    class DerivedIterator ITERATOR_BASE (random_access_iterator, XTypeInfo *, STDC::ptrdiff_t)
    {
    public:
	typedef std::random_access_iterator_tag		iterator_category;
	typedef XTypeInfo *				value_type;
	typedef STDC::ptrdiff_t				difference_type;
	typedef XTypeInfo * const&			reference;
	typedef XTypeInfo * const*			pointer;

	DerivedIterator (void);
	// implicit copy constructor
	// implicit assignment operator
	// implicit destructor
	
	bool			operator== (const DerivedIterator &x) const;
	bool			operator<  (const DerivedIterator &x) const;
	
	XTypeInfo *		operator* (void) const;
	XTypeInfo *		operator[] (STDC::ptrdiff_t n) const;
	
	DerivedIterator &	operator++ (void);
	DerivedIterator		operator++ (int);
	DerivedIterator &	operator-- (void);
	DerivedIterator		operator-- (int);
	DerivedIterator &	operator+= (STDC::ptrdiff_t n);
	DerivedIterator &	operator-= (STDC::ptrdiff_t n);
	
	friend DerivedIterator	operator+ (const DerivedIterator &x, STDC::ptrdiff_t n);
	friend DerivedIterator	operator+ (STDC::ptrdiff_t n, const DerivedIterator &x);
	friend DerivedIterator	operator- (const DerivedIterator &x, STDC::ptrdiff_t n);
	friend STDC::ptrdiff_t	operator- (const DerivedIterator &x,
					   const DerivedIterator &y);
					   
    private:
	friend class XTypeInfo;
    	DerivedIterator (ClassDef **def);
	
	ClassDef		**m_def;
    };

    // FIXME: should be `BaseInfo', not `BaseInfo *', but Sun CC 5.0 chokes on that.
    class BaseIterator ITERATOR_BASE (random_access_iterator, BaseInfo *, STDC::ptrdiff_t)
    {
    public:
	typedef std::random_access_iterator_tag		iterator_category;
	typedef BaseInfo				value_type;
	typedef STDC::ptrdiff_t				difference_type;
	typedef BaseInfo				reference;
	typedef BaseInfo				pointer;

	BaseIterator (void);
	// implicit copy constructor
	// implicit assignment operator
	// implicit destructor
	
	bool			operator== (const BaseIterator &x) const;
	bool			operator<  (const BaseIterator &x) const;
	
	BaseInfo		operator* (void) const;
	BaseInfo		operator-> (void) const;
	BaseInfo		operator[] (STDC::ptrdiff_t n) const;
	
	BaseIterator &		operator++ (void);
	BaseIterator		operator++ (int);
	BaseIterator &		operator-- (void);
	BaseIterator		operator-- (int);
	BaseIterator &		operator+= (STDC::ptrdiff_t n);
	BaseIterator &		operator-= (STDC::ptrdiff_t n);
	
	friend BaseIterator	operator+ (const BaseIterator &x, STDC::ptrdiff_t n);
	friend BaseIterator	operator+ (STDC::ptrdiff_t n, const BaseIterator &x);
	friend BaseIterator	operator- (const BaseIterator &x, STDC::ptrdiff_t n);
	friend STDC::ptrdiff_t	operator- (const BaseIterator &x,
					   const BaseIterator &y);
					   
    private:
	friend class XTypeInfo;
    	BaseIterator (const BaseDef *def);
	
	const BaseDef		*m_def;
    };

    class TypeIterator ITERATOR_BASE(bidirectional_iterator,
						  XTypeInfo *,
						  TypeTable::difference_type)
    {
    public:
	TypeIterator (void);
	// implicit copy constructor
	// implicit destructor
	// implicit assignment operator

	bool			operator== (const TypeIterator &x) const;

	XTypeInfo *		operator* (void) const;

	TypeIterator &		operator++ (void);
	TypeIterator		operator++ (int);
	TypeIterator &		operator-- (void);
	TypeIterator		operator-- (int);

    private:
	friend class XTypeInfo;
	TypeIterator (TypeTable::const_iterator iterator);

	TypeTable::const_iterator	m_iterator;
    };

    class Monitor
    {
    public:
	virtual ~Monitor (void);
	virtual void		typeHook (XTypeInfo *type) = 0;
	virtual	void		typeUnhook (XTypeInfo *type) = 0;
    };

    // class interface: operations on all extended types
    static ExtensionKey		allocateExtension (void);

    static void			addMonitor (Monitor *who, bool notify);
    static void			removeMonitor (Monitor *who, bool notify);

    static TypeIterator		beginTypes (void);
    static TypeIterator		endTypes (void);
    static XTypeInfo *		find (const std::type_info &std_info);
    static void *		cast (const XTypeInfo *to,
				      const XTypeInfo *from,
				      void *object);

    XTypeInfo (const std::type_info &std_info, ClassDef *def, BaseDef *bases);
    ~XTypeInfo (void);
    
    // access to standard and extended information
    const std::type_info &	standard (void) const;
    Extension			getExtension (ExtensionKey which) const;
    Extension			setExtension (ExtensionKey which, Extension value);

    // access to related classes
    BaseIterator		beginBases (void) const;
    BaseIterator		endBases (void) const;
    BaseIterator		base (const XTypeInfo *type) const;
    bool			hasBase (const XTypeInfo *type) const;

    DerivedIterator		beginDerived (void) const;
    DerivedIterator		endDerived (void) const;
    DerivedIterator		derived (const XTypeInfo *type) const;
    bool			hasDerived (const XTypeInfo *type) const;

    typedef std::vector<Monitor *> HookList;
private:
    // FIXME: for Sun CC 4.2: typedef std::vector<Monitor *> HookList; 

    static void			registerType (XTypeInfo *type);
    static void			deregisterType (XTypeInfo *type);

    static void			countBases (ClassDef *def, int &count);
    static void			collectBases (ClassDef *def, BaseDef *first,
					      BaseDef *&next, int distance);
    static void			removeDerived (ClassDef *type, ClassDef *from);

    static TypeTable &		types (void);
    static ExtensionKey &	extensions (void);
    static HookList &		hooks (void);

    const std::type_info	&m_std;
    ClassDef			*m_def;
    ClassDef			*m_activeDef;
    XTypeInfo			*m_nextEqual;

    XTypeInfo (const XTypeInfo &);
    XTypeInfo &operator= (const XTypeInfo &);
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

template <class T>
struct XTypeInfoInit
{
    XTypeInfoInit (XTypeInfo::ClassDef *def, XTypeInfo::BaseDef *bases);
    XTypeInfo m_info;
};

template <class D, class B>
struct XTypeInfoCast
{
    static void *		down (void *object);
    static void *		up (void *object);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>

// traverse the XTypeInfo trees, using downcast's to convert the object
template <class T>
inline void *xdynamic_cast(const XTypeInfo *to, T *object)
{ return XTypeInfo::cast (to, xtypeid (T), static_cast<void *> (object)); }

//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

inline
XTypeInfo::Holder::Holder (const std::type_info &info)
    : m_std (info)
{ }

inline bool
XTypeInfo::Holder::operator== (const Holder &x) const
{ return m_std == x.m_std; }

inline bool
XTypeInfo::Holder::operator< (const Holder &x) const
{ return m_std.before (x.m_std); }

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

inline
XTypeInfo::BaseInfo::BaseInfo (const BaseDef *def)
    : m_def (def)
{ }

inline XTypeInfo::BaseInfo *
XTypeInfo::BaseInfo::operator-> (void)
{ return this; }

inline int
XTypeInfo::BaseInfo::distance (void) const
{ return m_def->m_distance; }

inline XTypeInfo *
XTypeInfo::BaseInfo::base (void) const
{ return m_def->m_basedef->m_types; }

inline bool
XTypeInfo::BaseInfo::isVirtual (void) const
{ return m_def->m_virtual; }

inline void *
XTypeInfo::BaseInfo::downcast (void *object) const
{ return (*m_def->m_downcast) (object); }

inline void *
XTypeInfo::BaseInfo::upcast (void *object) const
{ return (*m_def->m_upcast) (object); }

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

inline
XTypeInfo::DerivedIterator::DerivedIterator (void)
    : m_def (0)
{ }

inline
XTypeInfo::DerivedIterator::DerivedIterator (ClassDef **def)
    : m_def (def)
{ }

inline bool
XTypeInfo::DerivedIterator::operator== (const DerivedIterator &x) const
{ return m_def == x.m_def; }

inline bool
XTypeInfo::DerivedIterator::operator< (const DerivedIterator &x) const
{ return m_def < x.m_def; }

inline XTypeInfo *
XTypeInfo::DerivedIterator::operator* (void) const
{ ASSERT (m_def); ASSERT ((*m_def)->m_types); return (*m_def)->m_types; }

inline XTypeInfo *
XTypeInfo::DerivedIterator::operator[] (STDC::ptrdiff_t n) const
{ ASSERT (m_def); ASSERT (m_def[n]->m_types); return m_def[n]->m_types; }

inline XTypeInfo::DerivedIterator &
XTypeInfo::DerivedIterator::operator++ (void)
{ ++m_def; return *this; }

inline XTypeInfo::DerivedIterator
XTypeInfo::DerivedIterator::operator++ (int)
{ return DerivedIterator (m_def++); }

inline XTypeInfo::DerivedIterator &
XTypeInfo::DerivedIterator::operator-- (void)
{ --m_def; return *this; }

inline XTypeInfo::DerivedIterator
XTypeInfo::DerivedIterator::operator-- (int)
{ return DerivedIterator (m_def--); }

inline XTypeInfo::DerivedIterator &
XTypeInfo::DerivedIterator::operator+= (STDC::ptrdiff_t n)
{ m_def += n; return *this; }

inline XTypeInfo::DerivedIterator &
XTypeInfo::DerivedIterator::operator-= (STDC::ptrdiff_t n)
{ m_def -= n; return *this; }

inline XTypeInfo::DerivedIterator
operator+ (const XTypeInfo::DerivedIterator &x, STDC::ptrdiff_t n)
{ return XTypeInfo::DerivedIterator (x.m_def + n); }

inline XTypeInfo::DerivedIterator
operator+ (STDC::ptrdiff_t n, const XTypeInfo::DerivedIterator &x)
{ return XTypeInfo::DerivedIterator (x.m_def + n); }

inline XTypeInfo::DerivedIterator
operator- (const XTypeInfo::DerivedIterator &x, STDC::ptrdiff_t n)
{ return XTypeInfo::DerivedIterator (x.m_def - n); }

inline STDC::ptrdiff_t
operator- (const XTypeInfo::DerivedIterator &x, const XTypeInfo::DerivedIterator &y)
{ return x.m_def - y.m_def; }

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

inline
XTypeInfo::BaseIterator::BaseIterator (void)
    : m_def (0)
{ }

inline
XTypeInfo::BaseIterator::BaseIterator (const BaseDef *def)
    : m_def (def)
{ }

inline bool
XTypeInfo::BaseIterator::operator== (const BaseIterator &x) const
{ return m_def == x.m_def; }

inline bool
XTypeInfo::BaseIterator::operator< (const BaseIterator &x) const
{ return m_def < x.m_def; }

inline XTypeInfo::BaseInfo
XTypeInfo::BaseIterator::operator* (void) const
{ ASSERT (m_def); return BaseInfo (m_def); }

inline XTypeInfo::BaseInfo
XTypeInfo::BaseIterator::operator-> (void) const
{ ASSERT (m_def); return BaseInfo (m_def); }

inline XTypeInfo::BaseInfo
XTypeInfo::BaseIterator::operator[] (STDC::ptrdiff_t n) const
{ ASSERT (m_def); return BaseInfo (m_def+n); }

inline XTypeInfo::BaseIterator &
XTypeInfo::BaseIterator::operator++ (void)
{ ++m_def; return *this; }

inline XTypeInfo::BaseIterator
XTypeInfo::BaseIterator::operator++ (int)
{ return BaseIterator (m_def++); }

inline XTypeInfo::BaseIterator &
XTypeInfo::BaseIterator::operator-- (void)
{ --m_def; return *this; }

inline XTypeInfo::BaseIterator
XTypeInfo::BaseIterator::operator-- (int)
{ return BaseIterator (m_def--); }

inline XTypeInfo::BaseIterator &
XTypeInfo::BaseIterator::operator+= (STDC::ptrdiff_t n)
{ m_def += n; return *this; }

inline XTypeInfo::BaseIterator &
XTypeInfo::BaseIterator::operator-= (STDC::ptrdiff_t n)
{ m_def -= n; return *this; }

inline XTypeInfo::BaseIterator
operator+ (const XTypeInfo::BaseIterator &x, STDC::ptrdiff_t n)
{ return XTypeInfo::BaseIterator (x.m_def + n); }

inline XTypeInfo::BaseIterator
operator+ (STDC::ptrdiff_t n, const XTypeInfo::BaseIterator &x)
{ return XTypeInfo::BaseIterator (x.m_def + n); }

inline XTypeInfo::BaseIterator
operator- (const XTypeInfo::BaseIterator &x, STDC::ptrdiff_t n)
{ return XTypeInfo::BaseIterator (x.m_def - n); }

inline STDC::ptrdiff_t
operator- (const XTypeInfo::BaseIterator &x, const XTypeInfo::BaseIterator &y)
{ return x.m_def - y.m_def; }

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

inline
XTypeInfo::TypeIterator::TypeIterator (void)
{}

inline
XTypeInfo::TypeIterator::TypeIterator (TypeTable::const_iterator iterator)
    : m_iterator (iterator)
{}

inline bool
XTypeInfo::TypeIterator::operator== (const TypeIterator &x) const
{ return m_iterator == x.m_iterator; }

inline XTypeInfo *
XTypeInfo::TypeIterator::operator* (void) const
{ return (*m_iterator).second->m_types; }

inline XTypeInfo::TypeIterator &
XTypeInfo::TypeIterator::operator++ (void)
{ ++m_iterator; return *this; }

inline XTypeInfo::TypeIterator
XTypeInfo::TypeIterator::operator++ (int)
{ TypeIterator tmp (m_iterator); ++m_iterator; return tmp; }

inline XTypeInfo::TypeIterator &
XTypeInfo::TypeIterator::operator-- (void)
{ --m_iterator; return *this; }

inline XTypeInfo::TypeIterator
XTypeInfo::TypeIterator::operator-- (int)
{ TypeIterator tmp (m_iterator); --m_iterator; return tmp; }

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

inline XTypeInfo::TypeIterator
XTypeInfo::beginTypes (void)
{ return TypeIterator (types ().begin ()); }

inline XTypeInfo::TypeIterator
XTypeInfo::endTypes (void)
{ return TypeIterator (types ().end ()); }

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

template <class D, class B>
inline void *XTypeInfoCast<D,B>::down (void *object)
{ return dynamic_cast<D *> (static_cast<B *> (object)); }

template <class D, class B>
inline void *XTypeInfoCast<D,B>::up (void *object)
{ return static_cast<B *> (static_cast<D *> (object)); }

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
template <class T>
inline XTypeInfoInit<T>::XTypeInfoInit (XTypeInfo::ClassDef *def,
					XTypeInfo::BaseDef *bases)
    : m_info (typeid (T), def, bases)
{ }

} // namespace lat
#endif // CLASSLIB_XTYPEINFO_H
