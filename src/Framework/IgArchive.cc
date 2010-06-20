#include "Framework/IgArchive.h"
#include <string>
#include <vector>
#include <iostream>
#include <sstream>

void
IgArchive::readMembers()
{
  std::string cmd = "unzip -Z -1 " + m_filename;
  FILE *f = popen(cmd.c_str(), "r");
  if (!f)
    throw IgArchive::Exception();
  std::string string;

  char buffer[1024*128];
  
  while (!feof(f))
  {
    size_t s = fread(buffer, 1, 1024*128-1, f);
    buffer[s] = 0;
    string += buffer;
  }

  if (pclose(f))
    throw IgArchive::Exception();

  std::istringstream ss(string);
  
  while(ss)
  {
    std::string memberName;
    getline(ss, memberName);
    m_members.push_back(new IgMember(memberName));
  }
}

const std::string
IgArchiveReader::read(IgMember *member)
{
  FILE *f = popen((std::string("unzip -q -c ") + m_archive->name() + " " + member->name()).c_str(), "r");
  if (!f)
    throw IgArchive::Exception();
  std::string string;
  char buffer[1024*128];
  
  while (!feof(f))
  {
    size_t s = fread(buffer, 1, 1024*128-1, f);
    buffer[s] = 0;
    string += buffer;
  }

  if (pclose(f))
    throw IgArchive::Exception();

  return string;  
}