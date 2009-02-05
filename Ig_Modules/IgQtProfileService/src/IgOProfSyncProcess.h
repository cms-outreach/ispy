#ifndef __IG_OPROF_SYNC_PROCESS_H__
#define __IG_OPROF_SYNC_PROCESS_H__

# include "Ig_Modules/IgQtProfileService/interface/config.h"

# include <qstring.h>
# include <qprocess.h>

class IG_QT_PROFILE_SERVICE_API IgOProfSyncProcess : public QObject
{
    Q_OBJECT
private:
    QProcess *Process;
    QString results;
public slots:
void readOutput(void);
public:
    IgOProfSyncProcess(const QString &program);
    QProcess &operator()(void);
    QString string(void);
};

#endif
