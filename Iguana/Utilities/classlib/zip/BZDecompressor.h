#ifndef CLASSLIB_BZ_DECOMPRESSOR_H
# define CLASSLIB_BZ_DECOMPRESSOR_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/zip/Decompressor.h"
# include "classlib/zip/BZConstants.h"

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Decompress data using the BZLIB algorithm.

    This class supports general purpose decompression using the BZLIB
    library.  The BZLIB compression library is much like the popular
    ZLIB; it usually compresses better but slower than ZLIB due to a
    rather different algorithm (Burrows-Wheeler block sorting text
    compression algorithm).  The decompression engine can use rather
    large amounts of memory; please refer to BZLIB documentation for
    an overview of the underlying library.

    @note Although the amount of work perfomed by this engine is
    related to how much input data and output space is available, it
    is not possible to predict or control how much work the engine
    will do in one call to decompress().  The decompressor is
    therefore not compatible with strict timing requirements, such as
    real-time or highly interactive use.

    @sa BZLIB documentation at http://www.muraroa.demon.co.uk  */
class BZDecompressor : public Decompressor, public BZConstants
{
public:
    BZDecompressor (void);
    ~BZDecompressor (void);

    // Global compressor state
    virtual void	reset (void);
    virtual void	end (void);

    // Input management
    virtual bool	more (void) const;
    virtual void	input (const void *buffer, IOSize length);
    virtual IOSize	rest (void) const;

    // Decompression
    virtual bool	finished (void) const;
    virtual IOSize	decompress (void *into, IOSize length);

    // Statistics
    virtual unsigned	checksum (void) const;
    virtual IOOffset	in (void) const;
    virtual IOOffset	out (void) const;

private:
    struct Stream;
    Stream		*m_stream;	//< BZLIB state
    unsigned		m_checksum;	//< CRC-32 checksum of input
    int			m_state;	//< BZLIB state from last compression

    // undefined, no semantics
    BZDecompressor (const BZDecompressor &other);
    BZDecompressor &operator= (const BZDecompressor &other);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace lat
#endif // CLASSLIB_BZ_DECOMPRESSOR_H
