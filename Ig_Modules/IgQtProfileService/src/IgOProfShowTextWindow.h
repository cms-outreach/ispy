#ifndef __IG_OPROF_SHOW_TEXT_WINDOW__
#define __IG_OPROF_SHOW_TEXT_WINDOW__

#include <qwidget.h>

class QTextEdit;
class QString;
class IgOProfConfiguration;

class IgOProfShowTextWindow : public QWidget
{
private:
    QTextEdit *TextBrowser;
public:
    IgOProfShowTextWindow(IgOProfConfiguration *config,const QString &name);
};

#endif
