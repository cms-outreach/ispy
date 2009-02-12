# ifdef PROJECT_NAME
#  include <Iguana/Framework/interface/IgLinearAlgebra.h>
# else
#  include <IgLinearAlgebra.h>
# endif
#include <cassert>

int 
main(int argc, char **argv)
{
  {
    IgV2d();
    IgV3d();
    IgV4d();
    IgV2d a(1,2);
    IgV3d b(3,4,5);
    IgV4d c(6,7,8,9);
    assert(a.x() == 1);
    assert(a.y() == 2);
    assert(b.x() == 3);
    assert(b.y() == 4);
    assert(b.z() == 5);
    assert(c.x() == 6);
    assert(c.y() == 7);
    assert(c.z() == 8);
    assert(c.w() == 9);
    assert(a[0] == 1);
    assert(a[1] == 2);
    assert(b[0] == 3);
    assert(b[1] == 4);
    assert(b[2] == 5);
    assert(c[0] == 6);
    assert(c[1] == 7);
    assert(c[2] == 8);
    assert(c[3] == 9);
  }
}
