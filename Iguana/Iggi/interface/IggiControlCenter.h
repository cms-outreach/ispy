#ifndef IGGI_IGGI_CONTROL_CENTER_H
# define IGGI_IGGI_CONTROL_CENTER_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "ui_IggiControlCenter.h"
# include "IggiItem.h"
# include "IggiController.h"
# include <vector>
# include <QTabWidget>
# include <QCloseEvent>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class IggiControlCenter : public QTabWidget
{
	Q_OBJECT
public:
	IggiControlCenter (QWidget* parent = 0);
	virtual ~IggiControlCenter () {};

	void setControlledItem (unsigned widgetIndex, IggiItem* item);
	
protected:
	virtual void closeEvent (QCloseEvent *event);
	
private:
	void addController (IggiController* controller);

	std::vector<IggiController*> m_controllerList;
	Ui::IggiControlCenter ui;
	
signals:
	void isShown (bool);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // IGGI_IGGI_CONTROL_CENTER_H
