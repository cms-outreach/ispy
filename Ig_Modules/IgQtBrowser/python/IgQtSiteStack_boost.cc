
// Boost Includes ==============================================================
#include <boost/python.hpp>
#include <boost/cstdint.hpp>

// Includes ====================================================================
#include <Ig_Framework/IgObjectBrowser/interface/IgSite.h>
#include <Ig_Framework/IgObjectBrowser/interface/IgState.h>
#include <Ig_Modules/IgQtBrowser/interface/IgQtSite.h>
#include <Ig_Modules/IgQtBrowser/interface/IgQtSiteStack.h>
#include <qwidget.h>

// Using =======================================================================
using namespace boost::python;

// Declarations ================================================================
namespace  {

struct IgQtSiteStack_Wrapper: IgQtSiteStack
{
    IgQtSiteStack_Wrapper(PyObject* self_, const IgQtSiteStack& p0):
        IgQtSiteStack(p0), __pyste_self(self_) {}

    IgQtSiteStack_Wrapper(PyObject* self_):
        IgQtSiteStack(), __pyste_self(self_) {}

    void tile() {
        call_method< void >(boost::ref (__pyste_self), "tile");
    }

    void default_tile() {
        IgQtSiteStack::tile();
    }

    void cascade() {
        call_method< void >(boost::ref (__pyste_self), "cascade");
    }

    void default_cascade() {
        IgQtSiteStack::cascade();
    }

    QWidget* self() {
        return call_method< QWidget* >(__pyste_self, "self");
    }

    QWidget* host() {
        return call_method< QWidget* >(__pyste_self, "host");
    }

    void host(QWidget* p0) {
        call_method< void >(__pyste_self, "host", p0);
    }

    PyObject* __pyste_self;
};


}// namespace 


// Module ======================================================================
BOOST_PYTHON_MODULE(IgQtSiteStack)
{
    class_< IgQtSiteStack, bases <IgQtSite>, boost::noncopyable, std::auto_ptr<IgQtSiteStack_Wrapper> >("IgQtSiteStack", no_init)
        .def("tile", &IgQtSiteStack::tile, &IgQtSiteStack_Wrapper::default_tile)
        .def("cascade", &IgQtSiteStack::cascade, &IgQtSiteStack_Wrapper::default_cascade)
        .def("currentName", &IgQtSiteStack::currentName)
        .def("host", (void (*)(IgSite*, QWidget*))&IgQtSite::host)
        .staticmethod("host")
    ;

}

