#ifndef CLASSLIB_COMPRESSOR_H
# define CLASSLIB_COMPRESSOR_H

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

/** Compress data.

    @example Client code should look like this.
       Compressor	*c = new XXXCompressor (...);
       InputStream	*input = new YYYInputStream (...);
       OutpuStream	*output = new ZZZOutputStream (...);
       unsigned char	inbuffer [IN_BUF_SIZE];
       unsigned char	outbuffer [OUT_BUF_SIZE];
       IOSize		in = 0, out = 0;
  
       while (! c->finished ())
       {
     	  // Give compressor more input data or tell it to finish.
 	  if (c->more ())
     	      if (in = input->read (inbuffer, sizeof (inbuffer)))
     		  c->input (inbuffer, in);
     	      else
     		  c->finish ();
  
 	  // Read out as much compressed data as possible
 	  while (out = c->compress (outbuffer, sizeof (outbuffer)))
 	      output->write (outbuffer, out);
       }
  
       c->end ();
       output->close (); // or ->finish ();  */
class Compressor
{
public:
    virtual ~Compressor (void);

    // Global compressor state
    virtual void	reset (void) = 0;
    virtual void	end (void) = 0;

    // Input management
    virtual bool	more (void) const = 0;
    virtual void	input (const void *buffer, IOSize length) = 0;
    virtual void	finish (void) = 0;
    virtual void	flush (void) = 0;

    // Compression
    virtual bool	finished (void) const = 0;
    virtual bool	flushed (void) const = 0;
    virtual IOSize	compress (void *into, IOSize length) = 0;

    // Statistics
    virtual unsigned	checksum (void) const = 0;
    virtual IOOffset	in (void) const = 0;
    virtual IOOffset	out (void) const = 0;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace lat
#endif // CLASSLIB_COMPRESSOR_H
