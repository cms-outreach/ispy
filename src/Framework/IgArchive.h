#include <string>
#include <vector>
#include "QString"
#include "QByteArray"

class IgMember
{
public:
  // FIXME: for the time being we assume that it's only real ig files.
  IgMember(const QString &name)
  :m_name(name), m_directory(false)
  {}
  
  bool isDirectory(void) { return m_directory; }
  const QString name(void) { return m_name; }
  bool empty(void) { return false; }


private:
  QString	 m_name;
  bool       m_directory;
};

// FIXME: should allow to specify read or write in constructor.
// FIXME: IgArchive::close() is a noop. It should be reimplemented so that it
//        actually closes a zip file.
// FIXME: do a better exception class.
class IgArchive
{
public:
  class Exception
  {
  public:
    const char *explain(void) { return "Unable to open file"; }
  };
  
  IgArchive(const QString &filename)
  :m_filename(filename)
  {
    readMembers();
  }

  typedef std::vector<IgMember *> Members;
  const Members  &members() {return m_members;};
  void close(void) {}
  const QString name(void) { return m_filename; }
private:  
  void readMembers();

  QString              m_filename;
  Members              m_members;
};

// 
class IgArchiveReader
{

public:
  IgArchiveReader(IgArchive *archive)
  :m_archive(archive)
  {}
  
  const QByteArray& read(IgMember *member);

private:
  IgArchive *m_archive;
  QByteArray m_stream;
};