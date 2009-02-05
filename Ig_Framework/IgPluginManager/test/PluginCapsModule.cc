//<<<<<< INCLUDES                                                       >>>>>>
//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

extern "C" void
IGUANA_CAPABILITIES (const char **&names, int & /* n */)
{
    static const char *caps [] = {
	"IGUANA/Test Capabilities/Test 1",
	"IGUANA/Test Capabilities/Test 2",
	"IGUANA/Test Capabilities/Foo",
	0
    };

    names = caps;
}
