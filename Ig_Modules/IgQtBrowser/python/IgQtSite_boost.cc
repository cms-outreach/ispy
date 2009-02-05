
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

struct IgQtSite_Wrapper: IgQtSite
{
    IgQtSite_Wrapper(PyObject* self_, const IgQtSite& p0):
        IgQtSite(p0), __pyste_self(self_) {}

    IgQtSite_Wrapper(PyObject* self_):
        IgQtSite(), __pyste_self(self_) {}

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
BOOST_PYTHON_MODULE(IgQtSite)
{
    class_< IgQtSite, boost::noncopyable, std::auto_ptr<IgQtSite_Wrapper> >("IgQtSite", no_init)
        .def("host", (void (*)(IgSite*, QWidget*))&IgQtSite::host)
        .staticmethod("host")
    ;

}

