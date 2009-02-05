#ifndef IG_PYTHON_IG_PYTHON_INTERPRETER_SERVICE_H
# define IG_PYTHON_IG_PYTHON_INTERPRETER_SERVICE_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgPython/interface/config.h"
# include <iostream>
# include <string>
# ifdef slots
#  undef slots
# endif
//# include <Python.h>
# include <boost/python.hpp>
# include <boost/python/object.hpp>

# ifndef slots
#  define slots
# endif

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgPythonInterpreter;
class IgPythonCompleter;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

using namespace boost::python;


/**This is a generic python interpreter class.  It allows python
   scripts to be executed from C++ environment and provides means to
   interact with the python environment via boost_python.  Each
   interpreter has it's own dictionary so that different interpreters
   don't interact the one with the other. However this can be changed
   and a single dictionary for different interpreter instances can be
   specified.
 */
class IG_PYTHON_API IgPythonInterpreter
{
public:
    /**Initialization of the interpreter is done only once.  By
       default it uses a copy of the __builtins__ dictionary, so that
       each instance of the object is actually a separate python
       environment (hopefully).
    */
    IgPythonInterpreter (void);

    /**Finalizes the Python environment only if reference counter is
     zero
     */	
    ~IgPythonInterpreter (void);

    /**Sets/gets both the local and the global python namespace to the
       object obj.  This way (hopefully) only methods/members of obj
       can be called.  This should limit the interface accessible to
       the scripts to only the one we want them to see.
    */
    void setNamespace (object *obj);
    void setLocalNamespace (object *obj);
    object &getLocalNamespace (void);    
    void setGlobalNamespace (object *obj);
    object &getGlobalNamespace (void);    

    /**Loads a module and adds it to the python environment.
     */
    void loadModule (char *module);
    /**Adds a C++ created python object to the global dictionary of the python environment.
     */
    //void addObject(char *name,object &obj);
    
    ///**Extracts a python object from the current dictionary so that it can be handled from C++.
    // */
    //object &getObject(const char *name);
    //object &getLocalObject(const char *name);
    //object &getGlobalObject(const char *name);
    
    /**Interprets a python script contained in a char array.  
       \param script the script to be executed.
     */
    void interpret (const char * script);
    /**Evaluates the passed python expression.
       \param script the expression to be evaluated.
       \return a pointer to a python object containing the result of the evaluation. 
     */
    object *eval (const char * script);
    
    /**Executes a script from disk.
       \param name the name of the script to be executed.
    */
    void execute (const char *name);

    /**Redirects the errors of the pythn environment to the provided
       std::ostream
       
       \param stream the stream to redirect output to.
     */
    void redirectError (std::ostream &stream = std::cerr);    
    
    /**Redirects the output of the pythn environment to the provided
       std::ostream
       
       \param stream the stream to redirect errors to.
     */
    void redirectOutput (std::ostream &stream = std::cout);    

    /**Completes the passed name according to current dictionary
       \param objName the string to be completed
     */
    std::string completeName (const char *objName);
    

    // implicit copy constructor
    // implicit assignment operator
private:    
    handle <> m_mainModuleHandle;
    handle <> m_mainNamespaceHandle;
    object *m_localDict;
    object *m_globalDict;
    object *m_builtinDict;
    IgPythonCompleter *m_completer;    
};


//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_PYTHON_IG_PYTHON_INTERPRETER_SERVICE_H
