#include "ISpyDCSInterface.h"
#include "ISpyDCSWidget.h"
#include "QtGUI/Ig3DBaseModel.h"

#include <Inventor/Qt/SoQt.h>
#include <iostream>

EWO_PLUGIN( ISpyDCSInterface )

ISpyDCSInterface::ISpyDCSInterface(QWidget* parent): BaseExternWidget(parent) {
	// The widget will be deleted by the QWidget parent
	// Don't do it in destructor
	dcsWidget = new ISpyDCSWidget(parent);

	Ig3DBaseModel* model = dcsWidget->getModel();
	connect(model, SIGNAL(elementSelected(QString)), this, SLOT(elementSelected(QString)));
}

ISpyDCSInterface::~ISpyDCSInterface() {
}

QWidget* ISpyDCSInterface::widget() const {
	return dcsWidget;
}

QStringList ISpyDCSInterface::signalList() const {
	QStringList list;

	// signals exposed to WinCC OA
	list.append("elementSelected(string name)");

	return list;
}

QStringList ISpyDCSInterface::methodList() const {
	QStringList list;

	// methods exposed to WinCC OA
	list.append("void loadModel(string filePath)");
	list.append("void setColor(string elemName, uint r, uint g, uint b)");
	list.append("void setTransparency(string elemName, float val)");
	list.append("void setSolid(string elemName, bool b)");
	list.append("void showElement(string elemName, bool b)");
	list.append("void loadLayout(string filePath)");
	list.append("void showTableWidget(bool b)");
	list.append("void showTreeWidget(bool b)");
	list.append("void enableNavigation(bool b)");
	list.append("void showMenu(bool b)");
	list.append("void showToolbar(bool b)");
	list.append("void showView(int viewIndex)");
  list.append("dyn_string getCameraInfo()");
  list.append("void seekToTarget(dyn_string targetCameraSettings, int time, int fps)");
	list.append("void zoom(float level)");

	return list;
}

QVariant ISpyDCSInterface::invokeMethod(const QString &name, QList<QVariant> &values, QString &error) {
	if (name == "loadModel") {
		if (!hasNumArgs(name, values, 1, error)) return QVariant();
		dcsWidget->enableRendering(false);
		dcsWidget->loadModel(values[0].toString());
		dcsWidget->enableRendering(true);
		return QVariant();
	}
	else if (name == "loadLayout") {
		if (!hasNumArgs(name, values, 1, error)) return QVariant();
		dcsWidget->loadLayout(values[0].toString());
		return QVariant();
	}
	else if (name == "setColor") {
		if (!hasNumArgs(name, values, 4, error)) return QVariant();
		dcsWidget->enableRendering(false);
		dcsWidget->setColor(values[0].toString(), values[1].toUInt(), values[2].toUInt(), values[3].toUInt());
		dcsWidget->enableRendering(true);
		return QVariant();
	}
	else if (name == "setTransparency") {
		if (!hasNumArgs(name, values, 2, error)) return QVariant();
		dcsWidget->enableRendering(false);
		dcsWidget->setTransparency(values[0].toString(), values[1].toFloat());
		dcsWidget->enableRendering(true);
		return QVariant();
	}
	else if (name == "setSolid") {
		if (!hasNumArgs(name, values, 2, error)) return QVariant();
		dcsWidget->enableRendering(false);
		dcsWidget->setSolid(values[0].toString(), values[1].toBool());
		dcsWidget->enableRendering(true);
		return QVariant();
	}
	else if (name == "showElement") {
		if (!hasNumArgs(name, values, 2, error)) return QVariant();
		dcsWidget->enableRendering(false);
		dcsWidget->showElement(values[0].toString(), values[1].toBool());
		dcsWidget->enableRendering(true);
		return QVariant();
	}
	else if (name == "showTableWidget") {
		if (!hasNumArgs(name, values, 1, error)) return QVariant();
		dcsWidget->showTableWidget(values[0].toBool());
		return QVariant();
	}
	else if (name == "showTreeWidget") {
		if (!hasNumArgs(name, values, 1, error)) return QVariant();
		dcsWidget->showTreeWidget(values[0].toBool());
		return QVariant();
	}
	else if (name == "showView") {
		if (!hasNumArgs(name, values, 1, error)) return QVariant();
		dcsWidget->showView(values[0].toInt());
		return QVariant();
	}
	else if (name == "enableNavigation") {
		if (!hasNumArgs(name, values, 1, error)) return QVariant();
		dcsWidget->enableNavigation(values[0].toBool());
		return QVariant();
	}
	else if (name == "showMenu") {
		if (!hasNumArgs(name, values, 1, error)) return QVariant();
		dcsWidget->showMenu(values[0].toBool());
		return QVariant();
	}
	else if (name == "showToolbar") {
		if (!hasNumArgs(name, values, 1, error)) return QVariant();
		dcsWidget->showToolbar(values[0].toBool());
		return QVariant();
	}
	else if (name=="getCameraInfo") {
		if (!hasNumArgs(name, values, 0, error) ) return QVariant();
		return dcsWidget->getCameraInfo();
	}
	else if (name == "seekToTarget") {
		if (!hasNumArgs(name, values, 1, error)) return QVariant();
		int time = (values.size() >= 2) ? values[1].toInt() : 1000;
		int fps  = (values.size() >= 3) ? values[2].toInt() : 30;
		dcsWidget->seekToTargetSmoothly(values[0].toList(), time, fps);
	}
	else if (name == "zoom") {
		if (!hasNumArgs(name, values, 1, error)) return QVariant();
		dcsWidget->zoom(values[0].toFloat());
	}

	return QVariant();
}

void ISpyDCSInterface::elementSelected(QString name) {
	QList<QVariant> vector;
	vector << QVariant(name);

	emit signal("elementSelected", vector);
}