#ifndef CLASSLIB_DIGEST_H
# define CLASSLIB_DIGEST_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/sysapi/system.h"
# include "classlib/iobase/IOBuffer.h"
# include <vector>
# include <string>

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Calculate a checksum of a data stream.  */
class Digest
{
public:
    typedef std::vector<unsigned char> Value;

    virtual ~Digest (void);

    virtual std::string	format (void) const;
    virtual Value	digest (void) const = 0;
    virtual void	update (const void *data, IOSize n) = 0;
    void		update (unsigned char byte);
    void		update (IOBuffer data);
    virtual void	reset (void) = 0;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace lat
#endif // CLASSLIB_DIGEST_H
