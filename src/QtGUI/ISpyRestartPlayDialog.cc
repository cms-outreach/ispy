#include "QtGUI/ISpyRestartPlayDialog.h"
#include <QtGui>

ISpyRestartPlayDialog::ISpyRestartPlayDialog (QWidget *parent)
  : QDialog(parent),
    m_count(60)
{
  setupUi(this);

  QTimer *timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), this, SLOT(showTime()));
  timer->start(1000);
  showTime();
}

void
ISpyRestartPlayDialog::showTime(void)
{
  QTime time(0,0,0);
  QTime newTime = time.addSecs(m_count);
  QString text = newTime.toString("mm:ss");
  if ((newTime.second() % 2) == 0)
    text[2] = ' ';
  lcdNumber->display(text);
  if (m_count == 0)
    done (1);
  else
    m_count--;
}
