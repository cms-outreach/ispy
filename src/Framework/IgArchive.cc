#include "Framework/IgArchive.h"
#include "QtGUI/IgSubProcess.h"
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include "QString"
#include "QRegExp"
#include "QStringBuilder"
#include "QTextStream"
#include "QProcess"

void
IgArchive::readMembers()
{
  QStringList args;

  IgSubProcess subprocess;

#ifdef WIN32
//  QString command("C:/Graphics/unzip/win32/vc8/unzip__Win32_Release/unzip");
  QString command("unzip");
  args << "-Z" << "-1";
#else
  QString command("unzip");
  args << "-Z" << "-1";
#endif

  args << m_filename;
  subprocess.start(command, args);
  if (!subprocess.waitForFinished()) throw IgArchive::Exception();

  QTextStream ss(subprocess.getBuffer());
  
  while(!ss.atEnd())
  {
    QString memberName = ss.readLine();
	m_members.push_back(new IgMember(memberName));
  }
}

const QByteArray&
IgArchiveReader::read(IgMember *member)
{
  QStringList args;

  IgSubProcess subprocess;

#ifdef WIN32
//  QString command("C:/Graphics/unzip/win32/vc8/unzip__Win32_Release/unzip");
  QString command("unzip");
  args << "-p";
#else
  QString command("unzip");
  args << "-p";
#endif

  args << m_archive->name();
  args << member->name();
  subprocess.start(command, args);

  if (!subprocess.waitForFinished(120000)) throw IgArchive::Exception();


  m_stream.append(subprocess.getBuffer());
  return m_stream;
}