#include "Ig_Modules/IgQtProfileService/src/IgOProfProcessListViewItem.h"
#include "Ig_Modules/IgQtProfileService/src/IgOProfSymbolListViewItem.h"
#include "Ig_Modules/IgQtProfileService/src/IgOProfHitsString.h"
#include "Ig_Modules/IgPython/interface/IgPythonInterpreter.h"
#include "Ig_Modules/IgQtProfileService/src/demangle_symbol.h"

IgOProfSymbolListViewItem::IgOProfSymbolListViewItem(IgOProfProcessListViewItem *parent, QString &mangledName, QString &library, int hits, int offsetHits, bool percent)
:QListViewItem((QListViewItem *)parent),
m_mangledName(mangledName),
m_library(library),	
m_hits(hits),
m_offsetHits(offsetHits)
{    
    if(percent == true) setText(0,(QString)IgOProfHitsString(getHits(),getOffsetHits(),parent->getHits(),parent->getOffsetHits()));
    else setText(0,(QString)IgOProfHitsString(getHits(),getOffsetHits()));
    setText(1,demangle_symbol(m_mangledName));
    setText(2,m_library);
}	

int 
IgOProfSymbolListViewItem::compare(QListViewItem *i,int col,bool ascending ) const
{
    IgOProfSymbolListViewItem *other = (IgOProfSymbolListViewItem *) i;
    switch( col )
    {
    default:	
	return key( col, ascending ).compare( other->key(col, ascending) );
	break;
    case 0:
	if(other->m_hits-other->m_offsetHits > m_hits-m_offsetHits)return 1;
	else return -1;
	break;
    }
}	

int 
IgOProfSymbolListViewItem::getHits(void)
{
    return m_hits;
}

int 
IgOProfSymbolListViewItem::getOffsetHits(void)
{
    return m_offsetHits;
}

QString 
IgOProfSymbolListViewItem::getName(void)
{
    return text(1);
}

QString 
IgOProfSymbolListViewItem::getMangledName(void)
{
    return m_mangledName;
}

QString 
IgOProfSymbolListViewItem::getLibrary(void)
{
    return m_library;
}

QDomElement
IgOProfSymbolListViewItem::saveToFile(QDomDocument *doc)
{
    QDomElement t = doc->createElement("SYMBOL");
    t.setAttribute("NAME",this->text(0));
    t.setAttribute("HITS",this->text(1));
    return t;
}	
