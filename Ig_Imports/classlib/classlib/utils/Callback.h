#ifndef CLASSLIB_CALLBACK_H
# define CLASSLIB_CALLBACK_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/sysapi/system.h"

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

/**/				class Callback;
template <class T1>		class Callback1;
template <class T1, class T2>	class Callback2;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class CallbackRep
{
public:
    CallbackRep (void) : m_refs (0) { }
    virtual ~CallbackRep (void) { }

    virtual void	call (void) = 0;
    virtual bool	equal (const CallbackRep *x) const = 0;

    void		ref (void)	{ ++m_refs; }
    void		unref (void)	{ if (--m_refs == 0) delete this; }

private:
    int			m_refs;
};

class Callback
{
public:
    Callback (CallbackRep *implementation = 0);
    Callback (const Callback &x);
    ~Callback (void);
    Callback &	operator= (const Callback &x);

    bool		operator== (const Callback &x) const;
    /**/		operator bool (void) const;
    void		operator() (void) const;

private:
    CallbackRep	*m_rep;
};

//////////////////////////////////////////////////////////////////////
template <class T1>
class Callback1Rep
{
public:
    Callback1Rep (void) : m_refs (0) { }
    virtual ~Callback1Rep (void) { }

    virtual void	call (T1) = 0;
    virtual bool	equal (const Callback1Rep *x) const = 0;

    void		ref (void)	{ ++m_refs; }
    void		unref (void)	{ if (--m_refs == 0) delete this; }

private:
    int			m_refs;
};

template <class T1>
class Callback1
{
public:
    Callback1 (Callback1Rep<T1> *implementation = 0);
    Callback1 (const Callback1 &x);
    ~Callback1 (void);
    Callback1 &	operator= (const Callback1 &x);

    bool		operator== (const Callback1 &x) const;
    /**/		operator bool (void) const;
    void		operator() (T1) const;

private:
    Callback1Rep<T1>	*m_rep;
};

//////////////////////////////////////////////////////////////////////
template <class T1, class T2>
class Callback2Rep
{
public:
    Callback2Rep (void) : m_refs (0) { }
    virtual ~Callback2Rep (void) { }

    virtual void	call (T1, T2) = 0;
    virtual bool	equal (const Callback2Rep *x) const = 0;

    void		ref (void)	{ ++m_refs; }
    void		unref (void)	{ if (--m_refs == 0) delete this; }

private:
    int			m_refs;
};

template <class T1, class T2>
class Callback2
{
public:
    Callback2 (Callback2Rep<T1,T2> *implementation = 0);
    Callback2 (const Callback2 &x);
    ~Callback2 (void);
    Callback2 &	operator= (const Callback2 &x);

    bool		operator== (const Callback2 &x) const;
    /**/		operator bool (void) const;
    void		operator() (T1, T2) const;

private:
    Callback2Rep<T1,T2> *m_rep;
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

class CallbackImpF00 : public CallbackRep
{
    typedef CallbackImpF00 self;
public:
    CallbackImpF00 (void (*function) (void))
	: m_function (function)
	{ }

    virtual void call (void)
	{ (*m_function) (); }

    virtual bool equal (const CallbackRep *other) const
	{ const self *x = dynamic_cast<const self *> (other);
	  return x && x->m_function == m_function; }

private:
    void	(*m_function) (void);
};

/////////////////////////////////////////////////////////////////////////////

template <class C>
class CallbackImpC00 : public CallbackRep
{
    typedef CallbackImpC00 self;
public:
    CallbackImpC00 (C *object, void (C::*function) (void))
	: m_object (object),
	  m_function (function)
	{ }

    virtual void call (void)
	{ (m_object->*m_function) (); }

    virtual bool equal (const CallbackRep *other) const
	{ const self *x = dynamic_cast<const self *> (other);
	  return x && x->m_object == m_object && x->m_function == m_function; }

private:
    C		*m_object;
    void	(C::*m_function) (void);
};

/////////////////////////////////////////////////////////////////////////////

template <class T1>
class CallbackImpF01 : public CallbackRep
{
    typedef CallbackImpF01 self;
public:
    CallbackImpF01 (void (*function) (T1),
		    const T1 &fill_1)
	: m_function (function),
	  m_fill_1 (fill_1)
	{ }

