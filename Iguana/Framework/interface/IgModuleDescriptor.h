#ifndef IGUANA_FRAMEWORK_IG_MODULE_DESCRIPTOR_H
# define IGUANA_FRAMEWORK_IG_MODULE_DESCRIPTOR_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Framework/interface/config.h"
# include <vector>
# include <string>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Cache data representation: a tree of vectors of strings.  */
class IgModuleDescriptor
{
public:
    IgModuleDescriptor (IgModuleDescriptor *parent = 0,
		        const std::string &t1 = "",
		        const std::string &t2 = "",
		        const std::string &t3 = "",
		        const std::string &t4 = "",
		        const std::string &t5 = "",
		        const std::string &t6 = "",
		        const std::string &t7 = "",
		        const std::string &t8 = "");
    IgModuleDescriptor (IgModuleDescriptor *parent,
		        const std::vector<std::string> &tokens);
    ~IgModuleDescriptor (void);
    // implicit copy constructor
    // implicit assignment operator

    IgModuleDescriptor *		parent (void) const;
    unsigned				children (void) const;
    IgModuleDescriptor *		child (unsigned index) const;
    void				addChild (IgModuleDescriptor *child);

    unsigned				tokens (void) const;
    const std::string &			token (unsigned index) const;
    void				addToken (const std::string &text);

    void				dup (IgModuleDescriptor *to) const;

private:
    IgModuleDescriptor			*m_parent;	//< Up in the tree.
    std::vector<IgModuleDescriptor *>	m_children;	//< Down in the tree.
    std::vector<std::string>		m_tokens;	//< Tokens.
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGUANA_FRAMEWORK_IG_MODULE_DESCRIPTOR_H
