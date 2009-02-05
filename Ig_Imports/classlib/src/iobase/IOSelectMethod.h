#ifndef CLASSLIB_IOSELECT_METHOD_H
# define CLASSLIB_IOSELECT_METHOD_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "IOSelectRequest.h"
# include "classlib/utils/DebugAids.h"
# include <vector>

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Abstract base class for #IOSelector implementation.  */
class IOSelectMethod
{
public:
    virtual ~IOSelectMethod (void);

    virtual void	setSignal (int sig);

    // Attaching and detaching descriptors
    virtual void	attach    (const IOSelectRequest &req) = 0;
    virtual void	detach    (const IOSelectRequest &req) = 0;
    virtual void	clear     (void) = 0;

    // Descriptor listening states
    virtual void	setMask   (IOFD fd, unsigned value) = 0;
    virtual void	andMask   (IOFD fd, unsigned value) = 0;
    virtual void	orMask    (IOFD fd, unsigned value) = 0;

    // Selection
    virtual bool	wait      (long msecs) = 0;
    virtual IOFD	next      (IOSelectEvent &event, IOSelectCB &cb) = 0;

    // Methods for IOSelector and actual implementations
    size_t		requests  (void);

protected:
    // Methods for actual implementations
    IOSelectRequest &	request   (size_t index);
    // size_t		findReq   (IOFD fd);
    size_t		addReq    (const IOSelectRequest &req);
    void		removeReq (const IOSelectRequest &req, size_t index);
    // void		removeReq (const IOSelectRequest &req);
    void		clearReqs (void);

private:
    std::vector<IOSelectRequest> m_requests; //< Current requests.
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

inline size_t
IOSelectMethod::requests (void)
{ return m_requests.size (); }

inline IOSelectRequest &
IOSelectMethod::request (size_t index)
{
    ASSERT (index < m_requests.size ());
    return m_requests [index];
}

} // namespace lat
#endif // CLASSLIB_IOSELECT_METHOD_H
