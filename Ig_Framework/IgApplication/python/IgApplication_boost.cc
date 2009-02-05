
// Boost Includes ==============================================================
#include <boost/python.hpp>
#include <boost/cstdint.hpp>
#include <boost/python/detail/api_placeholder.hpp>

// Includes ====================================================================
#include <Ig_Framework/IgApplication/interface/IgApplication.h>
#include <Ig_Framework/IgObjectBrowser/interface/IgState.h>
#include <list>
#include <string>
#include <iostream>

// Using =======================================================================
using namespace boost::python;

// Declarations ================================================================

namespace  {

struct IgApplication_Wrapper: IgApplication
{
    IgApplication_Wrapper(PyObject* self_, const IgApplication& p0):
        IgApplication(p0), self(self_) {}

    IgApplication_Wrapper(PyObject* self_):
        IgApplication(), self(self_) {}

    int pythonRun (list args) {
        int size = boost::python::len (args);
	char **argv = new char *[size+1];
	
	for (int i = 0; i < size; i++)
	{
	    argv[i] = extract <char *>(args[i]);	    
        }
        return IgApplication::run (size, argv);	
    }
    
    int run(int p0, char **p1) {
        return call_method< int >(self, "run", p0, p1);
    }

    int default_run(int p0, char **p1) {
        return IgApplication::run (p0, p1);
    }

    IgState* state() const {
        return call_method< IgState* >(self, "state");
    }

    IgState* default_state() const {
        return IgApplication::state();
    }

    const char* driver() const {
        return call_method< const char* >(self, "driver");
    }

    const char* default_driver() const {
        return IgApplication::driver();
    }

    const char* appname() const {
        return call_method< const char* >(self, "appname");
    }

    const char* default_appname() const {
        return IgApplication::appname();
    }

    bool verbose() const {
        return call_method< bool >(self, "verbose");
    }

    bool default_verbose() const {
        return IgApplication::verbose();
    }

    int argc() const {
        return call_method< int >(self, "argc");
    }

    int default_argc() const {
        return IgApplication::argc();
    }

    int usage() {
        return call_method< int >(self, "usage");
    }

    int default_usage() {
        return IgApplication::usage();
    }

    void startProfiler() {
        call_method< void >(self, "startProfiler");
    }

    void default_startProfiler() {
        IgApplication::startProfiler();
    }

    void startMemStats() {
        call_method< void >(self, "startMemStats");
    }

    void default_startMemStats() {
        IgApplication::startMemStats();
    }

    int initDebugging(const char* p0) {
        return IgApplication::initDebugging(p0);
	//        return call_method< int >(self, "initDebugging", p0);
    }

    int default_initDebugging(const char* p0) {
        return IgApplication::initDebugging(p0);
    }

    int initDatabase() {
        return IgApplication::initDatabase();
	//        return call_method< int >(self, "initDatabase");
    }

    int default_initDatabase() {
        return IgApplication::initDatabase();
    }

    int dumpDatabase() {
        return call_method< int >(self, "dumpDatabase");
    }

    int default_dumpDatabase() {
        return IgApplication::dumpDatabase();
    }

    int initState() {
        return IgApplication::initState();
        //return call_method< int >(self, "initState");
    }

    int default_initState() {
        return IgApplication::initState();
    }

    int loadDriver() {
        return IgApplication::loadDriver();
    }

    int default_loadDriver() {
        return IgApplication::loadDriver();
    }

    PyObject* self;
};


}// namespace 


// Module ======================================================================
BOOST_PYTHON_MODULE(IgApplication)
{
    class_< IgApplication, IgApplication_Wrapper >("IgApplication", init<  >())
        .def(init< const IgApplication& >())
        .def("run", &IgApplication::run, &IgApplication_Wrapper::default_run)
        .def("state", &IgApplication::state, &IgApplication_Wrapper::default_state, return_internal_reference< 1 >())
        .def("driver", &IgApplication::driver, &IgApplication_Wrapper::default_driver)
        .def("appname", &IgApplication::appname, &IgApplication_Wrapper::default_appname)
        .def("verbose", &IgApplication::verbose, &IgApplication_Wrapper::default_verbose)
        .def("argc", &IgApplication::argc, &IgApplication_Wrapper::default_argc)
        .def("pythonRun", &IgApplication_Wrapper::pythonRun)
    ;

}

