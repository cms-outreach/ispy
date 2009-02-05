#ifndef IG_QT_DEMO_IG_QT_DEMO_H
#define IG_QT_DEMO_IG_QT_DEMO_H

#include <qmainwindow.h>
#include <qstring.h>
#include <vector>

class QTextEdit;
class QListView;
class QVBoxLayout;
class QListViewItem;
class IgState;
class QMainWindow;

class IgQTDemoItem{
    public:    
        IgQTDemoItem(const QString& demofile);
        QString name()const{return m_name;}
        QString directory()const{return m_dir;}
        QString file(int index){
            if((index>=int(m_files.size())) || (index<0))
                return "";
            else
                return m_files[index];
        }
       int fileCount()const{return m_files.size();}       	
       bool status()const{return m_status;}
       void status(bool status=false){m_status=status;}
    private:
         void init(const QString& file);
         QString m_dir;
         QString m_name;
         std::vector<QString> m_files;	 
         bool m_status;
}; 

class IgQTDemo: public QObject
{
    Q_OBJECT

public:
    IgQTDemo(IgState *state, const QString& demofile);
    ~IgQTDemo();

protected:
    void closeEvent( QCloseEvent* );
    
private slots:
    void load( const QString &fileName );
    void demoChanged( QListViewItem *);
    void flistChanged( QListViewItem *);
    void print();

    void about();
    void aboutQt();

private:
    void init();
    void checkDemoItem(const QString& file);
    void terminateChild();
    void startChild();
    
    QMainWindow  *m_mainWindow;
    QPrinter *printer;
    QTextEdit *m_file;
    QListView *m_demo;
    QListView *m_flist;
    QVBoxLayout *m_layout;
    QString m_demofile;
    std::vector<IgQTDemoItem> m_demoitems;
    int m_lastitem;
    int m_pid;
};

#endif //IG_QT_DEMO_IG_QT_DEMO_H
