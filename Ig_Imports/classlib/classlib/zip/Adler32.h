#ifndef CLASSLIB_ADLER32_H
# define CLASSLIB_ADLER32_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/zip/Checksum.h"

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Compute an Adler-32 checksum of a data stream.

    An Adler-32 checksum is almost as reliable as CRC-32 but much
    faster to compute.  */
class Adler32 : public Checksum
{
public:
    Adler32 (void);
    // implicit copy constructor
    // implicit destructor
    // implicit copy constructor

    virtual unsigned	value (void) const;
    virtual void	update (const void *data, IOSize n);
    virtual void	reset (void);

private:
    unsigned 		m_value;	//< Current checksum value
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace lat
#endif // CLASSLIB_ADLER32_H
