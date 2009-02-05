#ifndef __IG_OPROF_MAIN_LIST_VIEW_H__
#define __IG_OPROF_MAIN_LIST_VIEW_H__

#include "Ig_Modules/IgQtProfileService/src/IgOProfPopupMenu.h"
#include "Ig_Modules/IgQtProfileService/src/IgOProfConfiguration.h"

#include <qlistview.h>

#include <map>
#include <list>

class QListView;
class QListViewItem;
class QPoint;
class QWidget;
class QString;
class QTimer;

class IgOProfConfiguration;
class IgOProfPopupMenu;
class IgOProfProcessListViewItem;
class IgOProfRepresentableInterface;

class IgOProfMainListView : public QListView
{
    Q_OBJECT
private:
    IgOProfProcessListViewItem *expanded_item;
    IgOProfConfiguration *m_configuration;
    IgOProfPopupMenu PopupMenu;
    std::map <QString,int> OffsetMap;
    std::list<QString> FilterString;	
    IgOProfRepresentableInterface *m_representable;    
    QTimer *m_timer;
    bool m_outOfDate;    
public slots:

    void timeProcessExited(void);
    void itemExpanded(QListViewItem *item);
    void itemCollapsed(QListViewItem *item);
    void update(void);
    void requestPopupMenu(QListViewItem *item, const QPoint &p, int column);
    void saveSession(void);
    void loadSession(void);
    void timedUpdate(void);    
public:
    bool NoPath;
    
    IgOProfMainListView(QWidget *w);
    
    bool showLibraries(void){return m_configuration->m_showLibraries;}
    bool showZeros(void){return m_configuration->m_showZeros;}
    bool showPercernt(void){return m_configuration->m_showPercent;}
    void toggleShowLibraries(void){m_configuration->m_showLibraries = !m_configuration->m_showLibraries;update();}
    void toggleShowZeros(void){m_configuration->m_showZeros = !m_configuration->m_showZeros;update();}
    void toggleShowPercent(void){m_configuration->m_showPercent = !m_configuration->m_showPercent; update();}
    void setCounter(int i){m_configuration->m_currentCounter = i; update();}
};

#endif
