#ifndef CLASSLIB_HOOK_H
# define CLASSLIB_HOOK_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/sysapi/system.h"

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

template <class R>
class HookRep 
{
public:
    HookRep (void) : m_refs (0) { }
    virtual ~HookRep (void) { }

    virtual R		call (void) = 0;
    // virtual void	equal (const HookRep *x) const;
    void		ref (void)	{ ++m_refs; }
    void		unref (void)	{ if (--m_refs == 0) delete this; }

private:
    int			m_refs;
};

template <class R>
class Hook
{
public:
    Hook (HookRep<R> *implementation = 0);
    Hook (const Hook &x);
    ~Hook (void);
    Hook &		operator= (const Hook &x);

    // bool		operator== (const Hook &x) const;
    /**/		operator bool (void) const;
    R			operator() (void) const;

private:
    HookRep<R>		*m_rep;
};

//////////////////////////////////////////////////////////////////////
template <class R, class T1>
class Hook1Rep
{
public:
    Hook1Rep (void) : m_refs (0) { }
    virtual ~Hook1Rep (void) { }

    virtual R		call (T1) = 0;
    void		ref (void)	{ ++m_refs; }
    void		unref (void)	{ if (--m_refs == 0) delete this; }

private:
    int			m_refs;
};

template <class R, class T1>
class Hook1
{
public:
    Hook1 (Hook1Rep<R,T1> *implementation = 0);
    Hook1 (const Hook1 &x);
    ~Hook1 (void);
    Hook1 &		operator= (const Hook1 &x);

    // bool		operator== (const Hook1 &x) const;
    /**/		operator bool (void) const;
    R			operator() (T1) const;

private:
    Hook1Rep<R,T1>	*m_rep;
};

//////////////////////////////////////////////////////////////////////
template <class R, class T1, class T2>
class Hook2Rep
{
public:
    Hook2Rep (void) : m_refs (0) { }
    virtual ~Hook2Rep (void) { }

    virtual R		call (T1, T2) = 0;
    void		ref (void)	{ ++m_refs; }
    void		unref (void)	{ if (--m_refs == 0) delete this; }

private:
    int			m_refs;
};

template <class R, class T1, class T2>
class Hook2
{
public:
    Hook2 (Hook2Rep<R,T1,T2> *implementation = 0);
    Hook2 (const Hook2 &x);
    ~Hook2 (void);
    Hook2 &		operator= (const Hook2 &x);

    // bool		operator== (const Hook2 &x) const;
    /**/		operator bool (void) const;
    R			operator() (T1, T2) const;

private:
    Hook2Rep<R,T1,T2>	*m_rep;
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
template <class R>
class HookImpF00 : public HookRep<R>
{
public:
    HookImpF00 (R (*function) (void))
	: m_function (function)
    { }

    virtual R call (void)
    { return (*m_function) (); }

private:
    R		(*m_function) (void);
};

/////////////////////////////////////////////////////////////////////////////
template <class C, class R>
class HookImpC00 : public HookRep<R>
{
public:
    HookImpC00 (C *object, R (C::*function) (void))
	: m_object (object),
	  m_function (function)
    { }

    virtual R call (void)
    { return (m_object->*m_function) (); }

private:
    C		*m_object;
    R		(C::*m_function) (void);
};

/////////////////////////////////////////////////////////////////////////////
template <class R, class T1>
class HookImpF01 : public HookRep<R>
{
public:
    HookImpF01 (R (*function) (T1), const T1 &fill_1)
	: m_function (function),
	  m_fill_1 (fill_1)
    { }

    virtual R call (void)
    { return (*m_function) (m_fill_1); }

private:
    R		(*m_function) (T1);
    T1		m_fill_1;
};

/////////////////////////////////////////////////////////////////////////////
template <class C, class R, class T1>
class HookImpC01 : public HookRep<R>
{
public:
    HookImpC01 (C *object, R (C::*function) (T1), const T1 &fill_1)
	: m_object (object),
	  m_function (function),
	  m_fill_1 (fill_1)
    { }

