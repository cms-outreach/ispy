
// Includes ====================================================================
#include <boost/python.hpp>
#include "Ig_Framework/IgObjectBrowser/interface/IgRepresentable.h"
#include <Ig_Modules/IgCommonData/interface/IgTwig.h>

// Using =======================================================================
using namespace boost::python;

// Declarations ================================================================
namespace  {


struct IgCompoundTwig_Wrapper: IgCompoundTwig
{
    IgCompoundTwig_Wrapper(PyObject* self_, IgTwig * p0, unsigned int p1):
        IgCompoundTwig(boost::ref (p0), p1), self(self_) {}

    unsigned int children() {
        return call_method< unsigned int >(self, "children");
    }

    unsigned int default_children() {
        return IgCompoundTwig::children();
    }

    IgTwig * child(unsigned int p0) {
        return call_method< IgTwig * >(self, "child", p0);
    }

    IgTwig * default_child(unsigned int p0) {
        return IgCompoundTwig::child(p0);
    }

    unsigned int indexOf(const IgTwig * p0) const {
        return call_method< unsigned int >(self, "indexOf", p0);
    }

    unsigned int default_indexOf(const IgTwig * p0) const {
        return IgCompoundTwig::indexOf(p0);
    }

    bool expand() {
        return call_method< bool >(self, "expand");
    }

    bool default_expand() {
        return IgCompoundTwig::expand();
    }

    void add(IgTwig * p0) {
        call_method< void >(self, "add", p0);
    }

    void default_add(IgTwig * p0) {
        IgCompoundTwig::add(p0);
    }

    void remove(IgTwig * p0) {
        call_method< void >(self, "remove", p0);
    }

    void default_remove(IgTwig * p0) {
        IgCompoundTwig::remove(p0);
    }

    void destroy() {
        call_method< void >(self, "destroy");
    }

    void default_destroy() {
        IgCompoundTwig::destroy();
    }

    void clear() {
        call_method< void >(self, "clear");
    }

    void default_clear() {
        IgCompoundTwig::clear();
    }

    PyObject* self;
};



}// namespace 


// Module ======================================================================
BOOST_PYTHON_MODULE(IgCompoundTwig)
{
    class_< IgCompoundTwig, boost::noncopyable, bases<IgTwig, IgRepresentable>, IgCompoundTwig_Wrapper >("IgCompoundTwig", no_init)
        .def("children", &IgCompoundTwig::children, &IgCompoundTwig_Wrapper::default_children)
        .def("child", &IgCompoundTwig::child, &IgCompoundTwig_Wrapper::default_child, return_value_policy<reference_existing_object>())
        .def("indexOf", &IgCompoundTwig::indexOf, &IgCompoundTwig_Wrapper::default_indexOf)
        .def("expand", &IgCompoundTwig::expand, &IgCompoundTwig_Wrapper::default_expand)
        .def("add", &IgCompoundTwig::add, &IgCompoundTwig_Wrapper::default_add)
        .def("remove", &IgCompoundTwig::remove, &IgCompoundTwig_Wrapper::default_remove)
        .def("destroy", &IgCompoundTwig::destroy, &IgCompoundTwig_Wrapper::default_destroy)
        .def("clear", &IgCompoundTwig::clear, &IgCompoundTwig_Wrapper::default_clear)
    ;

}
