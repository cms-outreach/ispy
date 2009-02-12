#ifndef IGUANA_FRAMEWORK_IG_LIBRARY_PRELOADER_H
# define IGUANA_FRAMEWORK_IG_LIBRARY_PRELOADER_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Framework/interface/config.h"
# include "classlib/utils/SharedLibrary.h"
# include "classlib/utils/Error.h"
# include <iostream>
# include <stdlib.h>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IGUANA_FRAMEWORK_API IgLibraryPreloader
{
public:
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    IgLibraryPreloader (const char *app, bool verbose);
    void	operator() (const std::string &name);
    int		status (void) const;

private:
    const char	*m_app;
    bool	m_verbose;
    int		m_status;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

inline
IgLibraryPreloader::IgLibraryPreloader (const char *app, bool verbose)
    : m_app (app),
      m_verbose (verbose),
      m_status (EXIT_SUCCESS)
{}

inline void
IgLibraryPreloader::operator() (const std::string &name)
{
    try
    {
	if (m_verbose) std::cout << "preloading `" << name << "'.\n";
	lat::SharedLibrary::load (name);
    }
    catch (lat::Error &error)
    {
	std::cerr << m_app << ": cannot preload `" << name
		  << "':\n  " << error.explain () << "\n";
	m_status = EXIT_FAILURE;
    }
}

inline int
IgLibraryPreloader::status (void) const
{ return m_status; }

#endif // IGUANA_FRAMEWORK_IG_LIBRARY_PRELOADER_H
