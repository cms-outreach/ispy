// TestScene.h: interface for the TestScene class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TESTSCENE_H__91698661_F86D_45D2_A0A8_BFF211613260__INCLUDED_)
#define AFX_TESTSCENE_H__91698661_F86D_45D2_A0A8_BFF211613260__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Ig_Examples/IgVisEx/interface/config.h"
#include "Ig_Modules/IgVis/interface/IgQtTwig.h"

/*!   \class TestScene TestScene.h TestScene.h
*
*     \brief Example class for returning pick information on smart objects
*
*     \author G. Alverson, Northeastern University
*     \data  04-Sep-2000
*/

#include <list>
#include <vector>
#include <memory>
#include <iostream>
#include "qobject.h"
#include "TestObject.h"
#include "Ig_Modules/IgVis/interface/IgQtLiveTwig.h"
class QTabWidget;

//////////////////////////////////////////////////////////////////////
// Internal definition of TestTwig
//////////////////////////////////////////////////////////////////////
class TestTwig : public IgQtLiveTwig<TestObject>
{
public:
    //! c-tors same as for IgQtTwig
    explicit TestTwig(IgQtTwig *parent, SbString name, bool visibility = true, bool cascade = true);
    //! alternate c-tor
    explicit TestTwig(SbString name, bool visibility = true, bool cascade = true);
    //! d-tor
    virtual ~TestTwig();
    virtual int selectState(const TestObject* data);
    //! get/set prototypes
    void setThreshold(int newThreshold) {threshold = newThreshold;}
    void getThreshold(int thresh) {thresh = threshold;}
private:
    int threshold;
};

class TestScene : public QObject, public IgQtTwig  
{
    Q_OBJECT

public:
    //! only arg is the output stream; all else from Twig
    explicit TestScene(std::ostream &stream = std::cout, bool build = true);
    //! d-tor
    virtual ~TestScene();
    //! re-implement method from Twig to supply real info
    void printInfo (std::ostream &os, const SoNode* node) const;
    //! to pass site for any controls; returns true if accepted
    bool setControlWidget(QWidget* qtw);
public slots:
    virtual void thresholdSlot(int);
    virtual void doubleSlot(double);
protected:
    void setList(std::list<TestObject> *pl){if (p_list) delete p_list; p_list = pl;} //leaks like a sieve: use w/ care
    void setVector(std::vector<TestObject> *pv){if (p_vector) delete p_vector; p_vector = pv;} // ditto
    virtual void buildScene();
private:
    //! internal list of objects we own
    std::list<TestObject> *p_list;
    std::vector<TestObject> *p_vector;
    // put this in the class strictly as a hack to test the threshold slider
    TestTwig *p_local;
};

#endif // !defined(AFX_TESTSCENE_H__91698661_F86D_45D2_A0A8_BFF211613260__INCLUDED_)
