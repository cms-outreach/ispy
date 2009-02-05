//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgQtProfileService/src/IgOProfConfigFile.h"
#include <qdom.h>
#include <qstring.h>
#include <qfile.h>

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

IgOProfConfigFile::IgOProfConfigFile(QFile *file)
{
    setContent(file);
}

IgOProfConfigFile::IgOProfConfigFile(const char *rootname)
{
    root = createElement(QString(rootname));
    appendChild(root);
}

void 
IgOProfConfigFile::setBool(const char * name)
{
    t = createElement(QString(name));
    root.appendChild(t);
}

void 
IgOProfConfigFile::setSingle(const char *tag, const char *value)
{
    (void) value;
    t = createElement(tag);
    t.setAttribute(QString("VALUE"),QString(tag));
    root.appendChild(t);	
}

void 
IgOProfConfigFile::setSingle(const char *tag, const QString &value)
{
    setSingle(tag,value.latin1());
}

void
IgOProfConfigFile::appendCustomChild(QDomElement &e)
{
    root.appendChild(e);
}

bool
IgOProfConfigFile::isSet(const char *tag)
{
    return elementsByTagName(tag).count() != 0;
}

QString
IgOProfConfigFile::getArg(const char *tag, const char *def)
{
    return elementsByTagName(tag).item(0).toElement().attribute(QString("VALUE"), QString(def));
}

QString
IgOProfConfigFile::getArg(const char *tag, const QString &def)
{
    return getArg(tag,def.latin1());
}
