
// Boost Includes ==============================================================
#include <boost/python.hpp>
#include <boost/cstdint.hpp>
#include <memory>

// Includes ====================================================================
#include <Ig_Framework/IgObjectBrowser/interface/IgRepresentable.h>
#include <Ig_Modules/IgCommonData/interface/IgTwig.h>
#include "Ig_Modules/IgCommonData/interface/config.h"

// Using =======================================================================
using namespace boost::python;

// Declarations ================================================================
struct IgSimpleTwig_Wrapper: IgSimpleTwig
{
    IgSimpleTwig_Wrapper(PyObject* self_, IgTwig *p0):
	IgSimpleTwig(boost::ref (p0)), self(self_) {}

     IgSimpleTwig_Wrapper(PyObject* self_, IgTwig *p0, const std::string& p1):
         IgSimpleTwig(boost::ref (p0), p1), self(self_) {}
    
    IgSimpleTwig_Wrapper(PyObject* self_, IgTwig *p0, const std::string& p1, bool p2):
        IgSimpleTwig(boost::ref (p0), p1, p2), self(self_) {}

    IgSimpleTwig_Wrapper(PyObject* self_, IgTwig *p0, const std::string& p1, bool p2, bool p3):
        IgSimpleTwig(boost::ref (p0), p1, p2, p3), self(self_) {}

    IgSimpleTwig_Wrapper(PyObject* self_, IgTwig *p0, const std::string& p1, bool p2, bool p3, bool p4):
        IgSimpleTwig(boost::ref (p0), p1, p2, p3, p4), self(self_) {}

    IgSimpleTwig_Wrapper(PyObject* self_, const std::string& p0):
        IgSimpleTwig(p0), self(self_) {}

    IgSimpleTwig_Wrapper(PyObject* self_, const std::string& p0, bool p1):
        IgSimpleTwig(p0, p1), self(self_) {}

    IgSimpleTwig_Wrapper(PyObject* self_, const std::string& p0, bool p1, bool p2):
        IgSimpleTwig(p0, p1, p2), self(self_) {}

    IgSimpleTwig_Wrapper(PyObject* self_, const std::string& p0, bool p1, bool p2, bool p3):
	IgSimpleTwig(p0, p1, p2, p3), self(self_) {}

    std::string name() const {
        return call_method< std::string >(self, "name");
    }

    std::string default_name() const {
        return IgSimpleTwig::name();
    }

    void name(const std::string& p0) {
        call_method< void >(self, "name", p0);
    }

    void default_name(const std::string& p0) {
        IgSimpleTwig::name(p0);
    }

    bool selfTraverse() const {
        return call_method< bool >(self, "selfTraverse");
    }

    bool default_selfTraverse() const {
        return IgSimpleTwig::selfTraverse();
    }

    void selfTraverse(bool p0) {
        call_method< void >(self, "selfTraverse", p0);
    }

    void default_selfTraverse(bool p0) {
        IgSimpleTwig::selfTraverse(p0);
    }

    bool selfVisible() const {
        return call_method< bool >(self, "selfVisible");
    }

    bool default_selfVisible() const {
        return IgSimpleTwig::selfVisible();
    }

    void selfVisible(bool p0) {
        call_method< void >(self, "selfVisible", p0);
    }

    void default_selfVisible(bool p0) {
        IgSimpleTwig::selfVisible(p0);
    }

    bool cascade() const {
        return call_method< bool >(self, "cascade");
    }

    bool default_cascade() const {
        return IgSimpleTwig::cascade();
    }

    void cascade(bool p0) {
        call_method< void >(self, "cascade", p0);
    }

    void default_cascade(bool p0) {
        IgSimpleTwig::cascade(p0);
    }

    unsigned int children() {
        return call_method< unsigned int >(self, "children");
    }

    unsigned int default_children() {
        return IgCompoundTwig::children();
    }

    unsigned int indexOf(const IgTwig* p0) const {
        return call_method< unsigned int >(self, "indexOf", boost::ref(p0));
    }

    unsigned int default_indexOf(const IgTwig* p0) const {
        return IgCompoundTwig::indexOf(boost::ref (p0));
    }

    bool expand() {
        return call_method< bool >(self, "expand");
    }

    bool default_expand() {
        return IgCompoundTwig::expand();
    }

    void add (IgTwig * p0) {
        call_method< void >(self, "add", boost::ref (p0));
    }
    
    void default_add (IgTwig * p0) {
        IgCompoundTwig::add(boost::ref (p0));
    }
    
    void remove(IgTwig* p0) {
        call_method< void >(self, "remove", boost::ref (p0));
    }

