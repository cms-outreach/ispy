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
IGUANA_CAPABILITIES (const char **&names, int &)
{
    static const char *caps [] = {
	"IGUANA/Test Capabilities/&!*@^#!&*@[]\\{}|",
	"IGUANA/Test Capabilities/?<>\"';:~!@#$%&*()_+=-`",
	0
    };

    names = caps;
}
