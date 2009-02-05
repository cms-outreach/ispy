#ifndef __IG_OPROF_PROCESS_LIST_VIEW_ITEM_H__
#define __IG_OPROF_PROCESS_LIST_VIEW_ITEM_H__

#include <qlistview.h>
#include <qstring.h>
#include <qdom.h>
#include <qcolor.h>
#include <string>

class QDomDocument;
class QColor;

class IgOProfProcessListViewItem : public QListViewItem
{
private:
    int m_hits;
    int m_offsetHits;
    QString m_name;
    QColor m_color;
    
    void setName(QString name);
    void setHits(int hits);
    void setOffsetHits(int offsetHits);    
public:
    IgOProfProcessListViewItem(QListView *parent, QString &name, int hits, int offsetHits);
    int compare(QListViewItem *i,int col,bool ascending ) const;
    int getHits(void) const;
    int getOffsetHits(void) const;
    QString getName(void);
    
    std::string getDisplayedName(void);
    void setDisplayedName(std::string displayedName);

    std::string getDisplayedHits(void);
    void setDisplayedHits(std::string displayedHits);

    /** Get/sets the color of the item
     */
    void setColor(std::string color);
    std::string getColor(void);
    
    virtual void paintCell(QPainter * p, const QColorGroup & cg, int column, int width, int align);
    
    /** This function is called after setup of the so that one can do post
	processing on the item. This is also exported in python, so that one can write the
	post processing routine in python as well.
     */
    virtual void postProcess(void);

    QDomElement saveToFile(QDomDocument *doc);
};

#endif
