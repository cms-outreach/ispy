#ifndef CLASSLIB_Z_DECOMPRESSOR_H
# define CLASSLIB_Z_DECOMPRESSOR_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/zip/Decompressor.h"
# include "classlib/zip/ZConstants.h"

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Decompress input data using the ZLIB algorithm.  */
class ZDecompressor : public Decompressor, public ZConstants
{
public:
    ZDecompressor (bool wrap = false);
    ~ZDecompressor (void);

    // Global decompressor state
    virtual void	reset (void);
    virtual void	end (void);

    // Input management
    virtual bool	more (void) const;
    virtual void	input (const void *buffer, IOSize length);
    virtual bool	needsDictionary (void) const;
    virtual void	dictionary (void *buffer, IOSize length);
    virtual IOSize	rest (void) const;

    // Decompression
    virtual bool	finished (void) const;
    virtual IOSize	decompress (void *into, IOSize length);
    virtual bool	sync (void);

    // Statistics
    virtual unsigned	checksum (void) const;
    virtual IOOffset	in (void) const;
    virtual IOOffset	out (void) const;

private:
    struct Stream;
    Stream		*m_stream;	//< ZLIB state, null if not initialised
    int			m_state;	//< Current decompression state.
    bool		m_wrap;		//< Flag for adding ZLIB stream wrapper

    // no semantics, undefined
    ZDecompressor (const ZDecompressor &other);
    ZDecompressor &operator= (const ZDecompressor &other);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace lat
#endif // CLASSLIB_Z_DECOMPRESSOR_H
