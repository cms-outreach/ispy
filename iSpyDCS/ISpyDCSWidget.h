#ifndef ISPYDCSWIDGET_H
#define ISPYDCSWIDGET_H

#include <QWidget>
#include <QList>
#include "QtGUI/ISpyApplication.h"

class SoQtExaminerViewer;
class QGridLayout;
class Ig3DBaseModel;
class QAction;

class ISpyDCSWidget : public QWidget, ISpyApplication {
public:
	ISpyDCSWidget(QWidget *parent);
	~ISpyDCSWidget();

	int doRun(void);

	void loadModel(const QString& file);
	void loadLayout(const QString& file);
	void loadStyles(const QString& file);
	void setColor(const QString& name, uint r, uint g, uint b);
	void setTransparency(const QString& name, float val);
	void setSolid(const QString& name, bool b);
	void showElement(const QString& name, bool b);

	void showTableWidget(bool b);
	void showTreeWidget(bool b);
	void showMenu(bool b);
	void showToolbar(bool b);
	void showView(int index);
	void zoom(float level);

	void enableRendering(bool b);
	void enableNavigation(bool b);

	Ig3DBaseModel* getModel();
	QStringList getCameraInfo();

	void checkSmoothMoveParams(int& time, int& fps);
	void seekToTargetSmoothly(const SbVec3f& pos, const SbRotation& rot, int time, int fps);
	void seekToTargetSmoothly(QList<QVariant>& cameraTargetSettings, int time, int fps);

protected:
	virtual void updateCollections(void);
	void setupActions(void);
	bool isValidColor(uint r, uint g, uint b);


public slots:
	void enableDisablePicking();

private:
	QGridLayout* layout;
	bool m_showTableWidget;
	bool m_showTreeWidget;
	bool m_showMenu;
	bool m_showToolbar;

	QAction *actionEnableDisablePicking;
};

#endif // ISPYDCSWIDGET_H