    virtual void call (void)
	{ (*m_function) (m_fill_1); }

    virtual bool equal (const CallbackRep *other) const
	{ const self *x = dynamic_cast<const self *> (other);
	  return x && x->m_function == m_function && x->m_fill_1 == m_fill_1; }

private:
    void	(*m_function) (T1);
    T1		m_fill_1;
};

/////////////////////////////////////////////////////////////////////////////

template <class C, class T1>
class CallbackImpC01 : public CallbackRep
{
    typedef CallbackImpC01 self;
public:
    CallbackImpC01 (C *object, void (C::*function) (T1),
		    const T1 &fill_1)
	: m_object (object),
	  m_function (function),
	  m_fill_1 (fill_1)
	{ }

    virtual void call (void)
	{ (m_object->*m_function) (m_fill_1); }

    virtual bool equal (const CallbackRep *other) const
	{ const self *x = dynamic_cast<const self *> (other);
	  return x && x->m_object == m_object && x->m_function == m_function
		   && x->m_fill_1 == m_fill_1; }

private:
    C		*m_object;
    void	(C::*m_function) (T1);
    T1		m_fill_1;
};

/////////////////////////////////////////////////////////////////////////////

template <class T1, class T2>
class CallbackImpF02 : public CallbackRep
{
    typedef CallbackImpF02 self;
public:
    CallbackImpF02 (void (*function) (T1, T2),
		    const T1 &fill_1, const T2 &fill_2)
	: m_function (function),
	  m_fill_1 (fill_1),
	  m_fill_2 (fill_2)
    { }

    virtual void call (void)
    { (*m_function) (m_fill_1, m_fill_2); }

    virtual bool equal (const CallbackRep *other) const
	{ const self *x = dynamic_cast<const self *> (other);
	  return x && x->m_function == m_function && x->m_fill_1 == m_fill_1
		   && x->m_fill_2 == m_fill_2; }

private:
    void	(*m_function) (T1, T2);
    T1		m_fill_1;
    T2		m_fill_2;
};

/////////////////////////////////////////////////////////////////////////////
template <class C, class T1, class T2>
class CallbackImpC02 : public CallbackRep
{
    typedef CallbackImpC02 self;
public:
    CallbackImpC02 (C *object, void (C::*function) (T1, T2),
		    const T1 &fill_1, const T2 &fill_2)
	: m_object (object),
	  m_function (function),
	  m_fill_1 (fill_1),
	  m_fill_2 (fill_2)
    { }

    virtual void call (void)
    { (m_object->*m_function) (m_fill_1, m_fill_2); }

    virtual bool equal (const CallbackRep *other) const
	{ const self *x = dynamic_cast<const self *> (other);
	  return x && x->m_object == m_object && x->m_function == m_function
		   && x->m_fill_1 == m_fill_1 && x->m_fill_2 == m_fill_2; }

private:
    C		*m_object;
    void	(C::*m_function) (T1, T2);
    T1		m_fill_1;
    T2		m_fill_2;
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
template <class T1>
class CallbackImpF10 : public Callback1Rep<T1>
{
    typedef CallbackImpF10 self;
public:
    CallbackImpF10 (void (*function) (T1))
	: m_function (function)
    { }

    virtual void call (T1 a)
    { (*m_function) (a); }

    virtual bool equal (const Callback1Rep<T1> *other) const
	{ const self *x = dynamic_cast<const self *> (other);
	  return x && x->m_function == m_function; }

private:
    void	(*m_function) (T1);
};

/////////////////////////////////////////////////////////////////////////////
template <class C, class T1>
class CallbackImpC10 : public Callback1Rep<T1>
{
    typedef CallbackImpC10 self;
public:
    CallbackImpC10 (C *object, void (C::*function) (T1))
	: m_object (object),
	  m_function (function)
    { }

    virtual void call (T1 a)
    { (m_object->*m_function) (a); }

