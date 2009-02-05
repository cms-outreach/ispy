#ifndef CLASSLIB_DECOMPRESSOR_H
# define CLASSLIB_DECOMPRESSOR_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/sysapi/system.h"
# include "classlib/iobase/IOBuffer.h"

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Decompress data.

    @example Client code should look like this
       Decompressor	*d = new BZDecompressor;
       InputStream	*input = new XXXInputStream (...);
       OutpuStream	*output = new YYYOutputStream (...);
       unsigned char	inbuffer [SIZE];
       unsigned char	outbuffer [SIZE];
       IOSize		in = 0, out = 0;
  
       while (! d->finished ())
       {
     	  // Give decompressor more input data as long as we have some.
 	  // This may read ahead beyond the end of the compressed data
 	  // stream by the maximum of SIZE-1 bytes; see below on how
 	  // to retrieve the unused input.
 	  if (d->more () && in = input->read (inbuffer, sizeof (inbuffer)))
     	      d->input (inbuffer, in);
  
 	  // Read out as much decompressed data as possible
 	  while (out = d->decompress (outbuffer, sizeof (outbuffer)))
 	      output->write (outbuffer, out);
       }
  
       // input unused: [unused, input + in)
       unsigned char *unused = input + d->rest ();
  
       d->end ();
       output->close ();  */
class Decompressor
{
public:
    virtual ~Decompressor (void);

    // Global compressor state
    virtual void	reset (void) = 0;
    virtual void	end (void) = 0;

    // Input management
    virtual bool	more (void) const = 0;
    virtual void	input (const void *buffer, IOSize length) = 0;
    virtual IOSize	rest (void) const = 0;

    // Compression
    virtual bool	finished (void) const = 0;
    virtual IOSize	decompress (void *into, IOSize length) = 0;

    // Statistics
    virtual unsigned	checksum (void) const = 0;
    virtual IOOffset	in (void) const = 0;
    virtual IOOffset	out (void) const = 0;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace lat
#endif // CLASSLIB_DECOMPRESSOR_H
