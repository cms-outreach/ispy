#ifndef IGUANA_FRAMEWORK_IG_ENVS_ELEMENT_H
# define IGUANA_FRAMEWORK_IG_ENVS_ELEMENT_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Framework/interface/IgStateElement.h"
# include <vector>
# include <string>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IGUANA_FRAMEWORK_API IgEnvsElement : public IgStateElement
{
    IG_DECLARE_STATE_ELEMENT (IgEnvsElement);
public:
    IgEnvsElement (IgState* state);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    
    bool	getEnv(const std::string& name, std::string& value) const;
    bool	getEnv(const std::string& name, std::vector<std::string>& value,
                    char separator = ':') const;
    bool	getEnv(const std::string& name, int& value) const;
    bool	getEnv(const std::string& name, long& value) const;
    bool	getEnv(const std::string& name, unsigned int& value) const;
    bool	getEnv(const std::string& name, unsigned long& value) const;
    bool	getEnv(const std::string& name, float& value) const;
    bool	getEnv(const std::string& name, double& value) const;
    
private:
    IgState	*m_state;	//< State object.
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_FRAMEWORK_IG_ENVS_ELEMENT_H
