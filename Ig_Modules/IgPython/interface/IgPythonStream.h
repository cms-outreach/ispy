#ifndef IG_PYTHON_IG_PYTHON_STREAM_H
# define IG_PYTHON_IG_PYTHON_STREAM_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgPython/interface/config.h"
# include "Ig_Modules/IgPython/interface/IgPythonInterpreter.h"
# include <ostream>
# include <streambuf>
# include <sstream>
# include <iostream>
# include <boost/python.hpp>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/**
   \brief A std::streambuf which on sync() passes the buffer to the python interpreter. 
 */

class IG_PYTHON_API IgPythonStreambuf : public std::stringbuf
{
private:
    IgPythonInterpreter *m_pythonInterpreter;
public:
    IgPythonStreambuf (void);
    
    /**Reimplements std::stringbuf sync() method so that the buffer is
       passed to python interpreter on call.
     */
    virtual int sync (void);
    
    /**\return an instance to the internal python interpreter.
     */
    IgPythonInterpreter *interpreter ()
	{
	    return m_pythonInterpreter;
	}
    
    /**Redirects python interpreter errors to provided stream:
       \param stream the stream to redirect errorts to.
     */
    void redirectError (std::ostream &stream);

    /**Redirects python interpreter output to provided stream:
       \param stream the stream to redirect errorts to.
    */
    void redirectOutput (std::ostream &stream);    

    /**Completes passed object name according to the internal python
       dictionary.

       \param objName the name to be completed.
       \return a string containing the completed name.
     */
    std::string completeName (const char *objName);
};

/** This wraps the IgPythonInterpreter so that the std::ostream API
    can be used to access it.  The user can stream scripts to it,
    either from string, const char * or files, using << operator.  

    The >> operator can be used to extract variables from the python
    environment.
 */

class IG_PYTHON_API IgPythonStream : public std::ostream
{
public:
    IgPythonStream (void);

    /**sets which one is the current object*/
    IgPythonStream &operator[](const char *objectName);    

    /**Template operator which automatically performs extraction of the current object from the python
       environment.
     */
    template <class T>
    IgPythonStream &operator>>(T &t)
	{
	    this->flush();	    
	    t = boost::python::extract<T>(m_streambuf.interpreter()->getGlobalNamespace()[m_currentObjectName])();
	    return *this;	    
	}	

    /**This specialization permits to set the name of the variable to be extracted by the following >> operation.
     */
    IgPythonStream &operator>>(const char *objectName)
	{
	    m_currentObjectName = objectName;
	    return *this;	    
	}
    
    /**Redirects python interpreter errors to provided stream:
       \param stream the stream to redirect errorts to.
     */
    void redirectError(std::ostream &stream);

    /**Redirects python interpreter output to provided stream:
       \param stream the stream to redirect errorts to.
    */
    void redirectOutput(std::ostream &stream);

    /**Completes passed object name according to the internal python dictionary.
       \param objName the name to be completed.
       \return a string containing the completed name.
     */
    std::string completeName(const char *objName);    
    
    /**\return underlying IgPythonInterpreter object.
     */
    IgPythonInterpreter *interpreter (void)
	{
	    return m_streambuf.interpreter ();	    
	}
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

private:
    std::string m_currentObjectName;
    IgPythonStreambuf m_streambuf;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_PYTHON_IG_PYTHON_STREAM_H
