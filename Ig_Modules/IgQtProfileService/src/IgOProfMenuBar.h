#ifndef __IG_OPROF_MENU_BAR_H__
#define __IG_OPROF_MENU_BAR_H__

#include <qobject.h>

#define CONF_SHOW_ZEROS 1
#define CONF_SHOW_LIBRARIES 2
#define CONF_SHOW_PERCENT 3
#define CONF_SEPARATE_SAMPLES 4
#define CONF_OFFSET_ON_START 5
#define CONF_SORT_ON_TOTAL 6
#define SESSION_LOAD 1
#define SESSION_SAVE 2
#define SESSION_HIDE 3

class QPopupMenu;
class IgOProfMainListView;
class IgOProfConfiguration;
class QWidget;
class QMenuBar;


class IgOProfMenuBar : public QObject
{
    Q_OBJECT
private:
    QMenuBar	*m_menuBar;
    QPopupMenu 	*m_sessionMenu;    
    QPopupMenu 	*m_counterMenu;
    QPopupMenu 	*m_configurationMenu;
    QPopupMenu 	*m_aboutMenu;
    IgOProfMainListView *m_listView;
    IgOProfConfiguration *m_config;
    
    void toggleBool(int n, bool *option);
    
public slots:
void slotConfigurationMenu(int item);
    void slotCounterMenu(int counter);
    void slotAboutMenu(int counter);
    void slotSessionMenu(int n);
public:
    IgOProfMenuBar(QMenuBar *menuBar,IgOProfMainListView *listView, IgOProfConfiguration *config, bool inDialog);
};	

#endif
