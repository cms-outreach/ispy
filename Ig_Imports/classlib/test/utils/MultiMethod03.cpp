#include "test/Test.h" 
#include "classlib/utils/DebugAids.h"
#include "classlib/utils/Signal.h"
#include "classlib/utils/MultiMethod.h"
#include "classlib/utils/Log.h"
#include <iostream>

using namespace lat;

class X { public: virtual ~X (); };
class Y : public X { };
class Z : public Y { };

class U { public: virtual ~U (); };
class V : public U { };
class W : public V { };

X::~X (void) {}
U::~U (void) {}

//////////////////////////////////////////////////////////////////////
#define API
XTYPEINFO_DECLARE (API, X);
XTYPEINFO_DECLARE (API, Y);
XTYPEINFO_DECLARE (API, Z);
XTYPEINFO_DECLARE (API, U);
XTYPEINFO_DECLARE (API, V);
XTYPEINFO_DECLARE (API, W);

// MSVC++ doesn't like empty macro arguments and using colon causes
// scope parsing difficulties for various compilers.
#define BLANK

MULTIMETHOD_DECLARE(extern,int,cross,2,0,(X,U));
MULTIMETHOD_DEFINE (BLANK, cross);
MMM_DEFUN_FUNC(int,::,cross,(X *, U *)) { std::cout << "[cross (X *, U *)]"; return 0; }

MULTIMETHOD_DECLARE(extern,int,confound,2,0,(U,X));
MULTIMETHOD_DEFINE (BLANK, confound);
MMM_DEFUN_FUNC(int,::,confound,(U *, X *)) { std::cout << "[confound (U *, X *)]"; return 0; }


XTYPEINFO_DEF_0(X);
XTYPEINFO_DEF_1(Y, X,false);
XTYPEINFO_DEF_1(Z, Y,false);

XTYPEINFO_DEF_0(U);
XTYPEINFO_DEF_1(V, U,false);
XTYPEINFO_DEF_1(W, V,false);

//////////////////////////////////////////////////////////////////////

int TEST(int /*argc*/, char **argv)
{
    Signal::handleFatal (argv[0]);

    X x; Y y; Z z;
    U u; V v; W w;

    std::cout << "(x, u) -> " << cross (&x, &u) << std::endl;
    std::cout << "(u, x) -> " << confound (&u, &x) << std::endl;

    return 0;
}
