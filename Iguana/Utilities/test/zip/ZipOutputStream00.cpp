#include "test/Test.h" 
#include "classlib/zip/ZipOutputStream.h"
#include "classlib/zip/ZipInputStream.h"
#include "classlib/zip/ZipMember.h"
#include "classlib/zip/ZConstants.h"
#include "classlib/iobase/File.h"
#include "classlib/iobase/Filename.h"
#include "classlib/utils/ShellEnvironment.h"
#include "classlib/iobase/DirIterator.h"
#include "classlib/iotools/StorageOutputStream.h"
#include "classlib/iotools/StorageInputStream.h"
#include "classlib/iotools/BufferOutputStream.h"
#include "classlib/iotools/BufferInputStream.h"
#include "classlib/utils/Signal.h"
#include <iostream>
#include <iomanip>





using namespace lat;

int TEST(int, char **argv)
{
  
  Filename inputDir( Filename("$SEAL/src/config/cmt").substitute (ShellEnvironment ()));
  Filename zipOutputFile( Filename("test00.zip").substitute (ShellEnvironment ()));
  
  Signal::handleFatal (argv [0]);
  
  File		output (zipOutputFile, IOFlags::OpenWrite
                  | IOFlags::OpenCreate | IOFlags::OpenTruncate);
  StorageOutputStream	soutput (&output);
  BufferOutputStream	boutput (&soutput);
  ZipOutputStream	t (&boutput);
  
  std::cout << "make a Zip of directory " << inputDir.name() << std::endl; 
  DirIterator first (inputDir.name());
  DirIterator last;
  
  std::vector<ZipMember *> outmembers; // FIXME: Stream should maintain...
  
  for ( ; first != last; ++first) { 
    
    Filename relfile(*first); 
    Filename fname(inputDir,relfile); 
    
    std::cout << " filename " << fname.name() << std::endl;
    
    
    if (fname.isRegular() /* && fname.extension() == "cpp" */  ) {      
      std::cout << "file " << fname.name() << " is regular" << std::endl;
      File		input (fname.name());
      
      char		buf [1024];
      IOSize		read;
      
      //std::vector<ZipMember *> inmembers;  // FIXME: Stream should maintain...
      
      ZipMember *outmember = new ZipMember (*first);
      outmembers.push_back (outmember);
      //	outmember->size(input.size());
      // neet to set method
      outmember->method(ZConstants::STORED);
      
      std::cout << "*** ADD " << outmember->name ()
                     << " (" << outmember->size (ZipMember::UNCOMPRESSED) << ")\n";
      t.nextMember (outmember);
      while ((read = input.read (buf, sizeof (buf))))
        t.write (buf, read);
      t.closeMember ();
    }
  }
  
  // FIXME: This should really be simpler to do, but at the moment
  //   that's difficult to achieve.  The input stream won't know
  //   the central directory extra data until it's seen it, and
  //   we can't share the members since the compression settings
  //   aren't unambiguous so the compressed size might change.
  //   The output side might change various fields and cause the
  //   input side to bail out when it gets to the central dir.
  
  /*
    for (size_t i = 0; i < outmembers.size (); ++i)
    {
    void		*extra = 0;
    size_t		len = 0;
    //inmembers [i]->extra (ZipMember::CENTRAL, &extra, &len);
    outmembers [i]->extra (ZipMember::CENTRAL, extra, len);
    }
  */
  
  t.close ();
  return 0;
}
