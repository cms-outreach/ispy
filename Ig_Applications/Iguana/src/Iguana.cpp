//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Framework/IgApplication/interface/IgApplication.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

#if __GNUC__ == 2
// Force instantiation of the default allocator on GCC 2.9x.  On RH 6.x
// there is some bug somewhere that manifests itself as different bits
// of the code getting different allocator free lists, with the result
// that STL containers and strings leak memory like sieve, depending on
// actual argument and return value passing conventions.  This seems to
// be a binutils bug, but no one seems to know for sure; the template
// instantiations *should* get unified, but they are not.  Forcing the
// instantiation here makes the solution definite.
//
// For a detailed report of this bug, please refer to:
//   http://gcc.gnu.org/ml/gcc-bugs/1999-07/msg00129.html

template class __default_alloc_template<1, 0>;
#endif

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

int
main (int argc, char *argv[])
{
    IgApplication app;
    return app.run (argc, argv);
}
