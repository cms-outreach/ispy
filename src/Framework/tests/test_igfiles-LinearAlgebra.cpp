#include <Framework/IgLinearAlgebra.h>
#include <QtTest/QtTest>

void
doTestLinearAlgebra(void)
{
  {
    IgV2d();
    IgV3d();
    IgV4d();
    IgV2d a(1,2);
    IgV3d b(3,4,5);
    IgV4d c(6,7,8,9);
    QVERIFY(a.x() == 1);
    QVERIFY(a.y() == 2);
    QVERIFY(b.x() == 3);
    QVERIFY(b.y() == 4);
    QVERIFY(b.z() == 5);
    QVERIFY(c.x() == 6);
    QVERIFY(c.y() == 7);
    QVERIFY(c.z() == 8);
    QVERIFY(c.w() == 9);
    QVERIFY(a[0] == 1);
    QVERIFY(a[1] == 2);
    QVERIFY(b[0] == 3);
    QVERIFY(b[1] == 4);
    QVERIFY(b[2] == 5);
    QVERIFY(c[0] == 6);
    QVERIFY(c[1] == 7);
    QVERIFY(c[2] == 8);
    QVERIFY(c[3] == 9);
  }
}
