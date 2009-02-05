
// Boost Includes ==============================================================
#include <boost/python.hpp>
#include <boost/cstdint.hpp>

// Includes ====================================================================
#include <Ig_Modules/IgCommonData/interface/IgTwig.h>

// Using =======================================================================
using namespace boost::python;

// Declarations ================================================================
namespace  {

struct IgTwig_Wrapper: IgTwig
{
    IgTwig_Wrapper (PyObject *self_, IgTwig *p0)
	:IgTwig (boost::ref (p0)), self(self_) {}
    
    unsigned int children() {
        return call_method< unsigned int >(self, "children");
    }

    unsigned int default_children() {
        return IgTwig::children();
    }

    IgTwig *child(unsigned int n) {
        return call_method< IgTwig *>(self, "child", n);
    }

    IgTwig *default_child(unsigned int n) {
        return IgTwig::child(n);
    }

    unsigned int indexOf(const IgTwig* p0) const {
        return call_method< unsigned int >(self, "indexOf", boost::ref (p0));
    }

    unsigned int default_indexOf(const IgTwig* p0) const {
        return IgTwig::indexOf(p0);
    }

    bool expand() {
        return call_method< bool >(self, "expand");
    }

    bool default_expand() {
        return IgTwig::expand();
    }

    void add(IgTwig* p0) {
        call_method< void >(self, "add", boost::ref (p0));
    }

    void default_add(IgTwig* p0) {
        IgTwig::add(boost::ref (p0));
    }

    void remove(IgTwig* p0) {
        call_method< void >(self, "remove", p0);
    }

    void default_remove(IgTwig* p0) {
        IgTwig::remove(p0);
    }

    void destroy() {
        call_method< void >(self, "destroy");
    }

    void default_destroy() {
        IgTwig::destroy();
    }

    void clear() {
        call_method< void >(self, "clear");
    }

    void default_clear() {
        IgTwig::clear();
    }

    std::string name() const {
        return call_method< std::string> (self, "name");
    }

    void name(const std::string& p0) {
        call_method< void >(self, "name", p0);
    }

    std::string fullName() {
        return call_method< std::string >(self, "fullName");
    }

    std::string default_fullName() {
        return IgTwig::fullName();
    }

    bool traverse() const {
        return call_method< bool >(self, "traverse");
    }

    bool default_traverse() const {
        return IgTwig::traverse();
    }

    bool selfTraverse() const {
        return call_method< bool >(self, "selfTraverse");
    }

    void selfTraverse(bool p0) {
        call_method< void >(self, "selfTraverse", p0);
    }

    bool visible() const {
        return call_method< bool >(self, "visible");
    }

    bool default_visible() const {
        return IgTwig::visible();
    }

    bool selfVisible() const {
        return call_method< bool >(self, "selfVisible");
    }

    void selfVisible(bool p0) {
        call_method< void >(self, "selfVisible", p0);
    }

    bool cascade() const {
        return call_method< bool >(self, "cascade");
    }

    void cascade(bool p0) {
        call_method< void >(self, "cascade", p0);
    }

    void attributes(std::map<std::string, std::pair<lat::Callback1<IgDatum*&>, lat::Callback1<const IgDatum*> > >& p0) {
        call_method< void >(self, "attributes", p0);
    }

    void default_attributes(std::map<std::string, std::pair<lat::Callback1<IgDatum*&>, lat::Callback1<const IgDatum*> > >& p0) {
        IgTwig::attributes(p0);
    }

    void added(IgTwig* p0) {
        call_method< void >(self, "added", p0);
    }

    void default_added(IgTwig* p0) {
        IgTwig::added(p0);
    }

    void removed(IgTwig* p0) {
        call_method< void >(self, "removed", p0);
    }

    void default_removed(IgTwig* p0) {
        IgTwig::removed(p0);
    }

    void cleared(IgTwig* p0) {
        call_method< void >(self, "cleared", p0);
    }

    void default_cleared(IgTwig* p0) {
        IgTwig::cleared(p0);
    }

    PyObject* self;
};


}// namespace 


// Module ======================================================================
BOOST_PYTHON_MODULE(IgTwig)
{
    class_< IgTwig, boost::noncopyable, IgTwig_Wrapper >("IgTwig", init<IgTwig *>())
        .def("children", &IgTwig::children, &IgTwig_Wrapper::default_children)
        .def("child", &IgTwig::child, return_value_policy<reference_existing_object> (), &IgTwig_Wrapper::default_child)
        .def("indexOf", &IgTwig::indexOf, &IgTwig_Wrapper::default_indexOf)
        .def("expand", &IgTwig::expand, &IgTwig_Wrapper::default_expand)
        .def("add", &IgTwig::add, &IgTwig_Wrapper::default_add,with_custodian_and_ward<1,2> ())
        .def("remove", &IgTwig::remove, &IgTwig_Wrapper::default_remove)
        .def("destroy", &IgTwig::destroy, &IgTwig_Wrapper::default_destroy)
        .def("clear", &IgTwig::clear, &IgTwig_Wrapper::default_clear)
        .def("name", (std::basic_string<char,std::char_traits<char>,std::allocator<char> > (IgTwig::*)() const)&IgTwig::name)
        .def("name", (void (IgTwig::*)(const std::basic_string<char,std::char_traits<char>,std::allocator<char> >&) )&IgTwig::name)
	.def("fullName", &IgTwig::fullName, &IgTwig_Wrapper::default_fullName)
	.def("traverse", &IgTwig::traverse, &IgTwig_Wrapper::default_traverse)
        .def("selfTraverse", (bool (IgTwig::*)() const)&IgTwig::selfTraverse)
	.def("selfTraverse", (void (IgTwig::*)(bool) )&IgTwig::selfTraverse)
        .def("visible", &IgTwig::visible, &IgTwig_Wrapper::default_visible)
        .def("selfVisible", (bool (IgTwig::*)() const)&IgTwig::selfVisible)
        .def("selfVisible", (void (IgTwig::*)(bool) )&IgTwig::selfVisible)
        .def("cascade", (bool (IgTwig::*)() const)&IgTwig::cascade)
        .def("cascade", (void (IgTwig::*)(bool) )&IgTwig::cascade)
        .def("attributes", &IgTwig::attributes, &IgTwig_Wrapper::default_attributes)
        .def("added", &IgTwig::added, &IgTwig_Wrapper::default_added)
        .def("removed", &IgTwig::removed, &IgTwig_Wrapper::default_removed)
        .def("cleared", &IgTwig::cleared, &IgTwig_Wrapper::default_cleared)
	.def("lookup", &IgTwig::lookup, return_value_policy<reference_existing_object> ())
    ;
}
