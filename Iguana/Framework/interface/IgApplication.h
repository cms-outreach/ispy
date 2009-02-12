#ifndef IGUANA_FRAMEWORK_IG_APPLICATION_H
# define IGUANA_FRAMEWORK_IG_APPLICATION_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Framework/interface/config.h"
# include "Iguana/Framework/interface/IgPluginManager.h"
# include <vector>
# include <string>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgState;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IGUANA_FRAMEWORK_API IgApplication
{
public:
    IgApplication (void);
    // implicit copy constructor
    // implicit assignment operator
    virtual ~IgApplication (void);

    virtual int			run (int argc, char *argv[]);

    virtual IgState *		state (void) const;
    virtual const char *	driver (void) const;
    virtual const char *	appname (void) const;
    virtual bool		verbose (void) const;
    virtual int			argc (void) const;
    virtual char **		argv (void) const;

protected:
    virtual int			usage (void);

    virtual void		startProfiler (void);
    virtual void		startMemStats (void);
    virtual int			initDebugging (const char *appname);

    virtual int			initDatabase (void);
    virtual int			dumpDatabase (void);

    virtual int			initState (void);
    virtual int			loadDriver (void);

private:
    void			pluginFeedback (IgPluginManager::FeedbackData data);

    // FIXME: make these into an app options service?
    bool			m_verbose;
    std::vector<std::string>	m_preload;
    IgState			*m_state;
    char			*m_driver;
    int				m_argc;
    char			**m_argv;
    char			*m_appname;
    bool			m_all;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_FRAMEWORK_IG_APPLICATION_H
