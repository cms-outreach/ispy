#ifndef IG_CONFIG_IG_CONFIG_ELEMENT_H
# define IG_CONFIG_IG_CONFIG_ELEMENT_H

//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgConfig/interface/config.h"
#include "Ig_Framework/IgObjectBrowser/interface/IgExtension.h"
#include <map>
#include <string>
#include <sstream>
#include <vector>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class QDomNode;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_CONFIG_API IgConfigElement : public IgExtension
{
    IG_DECLARE_STATE_ELEMENT (IgConfigElement);
public:
    typedef std::map<std::string, std::string> KeyMap;
    IgConfigElement (IgState* state);
    ~IgConfigElement ();

    void 	save(const std::string& filename = "");
    template <class T>
    void	putValue(const std::string& key, const T& value,
    			 bool override = true);
    void	putValue(const std::string& key, const std::string& value,
    			 bool override  = true);
    template <class T>
    void	putValues(const std::string& key, 
    			 const std::vector<T>& values,
    			 bool override = true);
    void	putValues(const std::string& key,
    			 const std::vector<std::string>& values,
    			 bool override = true);
    template <class T>
    bool 	getValue(const std::string& key, T& value) const;
    bool 	getValue(const std::string& key, std::string& value) const;

    template <class T>
    bool 	getValues(const std::string& key,
    			  std::vector<T>& values) const;
    bool 	getValues(const std::string& key,
    			  std::vector<std::string>& values) const;

    void		setPrefix (const std::string& prefix);
    const std::string& 	prefix (void) const;
    
    const KeyMap&	getKeyMap () const;

private:
    template<class T>
    void		convert(const std::string& str, T& value) const;
    void		convert(const std::string& str, bool& value) const;

    template<class T>
    std::string		convert(const T& value) const;

    void		initConfiguration ();
    void		processNode (const QDomNode& node,
    				     const std::string& path);
    const std::string&	substituteValue(std::string& value) const;

    IgState*				m_state;
    KeyMap				m_config;
    std::string				m_prefix;
    std::string				m_file;
    bool				m_dirty;
};

template<class T>
void IgConfigElement::convert(const std::string& str, T& value) const
{
    std::stringstream ios;
    int p = ios.precision();
    ios.setf(std::ios::fixed);
    ios.precision(20);
    ios << str.c_str();
    ios >> value;
    ios.precision(p);
}

template<class T>
std::string IgConfigElement::convert(const T& value) const
{
    std::string converted;
    std::stringstream ios;
    int p = ios.precision();
    ios.setf(std::ios::fixed);
    ios.precision(20);
    ios << value;
    ios >> converted;
    ios.precision(p);
    return converted;
}


template<class T>
bool IgConfigElement::getValue(const std::string& key, T& value) const
{
    std::string val;
    if (getValue(key, val))
    {
        convert (val, value);
        return true;
    }
    return false;
}

template <class T>
bool
IgConfigElement::getValues(const std::string& key,
    			      std::vector<T>& values) const
{
    std::vector<std::string> val;
    if (getValues(key, val))
    {
        T val1;
	for(size_t i=0; i<val.size(); i++)
	{
	    convert(val[i], val1);
	    values.push_back(val1);
	}
	return true;
    }
    return false;
}

template <class T>
void
IgConfigElement::putValue(const std::string& key,
			  const T& value,
			  bool override /* = true */)
{ putValue (key, convert(value), override); }

template <class T>
void
IgConfigElement::putValues(const std::string& key,
			  const std::vector<T>& values,
			  bool override /* = true */)
{
    std::vector<std::string> vec;
    for(size_t i=0; i<values.size(); i++)
        vec.push_back(convert(values[i]));
    putValues (key, vec, override);
}


//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_CONFIG_IG_CONFIG_ELEMENT_H
