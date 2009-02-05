#ifndef __IG_OPROF_CONFIG_FILE_H__
#define __IG_OPROF_CONFIG_FILE_H__

#include <qdom.h>

class QFile;
class QString;

class IgOProfConfigFile : public QDomDocument
{
private:
    QDomElement root;
    QDomElement t;
public:
    IgOProfConfigFile(QFile *file);
    IgOProfConfigFile(const char *rootname);
    void setBool(const char * name);
    void setSingle(const char *tag, const char *value = "");
    void setSingle(const char *tag, const QString &value);
    void appendCustomChild(QDomElement &e);
    bool isSet(const char *tag);
    QString getArg(const char *tag, const char *def="");
    QString getArg(const char *tag, const QString &def);
};

#endif
