#ifndef IGUANA_FRAMEWORK_IG_ARGS_ELEMENT_H
# define IGUANA_FRAMEWORK_IG_ARGS_ELEMENT_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Framework/interface/IgStateElement.h"
# include <vector>
# include <string>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgState;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IGUANA_FRAMEWORK_API IgArgsElement : public IgStateElement
{
    IG_DECLARE_STATE_ELEMENT (IgArgsElement);
public:
    IgArgsElement (IgState *in, int argc, char **argv);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
    
    virtual int &		args (void);
    virtual char *		arg (int arg);
    virtual char **		argv (void);
    virtual bool 		find (const std::vector<std::string> &arg,
    				      std::string &value,
				      bool remove = false);
    virtual bool 		find (const std::string &arg,
    				      std::string &value,
				      bool remove = false);
    virtual bool                find (const std::string &arg,
                                      int &value,
                                      bool remove = false);
    virtual bool		exists (const std::vector<std::string> &arg,
    				        bool hasvalue = false);
    virtual bool		exists (const std::string &arg,
    				        bool hasvalue = false);
    virtual void		remove (const std::vector<std::string> &arg,
    				        bool hasvalue = false);
    virtual void		remove (const std::string &arg,
    				        bool hasvalue = false);

private:
    IgState			*m_state;	//< State object.
    int				m_argc;		//< Program arguments.
    char			**m_argv;	//< Program arguments.
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_FRAMEWORK_IG_ARGS_ELEMENT_H
