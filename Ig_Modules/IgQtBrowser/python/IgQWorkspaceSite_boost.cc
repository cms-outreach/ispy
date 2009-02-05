
// Boost Includes ==============================================================
#include <boost/python.hpp>
#include <boost/cstdint.hpp>

// Includes ====================================================================
#include <Ig_Framework/IgObjectBrowser/interface/IgState.h>
#include <Ig_Modules/IgQtBrowser/interface/IgQtSite.h>
#include <Ig_Modules/IgQtBrowser/interface/IgQWorkspaceSite.h>
#include <qwidget.h>

// Using =======================================================================
using namespace boost::python;

// Declarations ================================================================
namespace  {

struct IgQWorkspaceSite_Wrapper: IgQWorkspaceSite
{
    IgQWorkspaceSite_Wrapper(PyObject* self_, const IgQWorkspaceSite& p0):
        IgQWorkspaceSite(p0), __pyste_self(self_) {}

    IgQWorkspaceSite_Wrapper(PyObject* self_, IgState* p0, IgSite* p1):
        IgQWorkspaceSite(p0, p1), __pyste_self(self_) {}

    QWidget* self() {
        return call_method< QWidget* >(__pyste_self, "self");
    }

    QWidget* default_self() {
        return IgQWorkspaceSite::self();
    }

    QWidget* host() {
        return call_method< QWidget* >(__pyste_self, "host");
    }

    QWidget* default_host() {
        return IgQWorkspaceSite::host();
    }

    void host(QWidget* p0) {
        call_method< void >(__pyste_self, "host", p0);
    }

    void default_host(QWidget* p0) {
        IgQWorkspaceSite::host(p0);
    }

    PyObject* __pyste_self;
};


}// namespace 


// Module ======================================================================
BOOST_PYTHON_MODULE(IgQWorkspaceSite)
{
    class_< IgQWorkspaceSite, bases<IgQtSiteStack>, IgQWorkspaceSite_Wrapper >("IgQWorkspaceSite", init< const IgQWorkspaceSite& >())
        .def(init< IgState*, IgSite* >())
        .def("catalogLabel", &IgQWorkspaceSite::catalogLabel)
        .def("tile", &IgQWorkspaceSite::tile)
        .def("cascade", &IgQWorkspaceSite::cascade)
        .def("currentName", &IgQtSiteStack::currentName)
        .def("host", (void (*)(IgSite*, QWidget*))&IgQtSite::host)
        .staticmethod("catalogLabel")
        .staticmethod("host")
    ;

}

