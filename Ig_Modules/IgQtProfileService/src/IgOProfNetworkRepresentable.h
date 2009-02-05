#ifndef IG_QT_PROFILE_SERVICE_IG_OPROF_NETWORK_REPRESENTABLE_H
# define IG_QT_PROFILE_SERVICE_IG_OPROF_NETWORK_REPRESENTABLE_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgQtProfileService/src/IgOProfRepresentableInterface.h"

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgOProfConfiguration;
class IgPythonStream;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_QT_PROFILE_SERVICE_API IgOProfNetworkRepresentable : public IgOProfRepresentableInterface
{
private:
    IgOProfConfiguration *m_configuration;
    IgPythonStream *m_pythonStream;    
    std::ostringstream m_script;    
public:
    IgOProfNetworkRepresentable (void);
    void requestUpdateProcessList(void);
    
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_QT_PROFILE_SERVICE_IG_OPROF_NETWORK_REPRESENTABLE_H
