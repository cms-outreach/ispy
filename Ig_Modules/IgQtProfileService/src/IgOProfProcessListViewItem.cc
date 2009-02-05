#include "Ig_Modules/IgQtProfileService/src/IgOProfProcessListViewItem.h"
#include "Ig_Modules/IgQtProfileService/src/IgOProfSymbolListViewItem.h"
#include <qdom.h>
#include <qdatetime.h>
#include <qcolor.h>

#include <string>
#include <iostream>

#include "Ig_Modules/IgPython/interface/IgPythonInterpreter.h"

using namespace std;

IgOProfProcessListViewItem::IgOProfProcessListViewItem(QListView *parent, QString &name, int hits, int offsetHits)
:QListViewItem(parent),
 m_hits(hits),
 m_offsetHits(offsetHits),
 m_name(name),
 m_color()
{
    /* This part has been removed because now I want to use 
       the callback mechanism to python scripting.
       
    IgPythonInterpreterService interpreter;
    dict currentItem;
    currentItem["hits"]=hits;
    currentItem["offsetHits"]=offsetHits;
    currentItem["displayedHits"]=std::string("");
    currentItem["displayedName"]=name.ascii();
    currentItem["name"]=std::string(name.ascii());
    currentItem["color"]=0x000000;
    
    interpreter.getGlobalNamespace()["currentItem"] = currentItem;
    
    interpreter.execute("share/ProcessItemDecorator.py");
    
    dict processedItem = extract<dict>(interpreter.getGlobalNamespace()["currentItem"])();    
    
    cerr << extract<std::string>(processedItem["displayedHits"])().c_str()<< endl;
    
    setText(0, extract<std::string>(processedItem["displayedHits"])().c_str());
    setName(extract<std::string>(processedItem["displayedName"])().c_str());    
    */
    postProcess();    
}

int 
IgOProfProcessListViewItem::compare(QListViewItem *i,int col,bool ascending ) const
{
    IgOProfProcessListViewItem *other = (IgOProfProcessListViewItem *) i;
    switch( col )
    {
    default:	
	return key( col, ascending ).compare( other->key(col, ascending) );
	break;
    case 0:
	if(other->getHits()-other->getOffsetHits() > getHits()-getOffsetHits()) return 1;
	else return -1;
	break;
    }
}

int 
IgOProfProcessListViewItem::getOffsetHits(void) const 
{
    return m_offsetHits;
}

void 
IgOProfProcessListViewItem::setOffsetHits(int offsetHits)
{
    m_offsetHits=offsetHits;
}
int 
IgOProfProcessListViewItem::getHits(void) const 
{
    return m_hits;
}
void 
IgOProfProcessListViewItem::setHits(int hits)
{
    m_hits = hits;
}

std::string 
IgOProfProcessListViewItem::getDisplayedName(void)
{
    return text(1).ascii();
}

std::string
IgOProfProcessListViewItem::getDisplayedHits(void)
{
    return text(0).ascii();
}

void
IgOProfProcessListViewItem::setDisplayedHits(std::string hits)
{
    setText(0,hits.c_str());
}

void
IgOProfProcessListViewItem::setDisplayedName(std::string name)
{
    setText(1, name.c_str());
}

void
IgOProfProcessListViewItem::setColor(std::string color)
{
    m_color.setNamedColor(color.c_str());
}

std::string
IgOProfProcessListViewItem::getColor(void)
{
    return std::string(m_color.name().ascii());
}

void
IgOProfProcessListViewItem::paintCell( QPainter *p, const QColorGroup &cg, int column, int width, int alignment )
{
    QColorGroup _cg( cg );
    _cg.setColor( QColorGroup::Text, m_color );
    QListViewItem::paintCell( p, _cg, column, width, alignment );
}

QString
IgOProfProcessListViewItem::getName(void)
{
    return this->text(1);
}

void
IgOProfProcessListViewItem::setName(QString name)
{
    setText(1,name);
}

void
IgOProfProcessListViewItem::postProcess(void)
{
    QString hitsString = QString::number(getHits()-getOffsetHits(),10);
    if( getOffsetHits() != 0)
    {
	hitsString += " ("+QString::number(getHits(),10)+")";
    }
    setDisplayedHits(hitsString.ascii());
    setDisplayedName(m_name.ascii());
}

QDomElement
IgOProfProcessListViewItem::saveToFile(QDomDocument *doc)
{
    QDomElement Main = doc->createElement("PROCESS");
    Main.setAttribute("NAME",getName());
    Main.setAttribute("HITS",QString::number(getHits()));
    Main.setAttribute("DATE",QDate::currentDate().toString("dd.MM.yyyy"));
    
    for(IgOProfSymbolListViewItem *i = (IgOProfSymbolListViewItem *) firstChild(); i!=NULL ; i = (IgOProfSymbolListViewItem *)i->nextSibling())
    {
	Main.appendChild(i->saveToFile(doc));
    }
    return Main;
}
