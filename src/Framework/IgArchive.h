#include <string>
#include <vector>

class IgMember
{
public:
  // FIXME: for the time being we assume that it's only real ig files.
  IgMember(const std::string &name)
  :m_name(name), m_directory(false)
  {}
  
  bool isDirectory() { return m_directory; }
  const char *name() { return m_name.c_str(); }
  bool empty(void) { return false; }

private:
  std::string   m_name;
  bool          m_directory;
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
  
  IgArchive(const char *filename)
  :m_filename(filename)
  {
    readMembers();
  }
  
  typedef std::vector<IgMember *> Members;
  const Members  &members() {return m_members;};
  void close(void) {}
  const char *name(void) { return m_filename.c_str(); }
private:  
  void readMembers();

  std::string           m_filename;
  Members               m_members;
};

// 
class IgArchiveReader
{
public:
  IgArchiveReader(IgArchive *archive)
  :m_archive(archive)
  {}
  
  const std::string read(IgMember *member);
private:
  IgArchive *m_archive;
};