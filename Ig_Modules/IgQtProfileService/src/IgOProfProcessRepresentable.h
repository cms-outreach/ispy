#ifndef IG_QT_PROFILE_SERVICE_IG_OPROF_PROCESS_REPRESENTABLE_H
# define IG_QT_PROFILE_SERVICE_IG_OPROF_PROCESS_REPRESENTABLE_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgQtProfileService/interface/config.h"
# include <classlib/iobase/IOSelectEvent.h>
# include <classlib/utils/Regexp.h>

# include <string>
# include <sstream>
# include <map>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgOProfConfiguration;
class IgOProfSymbolRepresentable;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_QT_PROFILE_SERVICE_API IgOProfProcessRepresentable
{
public:    
    typedef std::map<std::string,IgOProfSymbolRepresentable*> SymbolMap;
    typedef SymbolMap::iterator SymbolIterator;


    IgOProfProcessRepresentable (const std::string &name);
    void requestUpdateSymbolList (void);
    bool updateSymbolList (lat::IOSelectEvent *event);

    SymbolMap::iterator begin (void);
    SymbolMap::iterator end (void);
    SymbolMap::iterator find (const char *key);
    SymbolMap::iterator find (std::string &key);
    IgOProfSymbolRepresentable *getSymbol (const char *name);        

    std::string name (void);
    int 	hits (void);
    int 	offset (void);
    bool 	isExpanded (void);
    bool	isReading (void);
    bool	upToDate (void);
    void 	setHits (int hits);
    void 	setOffset (int offset);
    void	setIsExpanded (bool expanded);
    void	setUpToDate (int upToDate);
private:    
    SymbolMap	 		m_symbolMap;
    IgOProfConfiguration 	*m_configuration;
    lat::Regexp 		*m_profileInfosRE;    
    std::string 		*m_bufferString;
    int 			m_bufferPosition;    

    std::string 		m_name;
    int 			m_hits;
    int				m_offset;
    bool 			m_isExpanded;
    char 			*m_readBuffer;    
    
    bool 			m_isReading;    
    std::map<int,std::string> 	m_itoaMap;        
    bool 			m_upToDate;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_QT_PROFILE_SERVICE_IG_OPROF_PROCESS_REPRESENTABLE_H
