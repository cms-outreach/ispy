#ifndef IGGI_IGGI_CONTROL_CENTER_H
# define IGGI_IGGI_CONTROL_CENTER_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "ui_ISpyControlCenter.h"
# include "ISpyItem.h"
# include "ISpyController.h"
# include <vector>
# include <QTabWidget>
# include <QCloseEvent>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class ISpyControlCenter : public QTabWidget
{
	Q_OBJECT
public:
	ISpyControlCenter (QWidget* parent = 0);
	virtual ~ISpyControlCenter () {};

	void setControlledItem (unsigned widgetIndex, ISpyItem* item);
	
protected:
	virtual void closeEvent (QCloseEvent *event);
	
private:
	void addController (ISpyController* controller);

	std::vector<ISpyController*> m_controllerList;
	Ui::ISpyControlCenter ui;
	
signals:
	void isShown (bool);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGGI_IGGI_CONTROL_CENTER_H
