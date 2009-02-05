#ifndef CLASSLIB_SHARED_LIBRARY_H
# define CLASSLIB_SHARED_LIBRARY_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/utils/SharedLibraryError.h"
# include "classlib/utils/Callback.h"
# include <string>
# include <list>

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Shared library services.  */
class SharedLibrary
{
public:
    typedef void *		Data;
    typedef void		(*Function) (void);

    /** Information about a currently loaded shared library.  */
    struct LibraryInfo
    {
	unsigned long		m_text_start;	//< Start of text segment
	unsigned long		m_text_end;	//< End of text segment
	unsigned long		m_data_start;	//< Start of data segment
	unsigned long		m_data_end;	//< End of data segment
	unsigned long		m_bss_start;	//< Start of common
	unsigned long		m_bss_end;	//< End of common
	const char		*m_filename;	//< Filename
    };

    typedef Callback1<const LibraryInfo &> InfoHandler;

    static std::string		path (void);
    static void			path (const std::string &path);
    static std::string		libname (const std::string &name);
    static std::string		symname (const std::string &name);

    static SharedLibrary *	self (void);
    static SharedLibrary *	load (const std::string &name);
    static void			loaded (const InfoHandler &handler);

    void			release (void);
    void			abandon (void);

    Data			data (const std::string &name, bool mangle = true) const;
    Function			function (const std::string &name, bool mangle = true) const;

protected:
    SharedLibrary (void *handle);
    ~SharedLibrary (void);

private:
    void			*m_handle;

    // undefined semantics
    SharedLibrary (const SharedLibrary &);
    SharedLibrary &operator= (const SharedLibrary &);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace lat
#endif // CLASSLIB_SHARED_LIBRARY_H
