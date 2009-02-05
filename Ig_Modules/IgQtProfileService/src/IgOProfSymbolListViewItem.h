#ifndef __IG_OPROF_SYMBOL_LIST_VIEW_ITEM_H__
#define __IG_OPROF_SYMBOL_LIST_VIEW_ITEM_H__

#include "Ig_Modules/IgQtProfileService/src/IgOProfProcessListViewItem.h"
#include <qlistview.h>
#include <qstring.h>
#include <qdom.h>

class IgOProfProcessListViewItem;


class IgOProfSymbolListViewItem : public QListViewItem
{
private:
    QString m_mangledName;
    QString m_library;
    int m_hits;
    int m_offsetHits;
public:
    IgOProfSymbolListViewItem(IgOProfProcessListViewItem *parent, QString &name, QString &library, int hits, int offsetHits, bool percent);
    int compare(QListViewItem *i,int col,bool ascending ) const;
    int getHits(void);
    int getOffsetHits(void);
    QString getName(void);
    QString getMangledName(void);
    QString getLibrary(void);
    
    virtual QDomElement saveToFile(QDomDocument *doc);
};	

#endif
