#include "QtGUI/ISpyApplication.h"
#include <iostream>
#include <cstdlib>


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


int
main (int argc, char *argv[])
{
  putenv(strdup("LANG=C"));
  putenv(strdup("LC_ALL=C"));

  // Do not inform when COIN is running using indirect rendering
  // because it simply confuses things and there is not
  // much the user can really do since
  //
  // COIN_FULL_INDIRECT_RENDERING=1
  //
  // most likely will cause even more user feedback.
  //
  // TODO: check the actual impact of COIN_FULL_INDIRECT_RENDERING=1
  putenv(strdup("COIN_DONT_INFORM_INDIRECT_RENDERING=1"));

  ISpyApplication app;
  return app.run (argc, argv);
}