    virtual bool equal (const Callback1Rep<T1> *other) const
	{ const self *x = dynamic_cast<const self *> (other);
	  return x && x->m_object == m_object && x->m_function == m_function; }

private:
    C		*m_object;
    void	(C::*m_function) (T1);
};


/////////////////////////////////////////////////////////////////////////////
template <class T1, class T2>
class CallbackImpF11 : public Callback1Rep<T1>
{
    typedef CallbackImpF11 self;
public:
    CallbackImpF11 (void (*function) (T1, T2),
		    const T2 &fill_2)
	: m_function (function),
	  m_fill_2 (fill_2)
    { }

    virtual void call (T1 a)
    { (*m_function) (a, m_fill_2); }

    virtual bool equal (const Callback1Rep<T1> *other) const
	{ const self *x = dynamic_cast<const self *> (other);
	  return x && x->m_function == m_function && x->m_fill_2 == m_fill_2; }

private:
    void	(*m_function) (T1, T2);
    T2		m_fill_2;
};

/////////////////////////////////////////////////////////////////////////////
template <class C, class T1, class T2>
class CallbackImpC11 : public Callback1Rep<T1>
{
    typedef CallbackImpC11 self;
public:
    CallbackImpC11 (C *object, void (C::*function) (T1, T2),
		    const T2 &fill_2)
	: m_object (object),
	  m_function (function),
	  m_fill_2 (fill_2)
    { }

    virtual void call (T1 a)
    { (m_object->*m_function) (a, m_fill_2); }

    virtual bool equal (const Callback1Rep<T1> *other) const
	{ const self *x = dynamic_cast<const self *> (other);
	  return x && x->m_object == m_object && x->m_function == m_function
		   && x->m_fill_2 == m_fill_2; }

private:
    C		*m_object;
    void	(C::*m_function) (T1, T2);
    T2		m_fill_2;
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
template <class T1, class T2, class T3>
class CallbackImpF12 : public Callback1Rep<T1>
{
    typedef CallbackImpF12 self;
public:
    CallbackImpF12 (void (*function) (T1, T2, T3), const T2 &fill_2,
		    const T3 &fill_3)
	: m_function (function),
	  m_fill_2 (fill_2),
	  m_fill_3 (fill_3)
    { }

    virtual void call (T1 a)
    { (*m_function) (a, m_fill_2, m_fill_3); }

    virtual bool equal (const Callback1Rep<T1> *other) const
	{ const self *x = dynamic_cast<const self *> (other);
	  return x && x->m_function == m_function
		   && x->m_fill_2 == m_fill_2 && x->m_fill_3 == m_fill_3; }

private:
    void	(*m_function) (T1, T2, T3);
    T2		m_fill_2;
    T3		m_fill_3;
};

/////////////////////////////////////////////////////////////////////////////
template <class C, class T1, class T2, class T3>
class CallbackImpC12 : public Callback1Rep<T1>
{
    typedef CallbackImpC12 self;
public:
    CallbackImpC12 (C *object, void (C::*function) (T1, T2, T3),
		    const T2 &fill_2, const T3 &fill_3)
	: m_object (object),
	  m_function (function),
	  m_fill_2 (fill_2),
	  m_fill_3 (fill_3)
    { }

    virtual void call (T1 a)
    { (m_object->*m_function) (a, m_fill_2, m_fill_3); }

    virtual bool equal (const Callback1Rep<T1> *other) const
	{ const self *x = dynamic_cast<const self *> (other);
	  return x && x->m_object == m_object && x->m_function == m_function
		   && x->m_fill_2 == m_fill_2 && x->m_fill_3 == m_fill_3; }

private:
    C		*m_object;
    void	(C::*m_function) (T1, T2, T3);
    T2		m_fill_2;
    T3		m_fill_3;
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
template <class T1, class T2>
class CallbackImpF20 : public Callback2Rep<T1,T2>
{
    typedef CallbackImpF20 self;
public:
    CallbackImpF20 (void (*function) (T1, T2))
	: m_function (function)
    { }

    virtual void call (T1 a, T2 b)
    { (*m_function) (a, b); }

    virtual bool equal (const Callback2Rep<T1,T2> *other) const
	{ const self *x = dynamic_cast<const self *> (other);
	  return x && x->m_function == m_function; }

private:
    void	(*m_function) (T1, T2);
};

/////////////////////////////////////////////////////////////////////////////
template <class C, class T1, class T2>
class CallbackImpC20 : public Callback2Rep<T1,T2>
{
    typedef CallbackImpC20 self;
public:
    CallbackImpC20 (C *object, void (C::*function) (T1,T2))
	: m_object (object),
	  m_function (function)
    { }