    void default_remove(IgTwig* p0) {
        IgCompoundTwig::remove(boost::ref (p0));
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

    bool visible() const {
        return call_method< bool >(self, "visible");
    }

    bool default_visible() const {
        return IgTwig::visible();
    }

    void attributes(std::map<std::string, std::pair<lat::Callback1<IgDatum*&>, lat::Callback1<const IgDatum*> > >& p0) {
        call_method< void >(self, "attributes", p0);
    }

    void default_attributes(std::map<std::string, std::pair<lat::Callback1<IgDatum*&>, lat::Callback1<const IgDatum*> > >& p0) {
        IgTwig::attributes(p0);
    }

    PyObject* self;
};

IgSimpleTwig *
createIgSimpleTwig (const std::string &name)
{
    return new IgSimpleTwig (name);    
}

// Module ======================================================================
BOOST_PYTHON_MODULE(IgSimpleTwig)
{
    //    implicitly_convertible<std::auto_ptr<IgSimpleTwig_Wrapper> , std::auto_ptr<IgSimpleTwig> > ();
 
    class_< IgSimpleTwig, boost::noncopyable, bases<IgCompoundTwig, IgTwig, IgRepresentable>, IgSimpleTwig_Wrapper >("IgSimpleTwig", init< IgTwig*, optional< const std::string&, bool, bool, bool > >())
        .def(init<const std::string&, optional< bool, bool, bool > >())
        .def("name", (std::string (IgSimpleTwig::*)() const)&IgSimpleTwig::name, (std::string (IgSimpleTwig_Wrapper::*)() const)&IgSimpleTwig_Wrapper::default_name)
        .def("name", (void (IgSimpleTwig::*)(const std::string&) )&IgSimpleTwig::name, (void (IgSimpleTwig_Wrapper::*)(const std::string&))&IgSimpleTwig_Wrapper::default_name)
        .def("selfTraverse", (bool (IgSimpleTwig::*)() const)&IgSimpleTwig::selfTraverse, (bool (IgSimpleTwig_Wrapper::*)() const)&IgSimpleTwig_Wrapper::default_selfTraverse)
        .def("selfTraverse", (void (IgSimpleTwig::*)(bool) )&IgSimpleTwig::selfTraverse, (void (IgSimpleTwig_Wrapper::*)(bool))&IgSimpleTwig_Wrapper::default_selfTraverse)
        .def("selfVisible", (bool (IgSimpleTwig::*)() const)&IgSimpleTwig::selfVisible, (bool (IgSimpleTwig_Wrapper::*)() const)&IgSimpleTwig_Wrapper::default_selfVisible)
        .def("selfVisible", (void (IgSimpleTwig::*)(bool) )&IgSimpleTwig::selfVisible, (void (IgSimpleTwig_Wrapper::*)(bool))&IgSimpleTwig_Wrapper::default_selfVisible)
        .def("cascade", (bool (IgSimpleTwig::*)() const)&IgSimpleTwig::cascade, (bool (IgSimpleTwig_Wrapper::*)() const)&IgSimpleTwig_Wrapper::default_cascade)
        .def("cascade", (void (IgSimpleTwig::*)(bool) )&IgSimpleTwig::cascade, (void (IgSimpleTwig_Wrapper::*)(bool))&IgSimpleTwig_Wrapper::default_cascade)
        .def("children", (unsigned int (IgCompoundTwig::*)() )&IgCompoundTwig::children, (unsigned int (IgSimpleTwig_Wrapper::*)())&IgSimpleTwig_Wrapper::default_children)
        .def("indexOf", (unsigned int (IgCompoundTwig::*)(const IgTwig*) const)&IgCompoundTwig::indexOf, (unsigned int (IgSimpleTwig_Wrapper::*)(const IgTwig*) const)&IgSimpleTwig_Wrapper::default_indexOf)
        .def("expand", (bool (IgCompoundTwig::*)() )&IgCompoundTwig::expand, (bool (IgSimpleTwig_Wrapper::*)())&IgSimpleTwig_Wrapper::default_expand)
	.def("add", (void (IgCompoundTwig::*)(IgTwig*) )&IgCompoundTwig::add, (void (IgSimpleTwig_Wrapper::*)(IgTwig*))&IgSimpleTwig_Wrapper::default_add)
        .def("remove", (void (IgCompoundTwig::*)(IgTwig*) )&IgCompoundTwig::remove, (void (IgSimpleTwig_Wrapper::*)(IgTwig*))&IgSimpleTwig_Wrapper::default_remove)
        .def("destroy", (void (IgCompoundTwig::*)() )&IgCompoundTwig::destroy, (void (IgSimpleTwig_Wrapper::*)())&IgSimpleTwig_Wrapper::default_destroy)
        .def("clear", (void (IgCompoundTwig::*)() )&IgCompoundTwig::clear, (void (IgSimpleTwig_Wrapper::*)())&IgSimpleTwig_Wrapper::default_clear)
        .def("fullName", (std::string (IgTwig::*)() )&IgTwig::fullName, (std::string (IgSimpleTwig_Wrapper::*)())&IgSimpleTwig_Wrapper::default_fullName)
        .def("traverse", (bool (IgTwig::*)() const)&IgTwig::traverse, (bool (IgSimpleTwig_Wrapper::*)() const)&IgSimpleTwig_Wrapper::default_traverse)
        .def("visible", &IgTwig::visible, (bool (IgSimpleTwig_Wrapper::*)() const)&IgSimpleTwig_Wrapper::default_visible)
        .def("attributes", (void (IgTwig::*)(std::map<std::string,std::pair<lat::Callback1<IgDatum*&>, lat::Callback1<const IgDatum*> > >&) )&IgTwig::attributes, (void (IgSimpleTwig_Wrapper::*)(std::map<std::string,std::pair<lat::Callback1<IgDatum*&>, lat::Callback1<const IgDatum*> > >&))&IgSimpleTwig_Wrapper::default_attributes)
    ;

    def ("createIgSimpleTwig", &createIgSimpleTwig, return_value_policy<reference_existing_object> ());    
    
}
