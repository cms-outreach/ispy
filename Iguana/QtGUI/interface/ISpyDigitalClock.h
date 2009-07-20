#ifndef QT_GUI_ISPY_DIGITAL_CLOCK_H
# define QT_GUI_ISPY_DIGITAL_CLOCK_H

# include <QLCDNumber>

class ISpyDigitalClock : public QLCDNumber
{
  Q_OBJECT

public:
  ISpyDigitalClock(QWidget *parent = 0);

public slots:
  void resetTime(void);

private slots:
  void showTime(void);

private:
  int	m_count;
};

#endif // QT_GUI_ISPY_DIGITAL_CLOCK_H
