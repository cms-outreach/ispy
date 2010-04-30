#include <QtTest/QtTest>

extern void doTestCreateSimpleCollection();
extern void doTestCreateSimpleAssociations();
extern void doTestCreateSimpleAssociations02();
extern void doTestNavigateAssociations();
extern void doTestCreateWriteReadWriteReadPrintNoAssociations();
extern void doTestCreateReadWriteReadPrint();
extern void doTestNavigateAssociationsOneToOne();
extern void doTestNavigateOneAssociation();

class IgCollectionTest: public QObject
{
    Q_OBJECT
private slots:
    void initTestCase()
    { qDebug("Init test cases."); }

    void testCreateSimpleCollection(){doTestCreateSimpleCollection();}
    void testCreateSimpleAssociations(){doTestCreateSimpleAssociations();}
    void testCreateSimpleAssociations02(){doTestCreateSimpleAssociations02();}
    void testNavigateOneAssociation(){doTestNavigateOneAssociation();};
    void testNavigateAssociations(){doTestNavigateAssociations();}
    void testCreateWriteReadWriteReadPrintNoAssociations()
    {doTestCreateWriteReadWriteReadPrintNoAssociations();}
    void testCreateReadWriteReadPrint(){doTestCreateReadWriteReadPrint();}
    void testNavigateAssociationsOneToOne(){doTestNavigateAssociationsOneToOne();}

    void cleanupTestCase()
    { qDebug("Cleanup"); }
};

QTEST_MAIN(IgCollectionTest)
#include "tests/tmp/test_driver.moc"