    virtual R call (void)
    { return (m_object->*m_function) (m_fill_1); }

private:
    C		*m_object;
    R		(C::*m_function) (T1);
    T1		m_fill_1;
};

/////////////////////////////////////////////////////////////////////////////
template <class R, class T1, class T2>
class HookImpF02 : public HookRep<R>
{
public:
    HookImpF02 (R (*function) (T1, T2), const T1 &fill_1, const T2 &fill_2)
	: m_function (function),
	  m_fill_1 (fill_1),
	  m_fill_2 (fill_2)
    { }

    virtual R call (void)
    { return (*m_function) (m_fill_1, m_fill_2); }

private:
    R		(*m_function) (T1, T2);
    T1		m_fill_1;
    T2		m_fill_2;
};

/////////////////////////////////////////////////////////////////////////////
template <class C, class R, class T1, class T2>
class HookImpC02 : public HookRep<R>
{
public:
    HookImpC02 (C *object, R (C::*function) (T1,T2), const T1 &fill_1, const T2 &fill_2)
	: m_object (object),
	  m_function (function),
	  m_fill_1 (fill_1),
	  m_fill_2 (fill_2)
    { }

    virtual R call (void)
    { return (m_object->*m_function) (m_fill_1, m_fill_2); }

private:
    C		*m_object;
    R		(C::*m_function) (T1);
    T1		m_fill_1;
    T2		m_fill_2;
};

/////////////////////////////////////////////////////////////////////////////
template <class R, class T1>
class HookImpF10 : public Hook1Rep<R,T1>
{
public:
    HookImpF10 (R (*function) (T1))
	: m_function (function)
    { }

    virtual R call (T1 a)
    { return (*m_function) (a); }

private:
    R		(*m_function) (T1);
};

/////////////////////////////////////////////////////////////////////////////
template <class C, class R, class T1>
class HookImpC10 : public Hook1Rep<R,T1>
{
public:
    HookImpC10 (C *object, R (C::*function) (T1))
	: m_object (object),
	  m_function (function)
    { }

    virtual R call (T1 a)
    { return (m_object->*m_function) (a); }

private:
    C		*m_object;
    R		(C::*m_function) (T1);
};

/////////////////////////////////////////////////////////////////////////////
template <class R, class T1, class T2>
class HookImpF11 : public Hook1Rep<R,T1>
{
public:
    HookImpF11 (R (*function) (T1, T2), const T2 &fill_2)
	: m_function (function),
	  m_fill_2 (fill_2)
    { }

    virtual R call (T1 a)
    { return (*m_function) (a, m_fill_2); }

private:
    R		(*m_function) (T1, T2);
    T2		m_fill_2;
};

/////////////////////////////////////////////////////////////////////////////
template <class C, class R, class T1, class T2>
class HookImpC11 : public Hook1Rep<R,T1>
{
public:
    HookImpC11 (C *object, R (C::*function) (T1, T2), const T2 &fill_2)
	: m_object (object),
	  m_function (function),
	  m_fill_2 (fill_2)
    { }

    virtual R call (T1 a)
    { return (m_object->*m_function) (a, m_fill_2); }

private:
    C		*m_object;
    R		(C::*m_function) (T1, T2);
    T2		m_fill_2;
};

/////////////////////////////////////////////////////////////////////////////
template <class R, class T1, class T2, class T3>
class HookImpF12 : public Hook1Rep<R,T1>
{
public:
    HookImpF12 (R (*function) (T1, T2, T3), const T2 &fill_2, const T3 &fill_3)
	: m_function (function),
	  m_fill_2 (fill_2),
	  m_fill_3 (fill_3)
    { }

