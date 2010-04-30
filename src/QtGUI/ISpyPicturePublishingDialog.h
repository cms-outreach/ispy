#ifndef QT_GUI_ISPY_PICTURE_PUBLISHING_DIALOG_H
# define QT_GUI_ISPY_PICTURE_PUBLISHING_DIALOG_H

# include "ui/ui_ISpyPicturePublishingDialog.h"

class ISpyPicturePublishingDialog : public QDialog, private Ui::ISpyPicturePublishingDialog
{
  Q_OBJECT
public:
  ISpyPicturePublishingDialog (QWidget *parent = 0);
  
  QString &	dir(void);
  QStringList & views(void);
  int		delay(void);
  bool		isAuto(void);

signals:
  void		autoPrint(bool);
  
private slots:
  void 		setDir(QString name);
  void		setViews(QStringList views);
  void		setDelay(QString sec);
  void		setAuto(int state);
  void 		verifyDir(void);
  void 		verifyViews(void);
  void 		restore(void);
  
private:
  void		setDelay(int msec);
  
  // Auto-printing configuration
  bool			m_autoPrint;
  QStringList		m_viewsToPrint;
  QString		m_printDir;
  int			m_printDelay;
};

#endif // QT_GUI_ISPY_PICTURE_PUBLISHING_DIALOG_H
