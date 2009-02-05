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
MMM_DEFUN_FUNC(int,::,cross,(X *, W *)) { std::cout << "[cross (X *, W *)]"; return 1; }
MMM_DEFUN_FUNC(int,::,cross,(Y *, W *)) { std::cout << "[cross (Y *, W *)]"; return 2; }
MMM_DEFUN_FUNC(int,::,cross,(Z *, W *)) { std::cout << "[cross (Z *, W *)]"; return 3; }
MMM_DEFUN_FUNC(int,::,cross,(Z *, U *)) { std::cout << "[cross (Z *, U *)]"; return 4; }
MMM_DEFUN_FUNC(int,::,cross,(Y *, V *)) { std::cout << "[cross (Y *, V *)]"; return 5; }

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
    std::cout << "(x, w) -> " << cross (&x, &w) << std::endl;
    std::cout << "(z, v) -> " << cross (&z, &v) << std::endl;

    return 0;
}
