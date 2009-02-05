#ifndef CLASSLIB_ERROR_H
# define CLASSLIB_ERROR_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/sysapi/system.h"
# include <string>

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** An exception base class.

    This class provides a base for application run-time errors.  It
    supports designs important for robust exception handling: not
    losing information, abstracting the error as it propagates up in
    the application, and being able to re-explain the errors when
    dealing with them.

    The first two design issues are covered by allocating exceptions
    and chaining them together.  An error object is first constructed
    with constructor and thrown by value from deep in the call tree.
    As the object is caught and dealt with in the caller layers, they
    can build up a more abstract explanation of the error by creating
    another error object chained to a clone of the original one.
    Consider for an example an application failing to save a file
    because it runs out of disk space.  The I/O write routine will
    throw a system error object describing the out-of-space condition.
    The file save code will catch the error and throw a new ``save
    failure'' object chained to the I/O error object.  This continues
    up in the application until it gets to the level where the error
    can be finally dealt with and presented to the user.

    Once the exception is dealt with, it is necessary to find out what
    caused the problem.  This class supports two ways of doing so:
    getting a description as a message that can be presented to the
    user, or exploring the exception chain.

    Every error object can provide a description of the whole problem
    chain through the #explain() method.  The resulting string will
    provide an explanation that cascades through the whole chain of
    errors: "The file could not be saved <em>because</em> disk was
    full".

    The error objects can also be explored by the application.
    Typically the higher levels will catch errors by the base class
    reference("Error &").  To rediscover the source of the exception,
    the error object can be re-thrown and caught by its specific type
    to find its actual type (#rethrow()).  This allows the application
    to recovery without consulting the user.  For example, in our save
    scenario the application might be able to remove some dispensable
    temporary files and the retry saving before giving up and asking
    the user what to do.

    The basic motivation of this design is to avoid pushing user
    interface knowledge deep into the application.  Instead errors can
    be recovered where it make sense, and libraries without any
    knowledge of the user interface can be used as is.

    Note that this class does not inherit from the standard library's
    exception classes because of the somewhat incompatible design
    goals and because those classes have rather annoying exception
    specs on many of their methods.  This class does away with the
    specs but still requires them to be honoured: no error object
    method except #rethrow should ever throw.  */
class Error
{
public:
    Error (Error *chain = 0);
    Error (const Error &x);
    Error &operator= (const Error &x);
    virtual ~Error (void);

    Error *		next (void) const;

    virtual std::string	explain (int depth = -1) const;

    /** Explain this exception object.  */
    virtual std::string	explainSelf (void) const = 0;

    /** Make a deep copy of this exception chain.  */
    virtual Error *	clone (void) const = 0;

    /** Rethrow the error to discover its real dynamic type.  */
    virtual void	rethrow (void) = 0;

private:
    Error		*m_chain;	//< The next error in the chain
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

/** Get the next error in the chain. */
inline Error *
Error::next (void) const
{ return m_chain; }

} // namespace lat
#endif // CLASSLIB_ERROR_H
