#ifndef ISPYDCSINTERFACE_H
#define ISPYDCSINTERFACE_H

#include <BaseExternWidget.hxx>

class ISpyDCSWidget;
class EWO_EXPORT ISpyDCSInterface: public BaseExternWidget {
Q_OBJECT

public:
	ISpyDCSInterface(QWidget* parent);
	~ISpyDCSInterface();

	virtual QWidget* widget() const;

	virtual QStringList signalList() const;
	virtual QStringList methodList() const;

public slots:
	virtual QVariant invokeMethod(const QString& name, QList<QVariant>& values, QString& error);
	void elementSelected(QString);

private:
	ISpyDCSWidget* dcsWidget;
};

#endif // ISPYDCSINTERFACE_H
