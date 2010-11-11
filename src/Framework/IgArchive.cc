#include "Framework/IgArchive.h"
#include "unzip.h"
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstring>

void
IgArchive::readMembers()
{
  unzFile uf = NULL;
  
#ifdef USEWIN32IOAPI
  zlib_filefunc64_def ffunc;
#endif
  
#ifdef USEWIN32IOAPI
  fill_win32_filefunc64A(&ffunc);
  uf = unzOpen2_64(m_filename.c_str(),&ffunc);
#else
  uf = unzOpen64(m_filename.c_str());
#endif
  
  if (uf==NULL)
    throw IgArchive::Exception();
  
  uLong i;
  unz_global_info64 gi;
  int err;

  err = unzGetGlobalInfo64(uf,&gi);
  for (i=0; i < gi.number_entry; ++i)
  {
    // FIXME: does this really mean that archives cannot have filenames
    //        larger than 4096 characters???
    char memberName[4096];
    unz_file_info64 file_info;
    uLong ratio=0;
    const char *string_method;
    char charCrypt=' ';
    err = unzGetCurrentFileInfo64(uf, &file_info, memberName,
                                  sizeof(memberName), NULL, 0, NULL, 0);
    if (err != UNZ_OK)
      throw IgArchive::Exception();

    m_members.push_back(new IgMember(memberName));
    
    if ((i+1) < gi.number_entry)
    {
        err = unzGoToNextFile(uf);
        if (err != UNZ_OK)
          throw IgArchive::Exception();
    }
  }
  
  unzClose(uf);
}

void
IgArchiveReader::read(IgMember *member, std::string &result)
{
  unzFile uf = unzOpen(m_archive->name());
  if (uf == NULL)
    throw IgArchive::Exception();
    
  if (unzLocateFile(uf, member->name(), 0) != UNZ_OK)
    throw IgArchive::Exception();

  unz_file_info64 info;
  int ms = strlen(member->name());
  char *mn = new char[ms]; 
  if (unzGetCurrentFileInfo64(uf, &info, mn, ms, NULL, 0, NULL, 0) != UNZ_OK
      || unzOpenCurrentFile(uf) != UNZ_OK)
  {
    delete mn;
    throw IgArchive::Exception();
  }
  
  delete mn;

  char *buffer = (char*) malloc(info.uncompressed_size);
  int readSize = unzReadCurrentFile(uf, buffer, info.uncompressed_size);
  if (readSize != info.uncompressed_size)
  {
    free(buffer);
    throw IgArchive::Exception();
  }
  
  result.assign(buffer, info.uncompressed_size);
  free(buffer);
  
  if (unzCloseCurrentFile(uf) != UNZ_OK)
    throw IgArchive::Exception();
}
