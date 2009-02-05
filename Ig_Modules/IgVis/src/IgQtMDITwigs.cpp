#include "Ig_Modules/IgVis/interface/IgQtMDITwigs.h"

#include "qapplication.h"
#include "qmainwindow.h"
#include "qworkspace.h"
#include "qmenubar.h"
#include "qvbox.h"
#include "qframe.h"
#include "qpushbutton.h"
#include "qstatusbar.h"
#include "qfiledialog.h"

const QString IgQtMDITwigs::mdiTwigsName = QString("IgQtMDITwigs");
int IgQtMDITwigs::serialNo = 0;

IgQtMDITwigs::IgQtMDITwigs(QWidget *parent, QString title) :
QHBox(parent,title,0) 
{

	QString sn;
	sn.setNum(serialNo++);
	QString uniqueName = mdiTwigsName+'_'+sn;
	setName(uniqueName);

	if (title) 
	{
		setCaption(title);
	}
}
IgQtMDITwigs::~IgQtMDITwigs()
{
}