    virtual void call (T1 a, T2 b)
    { (m_object->*m_function) (a, b); }

    virtual bool equal (const Callback2Rep<T1,T2> *other) const
	{ const self *x = dynamic_cast<const self *> (other);
	  return x && x->m_object == m_object && x->m_function == m_function; }


private:
    C		*m_object;
    void	(C::*m_function) (T1, T2);
};

/////////////////////////////////////////////////////////////////////////////
template <class T1, class T2, class T3>
class CallbackImpF21 : public Callback2Rep<T1,T2>
{
    typedef CallbackImpF21 self;
public:
    CallbackImpF21 (void (*function) (T1, T2, T3),
		    const T3 &fill_3)
	: m_function (function),
	  m_fill_3 (fill_3)
    { }

    virtual void call (T1 a, T2 b)
    { (*m_function) (a, b, m_fill_3); }

    virtual bool equal (const Callback2Rep<T1,T2> *other) const
	{ const self *x = dynamic_cast<const self *> (other);
	  return x && x->m_function == m_function && x->m_fill_3 == m_fill_3; }

private:
    void	(*m_function) (T1, T2, T3);
    T3		m_fill_3;
};

/////////////////////////////////////////////////////////////////////////////
template <class C, class T1, class T2, class T3>
class CallbackImpC21 : public Callback2Rep<T1,T2>
{
    typedef CallbackImpC21 self;
public:
    CallbackImpC21 (C *object, void (C::*function) (T1,T2, T3),
		    const T3 &fill_3)
	: m_object (object),
	  m_function (function),
	  m_fill_3 (fill_3)
    { }

    virtual void call (T1 a, T2 b)
    { (m_object->*m_function) (a, b, m_fill_3); }

    virtual bool equal (const Callback2Rep<T1,T2> *other) const
	{ const self *x = dynamic_cast<const self *> (other);
	  return x && x->m_object == m_object && x->m_function == m_function
		   && x->m_fill_3 == m_fill_3; }

private:
    C		*m_object;
    void	(C::*m_function) (T1, T2, T3);
    T3		m_fill_3;
};

/////////////////////////////////////////////////////////////////////////////
template <class T1, class T2, class T3, class T4>
class CallbackImpF22 : public Callback2Rep<T1,T2>
{
    typedef CallbackImpF22 self;
public:
    CallbackImpF22 (void (*function) (T1, T2, T3, T4),
		    const T3 &fill_3, const T4 &fill_4)
	: m_function (function),
	  m_fill_3 (fill_3),
	  m_fill_4 (fill_4)
    { }

    virtual void call (T1 a, T2 b)
    { (*m_function) (a, b, m_fill_3, m_fill_4); }

    virtual bool equal (const Callback2Rep<T1,T2> *other) const
	{ const self *x = dynamic_cast<const self *> (other);
	  return x && x->m_function == m_function
		   && x->m_fill_3 == m_fill_3 && x->m_fill_4 == m_fill_4; }

private:
    void	(*m_function) (T1, T2, T3, T4);
    T3		m_fill_3;
    T4		m_fill_4;
};

/////////////////////////////////////////////////////////////////////////////
template <class C, class T1, class T2, class T3, class T4>
class CallbackImpC22 : public Callback2Rep<T1,T2>
{
    typedef CallbackImpC22 self;
public:
    CallbackImpC22 (C *object, void (C::*function) (T1,T2, T3, T4),
		    const T3 &fill_3, const T4 &fill_4)
	: m_object (object),
	  m_function (function),
	  m_fill_3 (fill_3),
	  m_fill_4 (fill_4)
    { }

    virtual void call (T1 a, T2 b)
    { (m_object->*m_function) (a, b, m_fill_3, m_fill_4); }

