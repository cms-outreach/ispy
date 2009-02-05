
// Boost Includes ==============================================================
#include <boost/python.hpp>
#include <boost/cstdint.hpp>

// Includes ====================================================================
#include <Ig_Modules/IgStudio/interface/IgViewDB.h>
#include <Ig_Modules/IgStudio/interface/IgPage.h>
#include <Ig_Modules/IgStudio/interface/IgView.h>

// Using =======================================================================
using namespace boost::python;

// Declarations ================================================================
namespace  {

struct IgViewDB_Wrapper: IgViewDB
{
    IgView* create(const std::basic_string<char,std::char_traits<char>,std::allocator<char> >& p0, IgPage* p1) const {
        return call_method< IgView* >(self, "create", p0, p1);
    }

    IgView* default_create(const std::basic_string<char,std::char_traits<char>,std::allocator<char> >& p0, IgPage* p1) const {
        return IgPluginFactory<IgView*(IgPage*)>::create(p0, p1);
    }

    IgView* create(IgMapUniqueIterator<std::multimap<std::basic_string<char, std::char_traits<char>, std::allocator<char> >, IgPluginFactoryImplTypes<IgView*(IgPage*)>::Info*, std::less<std::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::pair<const std::basic_string<char, std::char_traits<char>, std::allocator<char> >, IgPluginFactoryImplTypes<IgView*(IgPage*)>::Info*> > > > p0, IgPage* p1) const {
        return call_method< IgView* >(self, "create", p0, p1);
    }

    IgView* default_create(IgMapUniqueIterator<std::multimap<std::basic_string<char, std::char_traits<char>, std::allocator<char> >, IgPluginFactoryImplTypes<IgView*(IgPage*)>::Info*, std::less<std::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::pair<const std::basic_string<char, std::char_traits<char>, std::allocator<char> >, IgPluginFactoryImplTypes<IgView*(IgPage*)>::Info*> > > > p0, IgPage* p1) const {
        return IgPluginFactory<IgView*(IgPage*)>::create(p0, p1);
    }

    void installFactory(IgModuleDef* p0, std::basic_string<char,std::char_traits<char>,std::allocator<char> > p1, IgPluginFactoryImplTypes<IgView*(IgPage*)>::Factory* p2) {
        call_method< void >(self, "installFactory", p0, p1, p2);
    }

    void default_installFactory(IgModuleDef* p0, std::basic_string<char,std::char_traits<char>,std::allocator<char> > p1, IgPluginFactoryImplTypes<IgView*(IgPage*)>::Factory* p2) {
        IgPluginFactoryImpl<IgView*(IgPage*)>::installFactory(p0, p1, p2);
    }

    void declare(IgModuleDef* p0, std::basic_string<char,std::char_traits<char>,std::allocator<char> > p1) {
        call_method< void >(self, "declare", p0, p1);
    }

    void default_declare(IgModuleDef* p0, std::basic_string<char,std::char_traits<char>,std::allocator<char> > p1) {
        IgPluginFactoryImplBase<IgPluginFactoryImplTypes<IgView*(IgPage*)> >::declare(p0, p1);
    }

    void rebuild() {
        call_method< void >(self, "rebuild");
    }

    void default_rebuild() {
        IgPluginFactoryImplBase<IgPluginFactoryImplTypes<IgView*(IgPage*)> >::rebuild();
    }

    void restore(IgModule* p0, IgModuleDescriptor* p1) {
        call_method< void >(self, "restore", p0, p1);
    }

    void default_restore(IgModule* p0, IgModuleDescriptor* p1) {
        IgPluginFactoryImplBase<IgPluginFactoryImplTypes<IgView*(IgPage*)> >::restore(p0, p1);
    }

    PyObject* self;
};


}// namespace 


