#ifndef QT_GUI_ISPY_RESTART_PLAY_DIALOG_H
# define QT_GUI_ISPY_RESTART_PLAY_DIALOG_H

# include "ui_ISpyRestartPlayDialog.h"

class ISpyRestartPlayDialog : public QDialog, private Ui::ISpyRestartPlayDialog
{
  Q_OBJECT
public:
  ISpyRestartPlayDialog (QWidget *parent = 0);
  
private slots:
  void	showTime(void);
  
private:
  int	m_count;
};

#endif // QT_GUI_ISPY_RESTART_PLAY_DIALOG_H
