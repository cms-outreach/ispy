#ifndef CLASSLIB_DEBUG_H
# define CLASSLIB_DEBUG_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/utils/Macros.h"
# include "classlib/sysapi/IOTypes.h"
# include <cstddef>

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>

# ifdef CLASSLIB_FUNCTION
// Define for the function name in <<ASSERT>>.
#  define ASSERT_FUNC CLASSLIB_FUNCTION
# else
#  define ASSERT_FUNC 0
# endif

# undef ASSERT_ALWAYS
# undef ASSERT
# undef VERIFY
# undef ASSERT_VALID

/** @def NDEBUG
    @brief Flag to control debugging code, in particular assertions:
    defining it switches off all the debugging code.  */

/** @def ASSERT
    @brief An assertion expression.

    Check the argument expression.  If it evaluates to @c false, run
    #DebugAids::failed() with information about the expression and the
    source code location.  The expression should no side effects as it
    will not be evaluated if #NDEBUG is defined.  Use #VERIFY if the
    expression should be evaluated in non-debugging code as well.
    The expression is evaluated only once.

    #ASSERT can be used where ever an expression is allowed.  It does
    not evaluate to any useful value.  The expression is evaluated
    only if #NDEBUG is undefined; with #NDEBUG the macro expands to an
    empty void expression.  */

/** @def ASSERT_ALWAYS
    @brief Like #ASSERT but verified always regardless of #NDEBUG.

    If #NDEBUG is defined, the expression will not be stringified, and
    hence will not show up in the assertion failure message.  However
    the file name, line number and the name of the enclosing function
    will still be present.  */

/** @def ASSERT_VALID
    @brief An assertion that verifies that the argument is a pointer
           to a valid object.

    This macro checks that the argument expression results in a
    non-null pointer to an object and if so, invokes a type-specific
    #AssertValid() validator function.  If the the pointer check
    fails, invokes #DebugAids::failed().  Only evaluates the argument
    once.

    A common usage of this macro is to say <tt>ASSERT_VALID(this)</tt>
    in the beginning of every method in the class' public interface.
    This can catch problems quickly if something trashes the object's
    data members: the first use of the object thereafter will produce
    an assertion failure.  If you do follow this convention, be
    careful how you use the class' public methods in its internal
    implementation.  It is not a good idea to call a public method in
    the implementation of another method if the former uses
    #ASSERT_VALID and the latter has left the internal state
    inconsitent (presumably because it is not done yet).

    @sa #Debuggable for a base class to translate the validation call
    into a virtual function call.  */

/** @def VERIFY
    @brief Like #ASSERT but evaluates the argument regardless of #NDEBUG

    This macro is unlike #ASSERT_ALWAYS in that if #NDEBUG is defined
    and the expression evaluates to @c false, #DebugAids::failed()
    will not be invoked.  */

# if !NDEBUG
#  define ASSERT_ALWAYS(f)	\
		((f)?(void)0: 	\
		 lat::DebugAids::failed (__FILE__,__LINE__,ASSERT_FUNC,#f))
#  define ASSERT(f)		\
		((f)?(void)0:	\
		 lat::DebugAids::failed (__FILE__,__LINE__,ASSERT_FUNC,#f))
#  define VERIFY(f)		\
		((f)?(void)0:	\
		 lat::DebugAids::failed (__FILE__,__LINE__,ASSERT_FUNC,#f))
#  define ASSERT_VALID(p)	\
		(lat::ASSERT_VALIDATOR((p),__FILE__,__LINE__,ASSERT_FUNC,#p))

# else // NDEBUG
#  define ASSERT_ALWAYS(f)	\
		((f)?(void)0:	\
		 lat::DebugAids::failed (__FILE__,__LINE__,ASSERT_FUNC,0))
#  define ASSERT(f)		((void)0)
#  define VERIFY(f)		((void)(f))
#  define ASSERT_VALID(p)	((void)0)
# endif // !NDEBUG

//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class logstream;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Utility base class to translate debugging events into
    object-specific virtual function calls.  */
class Debuggable
{
public:
    virtual ~Debuggable (void);

    virtual void	validate (void) const;
    virtual void	log (logstream &to) const;
};

/** Utilities for debugging support.  */
class DebugAids
{
public:
    /** Application hook to deal with assertion failures.
    
        @a message is the formatted assertion failure message that
	should be displayed to the user; it should not be modified or
	freed by the hook.  The return value should be one of the
	following characters: `a' to abort, `r' to run #breakpoint()
	and then retry (this may cause a fall into a debugger on some
	platforms), `i' to ignore the failure, `c' to dump a core and
	then retry, `s' to dump stack trace and then retry (FIXME: `t'
	to throw/longjmp?).  Here "retry" means that the assertion
	failure will be processed again, invoking the hook once
	again. */
    typedef char		(*AssertHook) (const char *message);

    // Memory validation routines
    static bool			validate (const char *str, int length = -1);
    static bool			validate (const void *p,
					  STDC::size_t bytes,
					  bool rw);

    // Assertion failure support
    static void			failed (const char *file, int line,
					const char *function,
					const char *expr);
    static int			failIgnore (int count = -1);
    static AssertHook		failHook (AssertHook hook = 0);
    static IOFD			failStdxxFd (IOFD fd = IOFD_INVALID);
    static char			failStdxx (const char *message);
# ifdef _WIN32
    static char			failMsgBox (const char *message);
# endif

    // Miscellaneous functions
    static void			breakpoint (void);
    static IOFD			stacktraceFd (IOFD fd = IOFD_INVALID);
    static void			stacktrace (IOFD fd = IOFD_INVALID);
    static void			coredump (int sig, ...);


private:
    static AssertHook		s_assertHook;
    static int			s_assertIgnoreCount;
    static IOFD			s_assertOutputFd;
    static IOFD			s_stackTraceFd;
};

//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>

/** Type-specific object validators.

    Add to this overloaded function family your class-specific object
    validators.  Note that the declarations of your overloads must be
    visible when #ASSERT_VALID is those objects, otherwise the generic
    validator (for "void *" address) will be called.  Typically one
    declares a friend function directly in the class declaration,
    making the validator visible if the type itself is complete.

    The specialised validator gets a pointer that is guaranteed to be
    non-null.  A typical validator function consists of #ASSERT calls.
    For class hierarchies it is beneficial to define only one function
    for the base class and make it call a virtual function.  */
void		AssertValid (const void *address);
void		AssertValid (const Debuggable *object);

/** A helper to convert a logging output of an object to a virtual
    function call to @a object.  @a to is the stream to output to.
    Returns the stream after output.  */
logstream &	operator<< (logstream &to, const Debuggable &object);

/** Helper template for #ASSERT_VALID for invoking type-specific
    #AssertValid functions and to protect the #ASSERT_VALID macro
    against multiple evaluation of @a object.  Checks that @a object
    is non-null and a valid memory address at least for the size of
    the @a T.  If these checks pass, the type-specific #AssertValid
    function is invoked.  If not, #DebugAids::failed() is invoked with
    the assertion context information (@a file, @a line, @a function
    and @a expr).

    Do not specialise this template, extend #AssertValid instead.  Do
    not call this function directly, use #ASSERT_VALID instead.  */
template <class T>
inline void
ASSERT_VALIDATOR (const T *object, const char *file, int line,
		  const char *function, const char *expr)
{
    if (object && DebugAids::validate (object, sizeof (*object), false))
	lat::AssertValid (object);
    else
	DebugAids::failed (file, line, function, expr);
}

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace lat
#endif // CLASSLIB_DEBUG_H