    virtual R call (T1 a)
    { return (*m_function) (a, m_fill_2, m_fill_3); }

private:
    R		(*m_function) (T1, T2, T3);
    T2		m_fill_2;
    T3		m_fill_3;
};

/////////////////////////////////////////////////////////////////////////////
template <class C, class R, class T1, class T2, class T3>
class HookImpC12 : public Hook1Rep<R,T1>
{
public:
    HookImpC12 (C *object, R (C::*function) (T1, T2, T3), const T2 &fill_2, const T3 &fill_3)
	: m_object (object),
	  m_function (function),
	  m_fill_2 (fill_2),
	  m_fill_3 (fill_3)
    { }

    virtual R call (T1 a)
    { return (m_object->*m_function) (a, m_fill_2, m_fill_3); }

private:
    C		*m_object;
    R		(C::*m_function) (T1, T2, T3);
    T2		m_fill_2;
    T3		m_fill_3;
};

/////////////////////////////////////////////////////////////////////////////
template <class R, class T1, class T2>
class HookImpF20 : public  Hook2Rep<R,T1,T2>
{
public:
    HookImpF20 (R (*function) (T1, T2))
	: m_function (function)
    { }

    virtual R call (T1 a, T2 b)
    { return (*m_function) (a, b); }

private:
    R		(*m_function) (T1, T2);
};

/////////////////////////////////////////////////////////////////////////////
template <class C, class R, class T1, class T2>
class HookImpC20 : public Hook2Rep<R,T1,T2>
{
public:
    HookImpC20 (C *object, R (C::*function) (T1, T2))
	: m_object (object),
	  m_function (function)
    { }

    virtual R call (T1 a, T2 b)
    { return (m_object->*m_function) (a, b); }

private:
    C		*m_object;
    R		(C::*m_function) (T1, T2);
};

/////////////////////////////////////////////////////////////////////////////
template <class R, class T1, class T2, class T3>
class HookImpF21 : public Hook2Rep<R,T1,T2>
{
public:
    HookImpF21 (R (*function) (T1, T2, T3), const T3 &fill_3)
	: m_function (function),
	  m_fill_3 (fill_3)
    { }

    virtual R call (T1 a, T2 b)
    { return (*m_function) (a, b, m_fill_3); }

private:
    R		(*m_function) (T1, T2, T3);
    T3		m_fill_3;
};

/////////////////////////////////////////////////////////////////////////////
template <class C, class R, class T1, class T2, class T3>
class HookImpC21 : public Hook2Rep<R,T1,T2>
{
public:
    HookImpC21 (C *object, R (C::*function) (T1, T2, T3), const T3 &fill_3)
	: m_object (object),
	  m_function (function),
	  m_fill_3 (fill_3)
    { }

    virtual R call (T1 a, T2 b)
    { return (m_object->*m_function) (a, b, m_fill_3); }

private:
    C		*m_object;
    R		(C::*m_function) (T1, T2, T3);
    T3		m_fill_3;
};

/////////////////////////////////////////////////////////////////////////////
template <class R, class T1, class T2, class T3, class T4>
class HookImpF22 : public Hook2Rep<R,T1,T2>
{
public:
    HookImpF22 (R (*function) (T1, T2, T3, T4), const T3 &fill_3, const T4 &fill_4)
	: m_function (function),
	  m_fill_3 (fill_3),
	  m_fill_4 (fill_4)
    { }

    virtual R call (T1 a, T2 b)
    { return (*m_function) (a, b, m_fill_3, m_fill_4); }

private:
    R		(*m_function) (T1, T2, T3, T4);
    T3		m_fill_3;
    T4		m_fill_4;
};

/////////////////////////////////////////////////////////////////////////////
template <class C, class R, class T1, class T2, class T3, class T4>
class HookImpC22 : public Hook2Rep<R,T1,T2>
{
public:
    HookImpC22 (C *object, R (C::*function) (T1, T2, T3, T4), const T3 &fill_3, const T4 &fill_4)
	: m_object (object),
	  m_function (function),
	  m_fill_3 (fill_3),
	  m_fill_4 (fill_4)
    { }

