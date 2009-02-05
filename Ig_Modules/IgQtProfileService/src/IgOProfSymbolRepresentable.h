#ifndef IG_QT_PROFILE_SERVICE_IG_OPROF_SYMBOL_REPRESENTABLE_H
# define IG_QT_PROFILE_SERVICE_IG_OPROF_SYMBOL_REPRESENTABLE_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgQtProfileService/interface/config.h"
# include <string>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_QT_PROFILE_SERVICE_API IgOProfSymbolRepresentable
{
public:
    std::string name (void);
    std::string library (void);
    int		offset (void);
    int		hits (void);

    void	setOffset (int offset);
    void	setHits (int hits);
    
    IgOProfSymbolRepresentable(std::string name,
			       std::string library = std::string (""));    
private:
    std::string m_name;
    std::string m_library;
    int m_hits;
    int m_offset;
};


//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_QT_PROFILE_SERVICE_IG_OPROF_SYMBOL_REPRESENTABLE_H
