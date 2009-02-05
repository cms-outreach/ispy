#ifndef IG_PYTHON_IG_PYTHON_FACTORY_H
# define IG_PYTHON_IG_PYTHON_FACTORY_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgPython/interface/config.h"
# include "Ig_Modules/IgPython/interface/IgPythonInterpreter.h"
# include <string>
# include <boost/python.hpp>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** This factory is used to create python extended object of a boostified C++
    object.
    The C++ object exposes virtual function in his python interface so that one
    can extend such virtual functions in python (as well as in C++ if the class is
    derived in C++).
    
    Suppose you have an abstract C++ class like this:
    \code
    class HelloWorld
    {
    public:
      HelloWorld(void);
      virtual void greet(void)
      {
        std::cout << "Hello C++" << std::endl;
      };
    }
    \endcode
    and that you create python bindings for it with a simple Pyste file python/helloworld.pyste
    \code
    HelloWorld = Class("HelloWorld","helloworld.h")
    \endcode
    doing
    \code
    scram b python
    \endcode
    will create python bindings for it. Now you can extend the class in python:
    \code
    from PyHelloWorld import HelloWorld
    
    class HelloPython(HelloWorld):
      def greet(self):
        print "Hello Python!"
    \endcode
    
    We can now use IgPythonFactory to obtain a reference to an instance of one of those python extended objects.
    \code
    IgPythonFactory<HelloWorld> factory;
    factory.setScript("HelloPython.py");
    factory.set(HelloPython)
    HelloWorld &t = factory.create();
    t.greet();
    \endcode
    The previous code snippet would result in:
    \code
    Hello Python!
    \code
 */
template <class T>
class IG_PYTHON_API IgPythonFactory
{
    IgPythonInterpreter *m_interpreter;
    std::string m_className;
public:
    /**Creates the factory.
       \param interpreter the python interpreter object to be used for the creation
       of the objects.
     */
    IgPythonFactory (IgPythonInterpreter *interpreter);

    /**Loads the wanted script to derive and extend base class T.
      \param scriptName the name of the script in which the extended class is defined.
    */
    void loadScript(const char *scriptName);

    /**Executes the script passed to derive and extend base class T.
      \param script the name of the script in which the extended class is defined.
    */
    void setScript(const char *script);
	    
    /**Sets the name of the wanted extended class
       \param className name of the actual concrete class to be created.
     */
    void set(const char *className);

    /**Creates an instance of the python extended class, returning a instance to the
       generic baseClass.
       \return a reference to the created factory.
     */
    T &create(const std::string &args = "");
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

template <class T>
inline IgPythonFactory<T>::IgPythonFactory (IgPythonInterpreter *interpreter)
    :m_interpreter(interpreter),
     m_className("")
{
}

template <class T>
inline void IgPythonFactory<T>::loadScript(const char *scriptName)
{
    m_interpreter->interpret((std::string("from string import split\nfrom os import getenv\nfrom os.path import exists\nt_pythonPathSplitted=split(getenv('PYTHONPATH'),':')\nfilename='")+scriptName+".py'\nfor f in t_pythonPathSplitted:\n\tpythonModuleName=f+'/'+filename\n\tif exists(pythonModuleName)==1:\n\t\tbreak\nif exists(pythonModuleName) == 0:\n\tpythonModuleName=''\n").c_str());
    std::string filename =  (extract<std::string>(m_interpreter->getGlobalNamespace()["pythonModuleName"]))().c_str();
    if (filename != "")
    {	
	m_interpreter->execute(filename.c_str());
    }else
    {
	std::cerr << "Cannot load module:" << scriptName << ".py" << std::endl;	
    }    
}

template <class T>
inline void IgPythonFactory<T>::setScript(const char *script)
{
    m_interpreter->interpret(script);
}

//FIXME: not thread safe!
template <class T>
inline void IgPythonFactory<T>::set(const char *className)
{
    m_className = className;
}

template <class T>
inline T &IgPythonFactory<T>::create(const std::string &args /* ="" */)
{
    m_interpreter->interpret(("__newDerived__ = "
			      + m_className 
			      +"(" + args + ")\n").c_str());
    return extract<T&>(m_interpreter->getGlobalNamespace()["__newDerived__"])();
    //    object *obj = m_interpreter->eval((m_className+"()").c_str());    
    //return extract<T&>(*obj);    
}

#endif // IG_PYTHON_IG_PYTHON_FACTORY_H
