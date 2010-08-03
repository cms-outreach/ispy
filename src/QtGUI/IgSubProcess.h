#ifndef IGUANA_IG_SUBPROCESS_H
# define IGUANA_IG_SUBPROCESS_H

#include <QObject>
#include <QProcess>
#include <QByteArray>

class IgSubProcess : public QProcess
{
	Q_OBJECT

public:
	IgSubProcess();
	QByteArray& getBuffer() { return m_buffer; }
public slots:
	void readMore();
private:
	QByteArray m_buffer;

};
#endif //IGUANA_QTGUI_IGSUB_PROCESS_H
