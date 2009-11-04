#include "Iguana/QtGUI/src/ISpyPicturePublishingDialog.h"
#include <QDir>
#include <QMessageBox>
#include <iostream>

ISpyPicturePublishingDialog::ISpyPicturePublishingDialog(QWidget *parent)
  : QDialog(parent),
    m_autoPrint(false),
    m_printDelay(20000)
{
  setupUi(this);

  // Check an environment variable which sets
  // which views will be printed.
  //
  // The valid string should contain any from the
  // following: 3D:R-Phi:R-Z:Lego
  //
  // Note: There are no checks on validity of the
  // 	   entered values.
  //
  char *autoViews = getenv("ISPY_AUTO_PRINT_VIEWS");
  if(autoViews != NULL)
  {
    m_viewsToPrint = QString(autoViews).split(":");
  }
  else
    m_viewsToPrint << "3D" << "R-Phi" << "R-Z" << "Lego";
  setViews(m_viewsToPrint);
  
  QValidator *validator = new QIntValidator(1, 999, this);
  lineEditSec->setValidator(validator);
 
  QObject::connect(lineEditDir, SIGNAL(textEdited(QString)), this, SLOT(setDir(QString)));
  QObject::connect(checkBoxAuto, SIGNAL(stateChanged(int)), this, SLOT(setAuto(int)));
  QObject::connect(lineEditSec, SIGNAL(textChanged(QString)), this, SLOT(setDelay(QString)));
  QObject::connect(buttonBox, SIGNAL(accepted()), this, SLOT(verifyDir()));
  QObject::connect(buttonBox, SIGNAL(accepted()), this, SLOT(verifyViews()));
  QObject::connect(buttonBox, SIGNAL(rejected()), this, SLOT(restore()));
  
  // Check an environment variable which sets
  // directory into which the files are stored.
  // If a given directory does not exist, print
  // to a temporary one.
  //
  char *autoPath = getenv("ISPY_AUTO_PRINT_PATH");
  if(autoPath != NULL)
  {
    QDir autoDir(autoPath);
    if(! autoDir.exists())
    {
      QMessageBox::information(parent, "iSpy Auto Print Info",
			       tr("Directory %1 specified by ISPY_AUTO_PRINT_PATH "
				  "does not exist.\n"
				  "Auto-print will use the temporary directory.\n").arg(autoDir.path()));
    }    
    m_printDir = QString(autoPath);
  }
  else
    m_printDir = QDir::tempPath();
  
  lineEditDir->setText(m_printDir);
  
  // Check an environment variable which sets
  // auto-prinitng
  //
  char *autoPrint = getenv("ISPY_AUTO_PRINT");
  if(autoPrint != NULL)
  {
    int num = atoi(autoPrint);
    if(num == 0)
      checkBoxAuto->setCheckState(Qt::Unchecked);
    else
      checkBoxAuto->setCheckState(Qt::Checked);
  }

  // Check an environment variable which sets
  // the auto-print timer interval in sec
  //
  char *autoPrintDelay = getenv("ISPY_AUTO_PRINT_DELAY");
  if(autoPrintDelay != NULL)
  {
    int num = atoi(autoPrintDelay);
    // Set it only if something sensible is given
    if(num >= 1 || num < 999)
      m_printDelay = num * 1000;
  }
  lineEditSec->setText(QString::number(m_printDelay / 1000));
}

void
ISpyPicturePublishingDialog::verifyDir(void)
{
  QDir autoDir(m_printDir);
  if(! autoDir.exists())
  {
    QMessageBox::information(this, "iSpy Auto Print Info",
			     tr("Directory %1 "
				"does not exist.\n"
				"Auto-print will use a temporary directory.\n").arg(autoDir.path()));

    m_printDir = QDir::tempPath();
    lineEditDir->setText(m_printDir);
  }
}

void
ISpyPicturePublishingDialog::setDir(QString autoPath)
{
  m_printDir = QString(autoPath);
}

void
ISpyPicturePublishingDialog::verifyViews(void)
{
  QStringList tmpStringList;
  if(checkBox3D->checkState() == Qt::Checked)
    tmpStringList << checkBox3D->text();
  if(checkBoxRPhi->checkState() == Qt::Checked)
    tmpStringList << checkBoxRPhi->text();
  if(checkBoxRZ->checkState() == Qt::Checked)
    tmpStringList << checkBoxRZ->text();
  if(checkBoxLego->checkState() == Qt::Checked)
    tmpStringList << checkBoxLego->text();
  m_viewsToPrint.clear();
  m_viewsToPrint = tmpStringList;
}

void
ISpyPicturePublishingDialog::restore(void)
{
  setViews(m_viewsToPrint);
}

void
ISpyPicturePublishingDialog::setViews(QStringList views)
{
  if(views.contains("3D"))
    checkBox3D->setCheckState(Qt::Checked);
  if(views.contains("R-Phi"))
    checkBoxRPhi->setCheckState(Qt::Checked);
  if(views.contains("R-Z"))
    checkBoxRZ->setCheckState(Qt::Checked);
  if(views.contains("Lego"))
    checkBoxLego->setCheckState(Qt::Checked);
  m_viewsToPrint = views;
}

void
ISpyPicturePublishingDialog::setDelay(QString sec)
{
  // Convert sec to msec
  int msec = sec.toInt() * 1000;
  setDelay(msec);
}

void
ISpyPicturePublishingDialog::setDelay(int msec)
{
  m_printDelay = msec;
}

void
ISpyPicturePublishingDialog::setAuto(int state)
{
  state == Qt::Checked ? m_autoPrint = true : m_autoPrint = false;
}

QString &
ISpyPicturePublishingDialog::dir(void)
{
  return m_printDir;
}

QStringList &
ISpyPicturePublishingDialog::views(void)
{
  return m_viewsToPrint;
}

int
ISpyPicturePublishingDialog::delay(void)
{
  return m_printDelay;
}

bool
ISpyPicturePublishingDialog::isAuto(void)
{
  return m_autoPrint;
}
