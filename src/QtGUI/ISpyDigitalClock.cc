#include "QtGUI/ISpyDigitalClock.h"
#include <QtGui>

ISpyDigitalClock::ISpyDigitalClock(QWidget *parent)
  : QLCDNumber(parent),
    m_count(0)
{
  setSegmentStyle(QLCDNumber::Outline);

  QTimer *timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), this, SLOT(showTime()));
  timer->start(1000);
  showTime();
}

void
ISpyDigitalClock::showTime(void)
{
  QTime time(0,0,0);
  QTime newTime = time.addSecs(m_count);
  QString text = newTime.toString("mm:ss");
  if ((newTime.second() % 2) == 0)
    text[2] = ' ';
  display(text);
  ++m_count;
}

void
ISpyDigitalClock::resetTime(void)
{
  m_count = 0;
}

