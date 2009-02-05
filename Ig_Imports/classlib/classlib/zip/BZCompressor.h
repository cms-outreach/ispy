#ifndef CLASSLIB_BZ_COMPRESSOR_H
# define CLASSLIB_BZ_COMPRESSOR_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/zip/Compressor.h"
# include "classlib/zip/BZConstants.h"

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Compress data using the BZLIB algorithm.

    This class supports general purpose compression using the BZLIB
    library.  The BZLIB compression library is much like the popular
    ZLIB; it usually compresses better but slower than ZLIB due to a
    rather different algorithm (Burrows-Wheeler block sorting text
    compression algorithm).  The compression engine can use rather
    large amounts of memory; please refer to BZLIB documentation for
    an overview of the underlying library.

    @note Although the amount of work perfomed by this engine is
    related to how much input data and output space is available, it
    is not possible to predict or control how much work the engine
    will do in one call to compress().  BZLIB compressor usually
    consumes large amounts of input before producing any output
    (blocks of hundreds of kilobytes), compresses a block in a single
    call to compress(), and then begins to produce output.  It is
    therefore not compatible with strict timing requirements, such as
    real-time or highly interactive use.

    @sa BZLIB documentation at http://www.muraroa.demon.co.uk  */
class BZCompressor : public Compressor, public BZConstants
{
public:
    BZCompressor (unsigned level = DEFAULT_COMPRESSION,
		  unsigned workFactor = DEFAULT_WORK_FACTOR);
    ~BZCompressor (void);

    // Global compressor state
    virtual void	level (unsigned level);
    virtual unsigned	level (void) const;
    virtual void	workFactor (unsigned value);
    virtual unsigned	workFactor (void) const;
    virtual void	reset (void);
    virtual void	end (void);

    // Input management
    virtual bool	more (void) const;
    virtual void	input (const void *buffer, IOSize length);
    virtual void	finish (void);
    virtual void	flush (void);

    // Compression
    virtual bool	finished (void) const;
    virtual bool	flushed (void) const;
    virtual size_t	compress (void *into, IOSize length);

    // Statistics
    virtual unsigned	checksum (void) const;
    virtual IOOffset	in (void) const;
    virtual IOOffset	out (void) const;

private:
    struct Stream;
    Stream		*m_stream;	//< BZLIB state
    unsigned		m_level;	//< Compression blocking factor
    unsigned		m_workFactor;	//< Compression work factor
    unsigned		m_checksum;	//< CRC-32 checksum of input
    int			m_action;	//< Next BZLIB compression action
    int			m_state;	//< BZLIB state from last compression

    // undefined, no semantics
    BZCompressor (const BZCompressor &other);
    BZCompressor &operator= (const BZCompressor &other);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace lat
#endif // CLASSLIB_BZ_COMPRESSOR_H