// Module ======================================================================
BOOST_PYTHON_MODULE(IgViewDB)
{
    class_< IgViewDB,boost::noncopyable, IgViewDB_Wrapper >("IgViewDB", no_init)
        .def("create", (IgView* (IgPluginFactory<IgView*(IgPage*)>::*)(const std::basic_string<char,std::char_traits<char>,std::allocator<char> >&, IgPage*) const)&IgPluginFactory<IgView*(IgPage*)>::create, (IgView* (IgViewDB_Wrapper::*)(const std::basic_string<char,std::char_traits<char>,std::allocator<char> >&, IgPage*) const)&IgViewDB_Wrapper::default_create, return_value_policy< reference_existing_object >())
        .def("create", (IgView* (IgPluginFactory<IgView*(IgPage*)>::*)(IgMapUniqueIterator<std::multimap<std::basic_string<char, std::char_traits<char>, std::allocator<char> >, IgPluginFactoryImplTypes<IgView*(IgPage*)>::Info*, std::less<std::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::pair<const std::basic_string<char, std::char_traits<char>, std::allocator<char> >, IgPluginFactoryImplTypes<IgView*(IgPage*)>::Info*> > > >, IgPage*) const)&IgPluginFactory<IgView*(IgPage*)>::create, (IgView* (IgViewDB_Wrapper::*)(IgMapUniqueIterator<std::multimap<std::basic_string<char, std::char_traits<char>, std::allocator<char> >, IgPluginFactoryImplTypes<IgView*(IgPage*)>::Info*, std::less<std::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::pair<const std::basic_string<char, std::char_traits<char>, std::allocator<char> >, IgPluginFactoryImplTypes<IgView*(IgPage*)>::Info*> > > >, IgPage*) const)&IgViewDB_Wrapper::default_create, return_value_policy< reference_existing_object >())
        .def("installFactory", (void (IgPluginFactoryImpl<IgView*(IgPage*)>::*)(IgModuleDef*, std::basic_string<char,std::char_traits<char>,std::allocator<char> >, IgPluginFactoryImplTypes<IgView*(IgPage*)>::Factory*) )&IgPluginFactoryImpl<IgView*(IgPage*)>::installFactory, (void (IgViewDB_Wrapper::*)(IgModuleDef*, std::basic_string<char,std::char_traits<char>,std::allocator<char> >, IgPluginFactoryImplTypes<IgView*(IgPage*)>::Factory*))&IgViewDB_Wrapper::default_installFactory)
        .def("declare", (void (IgPluginFactoryImplBase<IgPluginFactoryImplTypes<IgView*(IgPage*)> >::*)(IgModuleDef*, std::basic_string<char,std::char_traits<char>,std::allocator<char> >) )&IgPluginFactoryImplBase<IgPluginFactoryImplTypes<IgView*(IgPage*)> >::declare, (void (IgViewDB_Wrapper::*)(IgModuleDef*, std::basic_string<char,std::char_traits<char>,std::allocator<char> >))&IgViewDB_Wrapper::default_declare)
        .def("rebuild", (void (IgPluginFactoryImplBase<IgPluginFactoryImplTypes<IgView*(IgPage*)> >::*)() )&IgPluginFactoryImplBase<IgPluginFactoryImplTypes<IgView*(IgPage*)> >::rebuild, (void (IgViewDB_Wrapper::*)())&IgViewDB_Wrapper::default_rebuild)
        .def("restore", (void (IgPluginFactoryImplBase<IgPluginFactoryImplTypes<IgView*(IgPage*)> >::*)(IgModule*, IgModuleDescriptor*) )&IgPluginFactoryImplBase<IgPluginFactoryImplTypes<IgView*(IgPage*)> >::restore, (void (IgViewDB_Wrapper::*)(IgModule*, IgModuleDescriptor*))&IgViewDB_Wrapper::default_restore)
        .def("get", &IgViewDB::get, return_value_policy< reference_existing_object >())
        .def("begin", &IgPluginFactoryImplBase<IgPluginFactoryImplTypes<IgView*(IgPage*)> >::begin)
        .def("end", &IgPluginFactoryImplBase<IgPluginFactoryImplTypes<IgView*(IgPage*)> >::end)
        .def("locate", &IgPluginFactoryImplBase<IgPluginFactoryImplTypes<IgView*(IgPage*)> >::locate)
        .def("category", &IgPluginFactoryBase::category, return_value_policy< copy_const_reference >())
        .staticmethod("get")
    ;

}

