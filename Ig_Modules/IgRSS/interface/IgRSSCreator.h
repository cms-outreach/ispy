#ifndef IG_RSS_IG_RSS_CREATOR_H
# define IG_RSS_IG_RSS_CREATOR_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Ig_Modules/IgRSS/interface/config.h"
# include <string>
# include <list>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IG_RSS_API IgRSSCreator
{
public:
    IgRSSCreator (void);
    void author (const std::string &author);
    void title (const std::string &title);
    void channel (const std::string &channel);
    void link (const std::string &url);
    void description (const std::string &text);
    
    void addItem (const std::string &rssItem);

    std::string text (void) const;
    
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor
private:
    std::string 	m_author;
    std::string		m_channel;
    std::string 	m_title;
    std::string 	m_link;
    std::string 	m_description;
    
    typedef std::list <std::string> ItemList;
    ItemList	     	m_items;
    
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IG_RSS_IG_RSS_CREATOR_H