    virtual bool equal (const Callback2Rep<T1,T2> *other) const
	{ const self *x = dynamic_cast<const self *> (other);
	  return x && x->m_object == m_object && x->m_function == m_function
		   && x->m_fill_3 == m_fill_3 && x->m_fill_4 == m_fill_4; }

private:
    C		*m_object;
    void	(C::*m_function) (T1, T2, T3, T4);
    T3		m_fill_3;
    T4		m_fill_4;
};


//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>

inline CallbackRep *
CreateCallback (void (*function) (void))
{
    return new CallbackImpF00 (function);
}

template <class C>
inline CallbackRep *
CreateCallback (C *object, void (C::*function) (void))
{
    return new CallbackImpC00<C> (object, function);
}

template <class T1>
inline CallbackRep *
CreateCallback (void (*function) (T1),
		const T1 &fill_1)
{
    return new CallbackImpF01<T1> (function, fill_1);
}

template <class C, class T1>
inline CallbackRep *
CreateCallback (C *object, void (C::*function) (T1),
		const T1 &fill_1)
{
    return new CallbackImpC01<C,T1> (object, function, fill_1);
}

template <class T1, class T2>
inline CallbackRep *
CreateCallback (void (*function) (T1, T2),
		const T1 &fill_1, const T2 &fill_2)
{ return new CallbackImpF02<T1,T2> (function, fill_1, fill_2); }

template <class C, class T1, class T2>
inline CallbackRep *
CreateCallback (C *object, void (C::*function) (T1, T2),
		const T1 &fill_1, const T2 &fill_2)
{ return new CallbackImpC02<C,T1,T2> (object, function, fill_1, fill_2); }

template <class T1>
inline Callback1Rep<T1> *
CreateCallback (void (*function) (T1))
{ return new CallbackImpF10<T1> (function); }

template <class C, class T1>
inline Callback1Rep<T1> *
CreateCallback (C *object, void (C::*function) (T1))
{ return new CallbackImpC10<C,T1> (object, function); }

template <class T1, class T2>
inline Callback1Rep<T1> *
CreateCallback (void (*function) (T1, T2),
		const T2 &fill_2)
{ return new CallbackImpF11<T1,T2> (function, fill_2); }

template <class C, class T1, class T2>
inline Callback1Rep<T1> *
CreateCallback (C *object, void (C::*function) (T1, T2),
		const T2 &fill_2)
{ return new CallbackImpC11<C,T1,T2> (object, function, fill_2); }

template <class T1, class T2, class T3>
inline Callback1Rep<T1> *
CreateCallback (void (*function) (T1, T2, T3),
		const T2 &fill_2, const T3 &fill_3)
{ return new CallbackImpF12<T1,T2,T3> (function, fill_2, fill_3); }

template <class C, class T1, class T2, class T3>
inline Callback1Rep<T1> *
CreateCallback (C *object, void (C::*function) (T1, T2),
		const T2 &fill_2, const T3 &fill_3)
{ return new CallbackImpC12<C,T1,T2,T3> (object, function, fill_2, fill_3); }

template <class T1, class T2>
inline Callback2Rep<T1,T2> *
CreateCallback (void (*function) (T1, T2))
{ return new CallbackImpF20<T1,T2> (function); }

template <class C, class T1, class T2>
inline Callback2Rep<T1,T2> *
CreateCallback (C *object, void (C::*function) (T1, T2))
{ return new CallbackImpC20<C,T1,T2> (object, function); }

template <class T1, class T2, class T3>
inline Callback2Rep<T1,T2> *
CreateCallback (void (*function) (T1, T2, T3),
		const T3 &fill_3)
{ return new CallbackImpF21<T1,T2,T3> (function, fill_3); }

template <class C, class T1, class T2, class T3>
inline Callback2Rep<T1,T2> *
CreateCallback (C *object, void (C::*function) (T1, T2, T3),
		const T3 &fill_3)
{ return new CallbackImpC21<C,T1,T2,T3> (object, function, fill_3); }

template <class T1, class T2, class T3, class T4>
inline Callback2Rep<T1,T2> *
CreateCallback (void (*function) (T1, T2, T3, T4),
		const T3 &fill_3, const T4 &fill_4)
{ return new CallbackImpF22<T1,T2,T3,T4> (function, fill_3, fill_4); }

template <class C, class T1, class T2, class T3, class T4>
inline Callback2Rep<T1,T2> *
CreateCallback (C *object, void (C::*function) (T1, T2, T3, T4),
		const T3 &fill_3, const T4 &fill_4)
{ return new CallbackImpC22<C,T1,T2,T3,T4> (object, function, fill_3, fill_4); }

//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

inline
Callback::Callback (CallbackRep *implementation /* = 0 */)
    : m_rep (implementation)
{ if (m_rep) m_rep->ref (); }

inline
Callback::Callback (const Callback &x)
    : m_rep (x.m_rep)
{ if (m_rep) m_rep->ref (); }

inline
Callback::~Callback (void)
{ if (m_rep) m_rep->unref (); }

inline Callback &
Callback::operator= (const Callback &x)
{
    if (m_rep != x.m_rep)
    {
	if (m_rep) m_rep->unref ();
	m_rep = x.m_rep;
	if (m_rep) m_rep->ref ();
    }
    return *this;
}

inline bool
Callback::operator== (const Callback &x) const
{ return m_rep == x.m_rep || (m_rep && x.m_rep && m_rep->equal (x.m_rep)); }

inline
Callback::operator bool (void) const
{ return m_rep ? true : false; } // FIXME: for Sun CC 4.2 (no bool)

inline void
Callback::operator() (void) const
{ m_rep->call (); }

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

template <class T1>
inline
Callback1<T1>::Callback1 (Callback1Rep<T1> *implementation /* = 0 */)
    : m_rep (implementation)
{ if (m_rep) m_rep->ref (); }

template <class T1>
inline
Callback1<T1>::Callback1 (const Callback1<T1> &x)
    : m_rep (x.m_rep)
{ if (m_rep) m_rep->ref (); }

template <class T1>
inline
Callback1<T1>::~Callback1 (void)
{ if (m_rep) m_rep->unref (); }

template <class T1>
inline Callback1<T1> &
Callback1<T1>::operator= (const Callback1<T1> &x)
{
    if (m_rep != x.m_rep)
    {
	if (m_rep) m_rep->unref ();
	m_rep = x.m_rep;
	if (m_rep) m_rep->ref ();
    }
    return *this;
}

template <class T1>
inline bool
Callback1<T1>::operator== (const Callback1<T1> &x) const
{ return m_rep == x.m_rep || (m_rep && x.m_rep && m_rep->equal (x.m_rep)); }

template <class T1>
inline
Callback1<T1>::operator bool (void) const
{ return m_rep ? true : false; } // FIXME: for Sun CC 4.2 (no bool)

template <class T1>
inline void
Callback1<T1>::operator() (T1 a) const
{ m_rep->call (a); }

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

template <class T1, class T2>
inline
Callback2<T1,T2>::Callback2 (Callback2Rep<T1,T2> *implementation /* = 0 */)
    : m_rep (implementation)
{ if (m_rep) m_rep->ref (); }

template <class T1, class T2>
inline
Callback2<T1,T2>::Callback2 (const Callback2<T1,T2> &x)
    : m_rep (x.m_rep)
{ if (m_rep) m_rep->ref (); }

template <class T1, class T2>
inline
Callback2<T1,T2>::~Callback2 (void)
{ if (m_rep) m_rep->unref (); }

template <class T1, class T2>
inline Callback2<T1,T2> &
Callback2<T1,T2>::operator= (const Callback2<T1,T2> &x)
{
    if (m_rep != x.m_rep)
    {
	if (m_rep) m_rep->unref ();
	m_rep = x.m_rep;
	if (m_rep) m_rep->ref ();
    }
    return *this;
}

template <class T1, class T2>
inline bool
Callback2<T1,T2>::operator== (const Callback2<T1,T2> &x) const
{ return m_rep == x.m_rep || (m_rep && x.m_rep && m_rep->equal (x.m_rep)); }

template <class T1, class T2>
inline
Callback2<T1,T2>::operator bool (void) const
{ return m_rep ? true : false; } // FIXME: for Sun CC 4.2 (no bool)

template <class T1, class T2>
inline void
Callback2<T1,T2>::operator() (T1 a, T2 b) const
{ m_rep->call (a, b); }

} // namespace lat
#endif // CLASSLIB_CALLBACK_H
