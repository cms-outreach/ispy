#ifndef CLASSLIB_CRC32_H
# define CLASSLIB_CRC32_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/zip/Checksum.h"

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Compute a CRC-32 checksum of a data stream.  */
class CRC32 : public Checksum
{
public:
    CRC32 (void);
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
#endif // CLASSLIB_CRC32_H
