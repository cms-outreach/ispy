#ifndef CLASSLIB_IOSELECT_WINDOWS_H
# define CLASSLIB_IOSELECT_WINDOWS_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "IOSelectMethod.h"
# include <list>
# ifdef _WIN32
#  include <windows.h>

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Implementation of #IOSelectMethod for Windows.  */
class IOSelectWindows : public IOSelectMethod
{
public:
    class Poll
    {
    public:
	virtual ~Poll (void);

	virtual void	start (IOSelectWindows *in);
	virtual void	update (IOSelectWindows *in);
	virtual void	close (IOSelectWindows *in);
    };

    class Poller
    {
    public:
	virtual ~Poller (void);

	virtual HANDLE	handle (void) const;

	virtual void	initialise (void);
	virtual void	run (void);
	virtual void	stop (void);
	virtual void	finalise (void);

    private:
	HANDLE		m_self;
    };

    IOSelectWindows (void);
    ~IOSelectWindows (void);

    // Attaching and detaching channels
    virtual void	attach    (const IOSelectRequest &req);
    virtual void	detach    (const IOSelectRequest &req);
    virtual void	clear     (void);

    // Channel status access
    virtual void	setMask   (IOFD fd, unsigned value);
    virtual void	andMask   (IOFD fd, unsigned value);
    virtual void	orMask    (IOFD fd, unsigned value);

    // Selection
    virtual bool	wait      (long msecs);
    virtual IOFD	next      (IOSelectEvent &event, IOSelectCB &cb);

private:
    size_t		findReq   (IOFD fd);

    // FIXME: additional records chained to this?
    std::vector<Poll *>	m_polls;
    std::vector<Poller *> m_pollers;
    size_t		m_fds;
    HANDLE		m_w4 [MAXIMUM_WAIT_OBJECTS];
    DWORD		m_sig;
    int			m_nready;
    int			m_current;
    bool		m_msgq;
    DWORD		m_ms;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace lat
# endif // _WIN32
#endif // CLASSLIB_IOSELECT_WINDOWS_H