    virtual R call (T1 a, T2 b)
    { return (m_object->*m_function) (a, b, m_fill_3, m_fill_4); }

private:
    C		*m_object;
    R		(C::*m_function) (T1, T2, T3, T4);
    T3		m_fill_3;
    T4		m_fill_4;
};

/////////////////////////////////////////////////////////////////////////////

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>

template <class R>
inline HookRep<R> *
CreateHook (R (*function) (void))
{ return new HookImpF00<R> (function); }

template <class C, class R>
inline HookRep<R> *
CreateHook (C *object, R (C::*function) (void))
{ return new HookImpC00<C,R> (object, function); }

template <class R, class T1>
inline HookRep<R> *
CreateHook (R (*function) (T1),
	    const T1 &fill_1)
{ return new HookImpF01<R,T1> (function, fill_1); }

template <class C, class R, class T1>
inline HookRep<R> *
CreateHook (C *object, R (C::*function) (T1),
	    const T1 &fill_1)
{ return new HookImpC01<C,R,T1> (object, function, fill_1); }

template <class R, class T1, class T2>
inline HookRep<R> *
CreateHook (R (*function) (T1, T2),
	    const T1 &fill_1, const T2 &fill_2)
{ return new HookImpF02<R,T1,T2> (function, fill_1, fill_2); }

template <class C, class R, class T1, class T2>
inline HookRep<R> *
CreateHook (C *object, R (C::*function) (T1,T2),
	    const T1 &fill_1, const T2 &fill_2)
{ return new HookImpC20<C,R,T1,T2> (object, function, fill_1, fill_2); }

template <class R, class T1>
inline Hook1Rep<R,T1> *
CreateHook (R (*function) (T1))
{ return new HookImpF10<R,T1> (function); }

template <class C, class R, class T1>
inline Hook1Rep<R,T1> *
CreateHook (C *object, R (C::*function) (T1))
{ return new HookImpC10<C,R,T1> (object, function); }

template <class R, class T1, class T2>
inline Hook1Rep<R,T1> *
CreateHook (R (*function) (T1, T2),
	    const T2 &fill_2)
{ return new HookImpF11<R,T1,T2> (function, fill_2); }

template <class C, class R, class T1, class T2>
inline Hook1Rep<R,T1> *
CreateHook (C *object, R (C::*function) (T1, T2),
	    const T2 &fill_2)
{ return new HookImpC11<C,R,T1,T2> (object, function, fill_2); }

template <class R, class T1, class T2, class T3>
inline Hook1Rep<R,T1> *
CreateHook (R (*function) (T1, T2, T3),
	    const T2 &fill_2, const T3 &fill_3)
{ return new HookImpF12<R,T1,T2,T3> (function, fill_2, fill_3); }

template <class C, class R, class T1, class T2, class T3>
inline Hook1Rep<R,T1> *
CreateHook (C *object, R (C::*function) (T1, T2, T3),
	    const T2 &fill_2, const T3 &fill_3)
{ return new HookImpC12<C,R,T1,T2,T3> (object, function, fill_2, fill_3); }

template <class R, class T1, class T2>
inline Hook2Rep<R,T1,T2> *
CreateHook (R (*function) (T1, T2))
{ return new HookImpF20<R,T1,T2> (function); }

template <class C, class R, class T1, class T2>
inline Hook2Rep<R,T1,T2> *
CreateHook (C *object, R (C::*function) (T1, T2))
{ return new HookImpC20<C,R,T1,T2> (object, function); }

template <class R, class T1, class T2, class T3>
inline Hook2Rep<R,T1,T2> *
CreateHook (R (*function) (T1, T2, T3),
	    const T3 &fill_3)
{ return new HookImpF21<R,T1,T2,T3> (function, fill_3); }

template <class C, class R, class T1, class T2, class T3>
inline Hook2Rep<R,T1,T2> *
CreateHook (C *object, R (C::*function) (T1, T2, T3),
	    const T3 &fill_3)
{ return new HookImpC21<C,R,T1,T2,T3> (object, function, fill_3); }

template <class R, class T1, class T2, class T3, class T4>
inline Hook2Rep<R,T1,T2> *
CreateHook (R (*function) (T1, T2, T3, T4),
	    const T3 &fill_3, const T4 &fill_4)
{ return new HookImpF22<R,T1,T2,T3,T4> (function, fill_3, fill_4); }

template <class C, class R, class T1, class T2, class T3, class T4>
inline Hook2Rep<R,T1,T2> *
CreateHook (C *object, R (C::*function) (T1, T2, T3, T4),
	    const T3 &fill_3, const T4 &fill_4)
{ return new HookImpC22<C,R,T1,T2,T3,T4> (object, function, fill_3, fill_4); }

//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

template <class R>
inline
Hook<R>::Hook (HookRep<R> *implementation /* = 0 */)
    : m_rep (implementation)
{ if (m_rep) m_rep->ref (); }

template <class R>
inline
Hook<R>::Hook (const Hook<R> &x)
    : m_rep (x.m_rep)
{ if (m_rep) m_rep->ref (); }

template <class R>
inline
Hook<R>::~Hook (void)
{ if (m_rep) m_rep->unref (); }

template <class R>
inline Hook<R> &
Hook<R>::operator= (const Hook<R> &x)
{
    if (m_rep != x.m_rep)
    {
	if (m_rep) m_rep->unref ();
	m_rep = x.m_rep;
	if (m_rep) m_rep->ref ();
    }
    return *this;
}

template <class R>
inline
Hook<R>::operator bool (void) const
{ return m_rep != 0; }

template <class R>
inline R
Hook<R>::operator() (void) const
{ return m_rep->call (); }

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
template <class R, class T1>
inline
Hook1<R,T1>::Hook1 (Hook1Rep<R,T1> *implementation /* = 0 */)
    : m_rep (implementation)
{ if (m_rep) m_rep->ref (); }

template <class R, class T1>
inline
Hook1<R,T1>::Hook1 (const Hook1<R,T1> &x)
    : m_rep (x.m_rep)
{ if (m_rep) m_rep->ref (); }

template <class R, class T1>
inline
Hook1<R,T1>::~Hook1 (void)
{ if (m_rep) m_rep->unref (); }

template <class R, class T1>
inline Hook1<R,T1> &
Hook1<R,T1>::operator= (const Hook1<R,T1> &x)
{
    if (m_rep != x.m_rep)
    {
	if (m_rep) m_rep->unref ();
	m_rep = x.m_rep;
	if (m_rep) m_rep->ref ();
    }
    return *this;
}

template <class R, class T1>
inline
Hook1<R,T1>::operator bool (void) const
{ return m_rep != 0; }

template <class R, class T1>
inline R
Hook1<R,T1>::operator() (T1 a) const
{ return m_rep->call (a); }

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
template <class R, class T1, class T2>
inline
Hook2<R,T1,T2>::Hook2 (Hook2Rep<R,T1,T2> *implementation /* = 0 */)
    : m_rep (implementation)
{ if (m_rep) m_rep->ref (); }

template <class R, class T1, class T2>
inline
Hook2<R,T1,T2>::Hook2 (const Hook2<R,T1,T2> &x)
    : m_rep (x.m_rep)
{ if (m_rep) m_rep->ref (); }

template <class R, class T1, class T2>
inline
Hook2<R,T1,T2>::~Hook2 (void)
{ if (m_rep) m_rep->unref (); }

template <class R, class T1, class T2>
inline Hook2<R,T1,T2> &
Hook2<R,T1,T2>::operator= (const Hook2<R,T1,T2> &x)
{
    if (m_rep != x.m_rep)
    {
	if (m_rep) m_rep->unref ();
	m_rep = x.m_rep;
	if (m_rep) m_rep->ref ();
    }
    return *this;
}

template <class R, class T1, class T2>
inline
Hook2<R,T1,T2>::operator bool (void) const
{ return m_rep != 0; }

template <class R, class T1, class T2>
inline R
Hook2<R,T1,T2>::operator() (T1 a, T2 b) const
{ return m_rep->call (a, b); }

} // namespace lat
#endif // CLASSLIB_HOOK_H
