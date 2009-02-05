//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgPython/interface/IgPythonInterpreter.h"
# include "Ig_Modules/IgPython/src/IgPythonOStream.h"
# include "Ig_Modules/IgPython/interface/IgPythonFactory.h"
# include "Ig_Modules/IgPython/interface/IgPythonCompleter.h"
# include <iostream>
# include <fstream>
# include <sstream>
# include <Python.h>
# include <boost/python.hpp>
# include <boost/python/object.hpp>
# include <boost/python/dict.hpp>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgPythonInterpreter::IgPythonInterpreter (void)
    :m_mainModuleHandle (borrowed (PyImport_AddModule ((char *)"__main__"))),
     m_mainNamespaceHandle (borrowed (PyModule_GetDict (m_mainModuleHandle.get ()))),
     m_completer (0)
{
    m_builtinDict = new dict (m_mainNamespaceHandle);
    
    PyDict_SetItemString (m_builtinDict->ptr (),
			  (char *) "__builtins__" , 
			  PyEval_GetBuiltins ());
    PyDict_SetItemString (m_builtinDict->ptr (), 
			  (char *) "sys" , 
			  PyImport_AddModule ((char*)"sys"));
    PyDict_SetItemString (m_builtinDict->ptr (), 
			  (char *) "__globals__", 
			  m_builtinDict->ptr ());
    
    m_localDict = m_builtinDict;
    m_globalDict = m_builtinDict;
    
    //This initializes the completer:
    IgPythonFactory<IgPythonCompleter> completerFactory (this);
    completerFactory.loadScript ("Completer");
    completerFactory.set ("IgDefaultCompleter");
    m_completer = &(completerFactory).create ();
}

IgPythonInterpreter::~IgPythonInterpreter (void)
{
    /*Deletes the dictionary. Notice that we don't delete m_localDict
      or m_globalDict because they could be overridden from the
      outside. The creators of such overriding dictionaries should be
      held responsible for their deletion.
    */
    delete m_builtinDict;
}

void
IgPythonInterpreter::setNamespace (object *obj)
{
    setLocalNamespace (obj);
    setGlobalNamespace (obj);
}

void 
IgPythonInterpreter::setLocalNamespace (object *obj)
{
    m_localDict = obj;
}

object &
IgPythonInterpreter::getLocalNamespace (void)
{
    return *m_localDict;
}

void 
IgPythonInterpreter::setGlobalNamespace (object *obj)
{
    m_globalDict = obj;
}

object &
IgPythonInterpreter::getGlobalNamespace (void)
{
    return *m_globalDict;
}

void 
IgPythonInterpreter::interpret(const char *script)
{
    //Minimal exception handling.
    try
    {
	PyObject *obj = PyRun_String ((char *) script, Py_file_input,
				      m_builtinDict->ptr (),
				      m_builtinDict->ptr ());
	object result = object (handle<> (borrowed (obj)));
    }
    catch (error_already_set)
    {
	PyErr_Print();
    }
}

object* 
IgPythonInterpreter::eval (const char *script)
{
    //Minimal exception handling.
    try
    {
	PyObject *obj = PyRun_String ((char *) script, Py_eval_input,
				      m_builtinDict->ptr (),
				      m_builtinDict->ptr ());
	return new object (handle<> (obj));
    }catch(error_already_set)
    {
	PyErr_Print();
    }
    return 0;
}

void
IgPythonInterpreter::execute (const char *name)
{
    std::ostringstream pythonScript;    
    std::ifstream file (name);
    pythonScript << file.rdbuf () << std::flush;
    interpret (pythonScript.str ().c_str ());
}

void
IgPythonInterpreter::redirectError (std::ostream &stream)
{
    (getGlobalNamespace()["sys"]).attr ("stderr") 
	= ((*(new class_<IgPythonOStreamError>("IgPythonOStreamError"))).def ("write", &IgPythonOStreamError::write)) ();
    IgPythonOStreamError &igStream = extract<IgPythonOStreamError &> ((getGlobalNamespace ()["sys"]).attr ("stderr")) ();
    igStream.stream() = &stream;
}

void
IgPythonInterpreter::redirectOutput (std::ostream &stream)
{
    (getGlobalNamespace()["sys"]).attr("stdout") = 
	((*(new class_<IgPythonOStreamOutput>("IgPythonOStreamOutput"))).def ("write", &IgPythonOStreamOutput::write))();
    
    IgPythonOStreamOutput &igStream = 
	extract<IgPythonOStreamOutput &>((getGlobalNamespace ()["sys"]).attr ("stdout")) ();
    igStream.stream () = &stream;
}

std::string
IgPythonInterpreter::completeName (const char *objName)
{
    std::string t_string = m_completer->completeName (objName);
    return t_string;
}

/*Here starts the global python class, this will be changed in a full
  blown singleton*/

namespace
{
    class IgPythonInterpreterBootstrap
    {
    public:
	IgPythonInterpreterBootstrap ()
	    {
		Py_Initialize ();
	    }

	~IgPythonInterpreterBootstrap ()
	    {
		Py_Finalize ();
	    }    
    };
    
    IgPythonInterpreterBootstrap bootMe;
}
