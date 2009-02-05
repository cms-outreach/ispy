//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgRSS/interface/IgRSSCreator.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgRSSCreator::IgRSSCreator (void)
{
}

void
IgRSSCreator::author (const std::string &author)
{
    m_author = author;    
}

void
IgRSSCreator::channel (const std::string &channel)
{
    m_channel = channel;    
}

void
IgRSSCreator::title (const std::string &title)
{
    m_title = title;    
}

void
IgRSSCreator::link (const std::string &url)
{
    m_link = url;    
}

void
IgRSSCreator::description (const std::string &text)
{
    m_description = text;    
}


void
IgRSSCreator::addItem (const std::string &rssItem)
{
    m_items.push_back (rssItem);    
}

std::string
IgRSSCreator::text (void) const
{
    std::string result = "<rss version=\"2.0\">"
			 "<channel>"
			 "<title>" + m_title + "</title>"
			 "<link>" + m_link + "</link>"
			 "<description>" + m_description + "</description> ";
    
    for (ItemList::const_iterator i = m_items.begin ();
	 i != m_items.end ();
	 i++)
    {
	result += (*i);	
    }
    
    result += "</channel>"
	      "</rss>";    
    return result;    
}
