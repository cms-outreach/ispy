#include "test/Test.h" 
#include "classlib/zip/GZIPInputStream.h"
#include "classlib/iobase/File.h"
#include "classlib/iobase/Filename.h"
#include "classlib/iotools/StorageInputStream.h"
#include "classlib/iotools/BufferInputStream.h"
#include "classlib/utils/Signal.h"
#include <iostream>

using namespace lat;

class CheckedGZIPInputStream : public GZIPInputStream
{
public:
    CheckedGZIPInputStream (InputStream *input)
	: GZIPInputStream (input) {}

    IOSize	left (void) {
	IOSize n = 0;
	while (m_input->read () != -1)
	    ++n;
	return n;
    }
};

int TEST(int, char **argv)
{
    Signal::handleFatal (argv [0]);

    File			input ("test.gz");
    StorageInputStream		sinput (&input);
    BufferInputStream		binput (&sinput);
    CheckedGZIPInputStream	t (&binput);
    char			buf [1024];

    std::cout << "reading: " << std::flush;
    while (t.read (buf, sizeof (buf)))
	std::cout << "." << std::flush;

    std::cout << "\n*** bytes left in stream: " << t.left () << "\n";
    return 0;
}
