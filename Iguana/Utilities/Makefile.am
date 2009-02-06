DEFAULT_INCLUDES = -I. -I$(srcdir)

######################################################################
EXTRA_LTLIBRARIES =			\
  libext-zlib.la			\
  libext-bz2lib.la			\
  libext-uuid.la			\
  libext-pcre.la			\
  libext-md5.la				\
  libext-sha1.la			\
  libext-dl.la				\
  libext-rx.la

lib_LTLIBRARIES	=			\
  libclasslib.la

libclasslib_la_LDFLAGS =		\
  -avoid-version -no-undefined		\
  @ZLIB@ @BZ2LIB@ @UUIDLIB@		\
  @PCRELIB@ @MD5LIB@ @SHA1LIB@		\
  @LIBS@ @EXTRA@
libclasslib_la_LIBADD =			\
  @ZLTLIB@ @BZ2LTLIB@ @UUIDLTLIB@	\
  @PCRELTLIB@ @MD5LTLIB@ @SHA1LTLIB@	\
  @DLLTLIB@ @RXLTLIB@
libclasslib_la_DEPENDENCIES =		\
  @ZLTLIB@ @BZ2LTLIB@ @UUIDLTLIB@	\
  @PCRELTLIB@ @MD5LTLIB@ @SHA1LTLIB@	\
  @DLLTLIB@ @RXLTLIB@
libclasslib_la_CXXFLAGS =		\
  @ZINC@ @BZ2INC@ @UUIDINC@		\
  @PCREINC@ @MD5INC@ @SHA1INC@		\
  @DLINC@ @RXINC@
libclasslib_la_SOURCES =		\
  src/utils/Argz.cpp			\
  src/utils/AutoLoad.cpp		\
  src/utils/DebugAids.cpp		\
  src/utils/Environment.cpp		\
  src/utils/Error.cpp			\
  src/utils/HostInfo.cpp		\
  src/utils/IntTraits.cpp		\
  src/utils/Log.cpp			\
  src/utils/LogConsole.cpp		\
  src/utils/LogSink.cpp			\
  src/utils/LogStderr.cpp		\
  src/utils/LongLong.cpp		\
  src/utils/LowMemoryPool.cpp		\
  src/utils/MultiMethod.cpp		\
  src/utils/ProcessInfo.cpp		\
  src/utils/Regexp.cpp			\
  src/utils/RegexpError.cpp		\
  src/utils/RegexpMatch.cpp		\
  src/utils/ResourceInfo.cpp		\
  src/utils/SearchPath.cpp		\
  src/utils/SharedLibrary.cpp		\
  src/utils/SharedLibraryError.cpp	\
  src/utils/ShellEnvironment.cpp	\
  src/utils/Signal.cpp			\
  src/utils/StringFormat.cpp		\
  src/utils/StringOps.cpp		\
  src/utils/SystemError.cpp		\
  src/utils/SystemInfo.cpp		\
  src/utils/Time.cpp			\
  src/utils/TimeInfo.cpp		\
  src/utils/UUID.cpp			\
  src/utils/UUIDError.cpp		\
  src/utils/UserInfo.cpp		\
  src/utils/UnixUserInfo.cpp		\
  src/utils/WindowsUserInfo.cpp		\
  src/utils/Windows.cpp			\
  src/utils/XTypeInfo.cpp		\
  src/iobase/DirIterator.cpp		\
  src/iobase/UnixDirIterator.cpp	\
  src/iobase/WindowsDirIterator.cpp	\
  src/iobase/File.cpp			\
  src/iobase/UnixFile.cpp		\
  src/iobase/WindowsFile.cpp		\
  src/iobase/FileAcl.cpp		\
  src/iobase/UnixFileAcl.cpp		\
  src/iobase/WindowsFileAcl.cpp		\
  src/iobase/FileError.cpp		\
  src/iobase/FileLock.cpp		\
  src/iobase/UnixFileLock.cpp		\
  src/iobase/WindowsFileLock.cpp	\
  src/iobase/FileMapping.cpp		\
  src/iobase/UnixFileMapping.cpp	\
  src/iobase/WindowsFileMapping.cpp	\
  src/iobase/FileMappingError.cpp	\
  src/iobase/FileNameLock.cpp		\
  src/iobase/FileSystemError.cpp	\
  src/iobase/Filename.cpp		\
  src/iobase/UnixFilename.cpp		\
  src/iobase/WindowsFilename.cpp	\
  src/iobase/FilenameError.cpp		\
  src/iobase/IOChannel.cpp		\
  src/iobase/UnixIOChannel.cpp		\
  src/iobase/WindowsIOChannel.cpp	\
  src/iobase/IOChannelPair.cpp		\
  src/iobase/IOChannelTransfer.cpp	\
  src/iobase/IOError.cpp		\
  src/iobase/IOInput.cpp		\
  src/iobase/IOOutput.cpp		\
  src/iobase/IOSelectDevPoll.cpp	\
  src/iobase/IOSelectFdMap.cpp		\
  src/iobase/IOSelectKqueue.cpp		\
  src/iobase/IOSelectMethod.cpp		\
  src/iobase/IOSelectPoll.cpp		\
  src/iobase/IOSelectSelect.cpp		\
  src/iobase/IOSelectSig.cpp		\
  src/iobase/IOSelectSigFd.cpp		\
  src/iobase/IOSelectWindows.cpp	\
  src/iobase/IOSelector.cpp		\
  src/iobase/IOStatus.cpp		\
  src/iobase/UnixIOStatus.cpp		\
  src/iobase/WindowsIOStatus.cpp	\
  src/iobase/InetServerSocket.cpp	\
  src/iobase/InetSocket.cpp		\
  src/iobase/LocalServerSocket.cpp	\
  src/iobase/LocalSocket.cpp		\
  src/iobase/LogFile.cpp		\
  src/iobase/LogPipe.cpp		\
  src/iobase/LogSocket.cpp		\
  src/iobase/NamedPipeServer.cpp	\
  src/iobase/UnixNamedPipeServer.cpp	\
  src/iobase/WindowsNamedPipeServer.cpp	\
  src/iobase/NetworkError.cpp		\
  src/iobase/Pipe.cpp			\
  src/iobase/UnixPipe.cpp		\
  src/iobase/WindowsPipe.cpp		\
  src/iobase/PipeCmd.cpp		\
  src/iobase/Socket.cpp			\
  src/iobase/UnixSocket.cpp		\
  src/iobase/WindowsSocket.cpp		\
  src/iobase/SocketConst.cpp		\
  src/iobase/SocketPair.cpp		\
  src/iobase/UnixSocketPair.cpp		\
  src/iobase/WindowsSocketPair.cpp	\
  src/iobase/Storage.cpp		\
  src/iobase/SubProcess.cpp		\
  src/iobase/UnixSubProcess.cpp		\
  src/iobase/WindowsSubProcess.cpp	\
  src/iobase/SubProcessError.cpp	\
  src/iobase/TempFile.cpp		\
  src/iobase/UnixTempFile.cpp		\
  src/iobase/WindowsTempFile.cpp	\
  src/iotools/BufferInputStream.cpp	\
  src/iotools/BufferOutputStream.cpp	\
  src/iotools/FilterInputStream.cpp	\
  src/iotools/FilterOutputStream.cpp	\
  src/iotools/IOChannelInputStream.cpp	\
  src/iotools/IOChannelOutputStream.cpp	\
  src/iotools/IOChannelStreamBuf.cpp	\
  src/iotools/IOException.cpp		\
  src/iotools/InputStream.cpp		\
  src/iotools/InputStreamBuf.cpp	\
  src/iotools/MemoryStorage.cpp		\
  src/iotools/OutputStream.cpp		\
  src/iotools/OutputStreamBuf.cpp	\
  src/iotools/PipedInputStream.cpp	\
  src/iotools/PipedOutputStream.cpp	\
  src/iotools/PushBackBuffer.cpp	\
  src/iotools/PushBackInputStream.cpp	\
  src/iotools/ReadBuffer.cpp		\
  src/iotools/ReadWriteBuffer.cpp	\
  src/iotools/StdError.cpp		\
  src/iotools/StdInputStream.cpp	\
  src/iotools/StdOutputStream.cpp	\
  src/iotools/StdStorage.cpp		\
  src/iotools/StdioError.cpp		\
  src/iotools/StdioStorage.cpp		\
  src/iotools/StorageInputStream.cpp	\
  src/iotools/StorageOutputStream.cpp	\
  src/iotools/StorageStreamBuf.cpp	\
  src/iotools/SubStorage.cpp		\
  src/iotools/WriteBuffer.cpp		\
  src/zip/Adler32.cpp			\
  src/zip/BZCompressor.cpp		\
  src/zip/BZDecompressor.cpp		\
  src/zip/BZError.cpp			\
  src/zip/BZIPInputStream.cpp		\
  src/zip/BZIPOutputStream.cpp		\
  src/zip/CPIOConstants.cpp		\
  src/zip/CPIOError.cpp			\
  src/zip/CPIOInputStream.cpp		\
  src/zip/CPIOMember.cpp		\
  src/zip/CPIOOutputStream.cpp		\
  src/zip/CRC32.cpp			\
  src/zip/CheckedInputStream.cpp	\
  src/zip/CheckedOutputStream.cpp	\
  src/zip/Checksum.cpp			\
  src/zip/CompressOutputStream.cpp	\
  src/zip/Compressor.cpp		\
  src/zip/DecompressInputStream.cpp	\
  src/zip/Decompressor.cpp		\
  src/zip/Digest.cpp			\
  src/zip/GZIPInputStream.cpp		\
  src/zip/GZIPOutputStream.cpp		\
  src/zip/InteractiveCompressor.cpp	\
  src/zip/InteractiveDecompressor.cpp	\
  src/zip/MD5Digest.cpp			\
  src/zip/SHA1Digest.cpp		\
  src/zip/SHA1Error.cpp			\
  src/zip/TarConstants.cpp		\
  src/zip/TarError.cpp			\
  src/zip/TarInputStream.cpp		\
  src/zip/TarMember.cpp			\
  src/zip/TarOutputStream.cpp		\
  src/zip/ZCompressor.cpp		\
  src/zip/ZDecompressor.cpp		\
  src/zip/ZError.cpp			\
  src/zip/ZInputStream.cpp		\
  src/zip/ZOutputStream.cpp		\
  src/zip/ZipArchive.cpp		\
  src/zip/ZipInputStream.cpp		\
  src/zip/ZipMember.cpp			\
  src/zip/ZipOutputStream.cpp		\
  src/zip/ZipUtils.cpp

nobase_include_HEADERS =		\
  classlib/sysapi/config.h		\
  classlib/iobase/DirIterator.h		\
  classlib/iobase/File.h		\
  classlib/iobase/FileAcl.h		\
  classlib/iobase/FileError.h		\
  classlib/iobase/FileLock.h		\
  classlib/iobase/FileMapping.h		\
  classlib/iobase/FileMappingError.h	\
  classlib/iobase/FileNameLock.h	\
  classlib/iobase/FileSystemError.h	\
  classlib/iobase/Filename.h		\
  classlib/iobase/FilenameError.h	\
  classlib/iobase/IOBuffer.h		\
  classlib/iobase/IOChannel.h		\
  classlib/iobase/IOChannelPair.h	\
  classlib/iobase/IOError.h		\
  classlib/iobase/IOFlags.h		\
  classlib/iobase/IOInput.h		\
  classlib/iobase/IOOutput.h		\
  classlib/iobase/IOSelectCB.h		\
  classlib/iobase/IOSelectEvent.h	\
  classlib/iobase/IOSelector.h		\
  classlib/iobase/IOStatus.h		\
  classlib/iobase/InetServerSocket.h	\
  classlib/iobase/InetSocket.h		\
  classlib/iobase/LocalServerSocket.h	\
  classlib/iobase/LocalSocket.h		\
  classlib/iobase/NamedPipeServer.h	\
  classlib/iobase/NetworkError.h	\
  classlib/iobase/Pipe.h		\
  classlib/iobase/PipeCmd.h		\
  classlib/iobase/Socket.h		\
  classlib/iobase/SocketConst.h		\
  classlib/iobase/SocketPair.h		\
  classlib/iobase/Storage.h		\
  classlib/iobase/SubProcess.h		\
  classlib/iobase/SubProcessError.h	\
  classlib/iobase/TempFile.h		\
  classlib/iotools/BufferInputStream.h	\
  classlib/iotools/BufferOutputStream.h	\
  classlib/iotools/CustomStreamBuf.h	\
  classlib/iotools/FilterInputStream.h	\
  classlib/iotools/FilterOutputStream.h	\
  classlib/iotools/IOChannelInputStream.h\
  classlib/iotools/IOChannelOutputStream.h\
  classlib/iotools/IOChannelStreamBuf.h	\
  classlib/iotools/IOException.h	\
  classlib/iotools/InputStream.h	\
  classlib/iotools/InputStreamBuf.h	\
  classlib/iotools/MemoryStorage.h	\
  classlib/iotools/OutputStream.h	\
  classlib/iotools/OutputStreamBuf.h	\
  classlib/iotools/PipedInputStream.h	\
  classlib/iotools/PipedOutputStream.h	\
  classlib/iotools/PushBackBuffer.h	\
  classlib/iotools/PushBackInputStream.h\
  classlib/iotools/ReadBuffer.h		\
  classlib/iotools/ReadWriteBuffer.h	\
  classlib/iotools/StdError.h		\
  classlib/iotools/StdInputStream.h	\
  classlib/iotools/StdOutputStream.h	\
  classlib/iotools/StdStorage.h		\
  classlib/iotools/StdioError.h		\
  classlib/iotools/StdioStorage.h	\
  classlib/iotools/StorageInputStream.h	\
  classlib/iotools/StorageOutputStream.h\
  classlib/iotools/StorageStreamBuf.h	\
  classlib/iotools/SubStorage.h		\
  classlib/iotools/WriteBuffer.h	\
  classlib/sysapi/DebugAids.h		\
  classlib/sysapi/DirIterator.h		\
  classlib/sysapi/ElfAbi.h		\
  classlib/sysapi/File.h		\
  classlib/sysapi/FileAcl.h		\
  classlib/sysapi/FileLock.h		\
  classlib/sysapi/FileMapping.h		\
  classlib/sysapi/FileNameLock.h	\
  classlib/sysapi/Filename.h		\
  classlib/sysapi/HostInfo.h		\
  classlib/sysapi/IOChannel.h		\
  classlib/sysapi/IOChannelTransfer.h	\
  classlib/sysapi/IOPlatformData.h	\
  classlib/sysapi/IOStatus.h		\
  classlib/sysapi/IOTypes.h		\
  classlib/sysapi/InetSocket.h		\
  classlib/sysapi/LocalSocket.h		\
  classlib/sysapi/NamedPipeServer.h	\
  classlib/sysapi/Pipe.h		\
  classlib/sysapi/PipeCmd.h		\
  classlib/sysapi/ProcessInfo.h		\
  classlib/sysapi/ResourceInfo.h	\
  classlib/sysapi/SharedLibrary.h	\
  classlib/sysapi/Signal.h		\
  classlib/sysapi/Socket.h		\
  classlib/sysapi/SocketPair.h		\
  classlib/sysapi/SubProcess.h		\
  classlib/sysapi/SystemError.h		\
  classlib/sysapi/SystemInfo.h		\
  classlib/sysapi/TempFile.h		\
  classlib/sysapi/Time.h		\
  classlib/sysapi/TimeInfo.h		\
  classlib/sysapi/UserInfo.h		\
  classlib/sysapi/Windows.h		\
  classlib/sysapi/WindowsIO.h		\
  classlib/sysapi/iosfwd.h		\
  classlib/sysapi/nonstdint.h		\
  classlib/sysapi/system.h		\
  classlib/utils/Argz.h			\
  classlib/utils/AutoLoad.h		\
  classlib/utils/BitIterator.h		\
  classlib/utils/BitOps.h		\
  classlib/utils/BitPattern.h		\
  classlib/utils/BitTraits.h		\
  classlib/utils/Callback.h		\
  classlib/utils/DebugAids.h		\
  classlib/utils/Environment.h		\
  classlib/utils/Error.h		\
  classlib/utils/Hook.h			\
  classlib/utils/HookList.h		\
  classlib/utils/HostInfo.h		\
  classlib/utils/IntBits.h		\
  classlib/utils/IntTraits.h		\
  classlib/utils/Log.h			\
  classlib/utils/LongLong.h		\
  classlib/utils/LowMemoryPool.h	\
  classlib/utils/Macros.h		\
  classlib/utils/MapIterator.h		\
  classlib/utils/MultiMethod.h		\
  classlib/utils/PODVector.h		\
  classlib/utils/ProcessInfo.h		\
  classlib/utils/Range.h		\
  classlib/utils/Regexp.h		\
  classlib/utils/RegexpError.h		\
  classlib/utils/RegexpMatch.h		\
  classlib/utils/ResourceInfo.h		\
  classlib/utils/SaveErrno.h		\
  classlib/utils/SearchPath.h		\
  classlib/utils/Sequence.h		\
  classlib/utils/SharedLibrary.h	\
  classlib/utils/SharedLibraryError.h	\
  classlib/utils/ShellEnvironment.h	\
  classlib/utils/Signal.h		\
  classlib/utils/StringFormat.h		\
  classlib/utils/StringList.h		\
  classlib/utils/StringOps.h		\
  classlib/utils/SysErr.h		\
  classlib/utils/SystemError.h		\
  classlib/utils/SystemInfo.h		\
  classlib/utils/Time.h			\
  classlib/utils/TimeConst.h		\
  classlib/utils/TimeInfo.h		\
  classlib/utils/UUID.h			\
  classlib/utils/UUIDError.h		\
  classlib/utils/Unwind.h		\
  classlib/utils/UserInfo.h		\
  classlib/utils/XTypeInfo.h		\
  classlib/utils/iosfwd.h		\
  classlib/zip/Adler32.h		\
  classlib/zip/BZCompressor.h		\
  classlib/zip/BZConstants.h		\
  classlib/zip/BZDecompressor.h		\
  classlib/zip/BZError.h		\
  classlib/zip/BZIPInputStream.h	\
  classlib/zip/BZIPOutputStream.h	\
  classlib/zip/CPIOConstants.h		\
  classlib/zip/CPIOError.h		\
  classlib/zip/CPIOInputStream.h	\
  classlib/zip/CPIOMember.h		\
  classlib/zip/CPIOOutputStream.h	\
  classlib/zip/CRC32.h			\
  classlib/zip/CheckedInputStream.h	\
  classlib/zip/CheckedOutputStream.h	\
  classlib/zip/Checksum.h		\
  classlib/zip/CompressOutputStream.h	\
  classlib/zip/Compressor.h		\
  classlib/zip/DecompressInputStream.h	\
  classlib/zip/Decompressor.h		\
  classlib/zip/Digest.h			\
  classlib/zip/GZIPInputStream.h	\
  classlib/zip/GZIPOutputStream.h	\
  classlib/zip/InteractiveCompressor.h	\
  classlib/zip/InteractiveDecompressor.h\
  classlib/zip/MD5Digest.h		\
  classlib/zip/SHA1Digest.h		\
  classlib/zip/SHA1Error.h		\
  classlib/zip/TarConstants.h		\
  classlib/zip/TarError.h		\
  classlib/zip/TarInputStream.h		\
  classlib/zip/TarMember.h		\
  classlib/zip/TarOutputStream.h	\
  classlib/zip/ZCompressor.h		\
  classlib/zip/ZConstants.h		\
  classlib/zip/ZDecompressor.h		\
  classlib/zip/ZError.h			\
  classlib/zip/ZInputStream.h		\
  classlib/zip/ZOutputStream.h		\
  classlib/zip/ZipArchive.h		\
  classlib/zip/ZipInputStream.h		\
  classlib/zip/ZipMember.h		\
  classlib/zip/ZipOutputStream.h	\
  classlib/zip/ZipUtils.h

######################################################################
libext_zlib_la_CFLAGS = -Iext/zlib -I$(srcdir)/ext/zlib
libext_zlib_la_SOURCES = \
  ext/zlib/adler32.c ext/zlib/compress.c ext/zlib/crc32.c \
  ext/zlib/gzio.c ext/zlib/uncompr.c ext/zlib/deflate.c \
  ext/zlib/trees.c ext/zlib/zutil.c ext/zlib/inflate.c \
  ext/zlib/infback.c ext/zlib/inffast.c ext/zlib/inftrees.c

libext_bz2lib_la_CFLAGS = -Iext/bz2lib -I$(srcdir)/ext/bz2lib
libext_bz2lib_la_SOURCES = \
  ext/bz2lib/blocksort.c ext/bz2lib/huffman.c \
  ext/bz2lib/crctable.c ext/bz2lib/randtable.c \
  ext/bz2lib/compress.c ext/bz2lib/decompress.c \
  ext/bz2lib/bzlib.c

# FIXME: Original is gen_uuid.c/uuid_time.c, use ours for windows compatibility
libext_uuid_la_CFLAGS = -Iext/uuid -I$(srcdir)/ext/uuid
libext_uuid_la_SOURCES = \
  ext/uuid/clear.c ext/uuid/compare.c ext/uuid/copy.c \
  ext/uuid/gen_uuid_new.c ext/uuid/isnull.c ext/uuid/pack.c \
  ext/uuid/parse.c ext/uuid/unpack.c ext/uuid/unparse.c \
  ext/uuid/uuid_time_new.c

libext_pcre_la_CFLAGS = -Iext/pcre -I$(srcdir)/ext/pcre
libext_pcre_la_SOURCES = \
  ext/pcre/pcre_chartables.c ext/pcre/pcre_compile.c \
  ext/pcre/pcre_config.c ext/pcre/pcre_dfa_exec.c \
  ext/pcre/pcre_exec.c ext/pcre/pcre_fullinfo.c \
  ext/pcre/pcre_get.c ext/pcre/pcre_globals.c \
  ext/pcre/pcre_info.c ext/pcre/pcre_maketables.c \
  ext/pcre/pcre_newline.c ext/pcre/pcre_ord2utf8.c \
  ext/pcre/pcre_refcount.c ext/pcre/pcre_study.c \
  ext/pcre/pcre_tables.c ext/pcre/pcre_try_flipped.c \
  ext/pcre/pcre_ucp_searchfuncs.c ext/pcre/pcre_valid_utf8.c \
  ext/pcre/pcre_version.c ext/pcre/pcre_xclass.c

libext_md5_la_CFLAGS = -Iext/md5 -I$(srcdir)/ext/md5
libext_md5_la_SOURCES = \
  ext/rfc1321/md5c.c

libext_sha1_la_CFLAGS = -Iext/sha1 -I$(srcdir)/ext/sha1
libext_sha1_la_SOURCES = \
  ext/rfc3174/sha1.c

libext_dl_la_CFLAGS = -Iext/aix-dlfcn -I$(srcdir)/ext/aix-dlfcn
libext_dl_la_SOURCES = \
  ext/aix-dlfcn/dlfcn.c

libext_rx_la_CFLAGS = -Iext/rx -I$(srcdir)/ext/rx
libext_rx_la_SOURCES = \
  ext/rx/hashrexp.c ext/rx/rx.c ext/rx/rxanal.c \
  ext/rx/rxbasic.c ext/rx/rxbitset.c ext/rx/rxcset.c \
  ext/rx/rxdbug.c ext/rx/rxgnucomp.c ext/rx/rxhash.c \
  ext/rx/rxnfa.c ext/rx/rxnode.c ext/rx/rxposix.c \
  ext/rx/rxsimp.c ext/rx/rxspencer.c ext/rx/rxstr.c \
  ext/rx/rxsuper.c ext/rx/rxunfa.c # ext/rx/rgx.c

EXTRA_PROGRAMS = dftables
dftables_SOURCES = ext/pcre/dftables.c
dftables_CFLAGS = -Iext/pcre -I$(srcdir)/ext/pcre

ext/pcre/pcre_chartables.c: dftables
	./dftables $@

######################################################################
TESTS_ENVIRONMENT = QUIET_ASSERT=a
TESTS = $(check_PROGRAMS)
check_LTLIBRARIES =			\
  libSharedLibrary01Lib.la
check_PROGRAMS =			\
  test_iobase_DirIterator01		\
  test_iobase_DirIterator02		\
  test_iobase_DirIterator03		\
  test_iobase_DirIterator04		\
  test_iobase_File01			\
  test_iobase_File02			\
  test_iobase_File03			\
  test_iobase_File04			\
  test_iobase_File05			\
  test_iobase_File06			\
  test_iobase_FileAcl01			\
  test_iobase_FileLock01		\
  test_iobase_FileMapping01		\
  test_iobase_FileMapping02		\
  test_iobase_FileMapping03		\
  test_iobase_Filename000		\
  test_iobase_Filename01		\
  test_iobase_Filename02_01		\
  test_iobase_Filename02_02		\
  test_iobase_Filename02_03		\
  test_iobase_Filename02_04		\
  test_iobase_Filename02_05		\
  test_iobase_Filename02_06		\
  test_iobase_Filename02_07		\
  test_iobase_Filename02_08		\
  test_iobase_Filename02_09		\
  test_iobase_Filename02_10		\
  test_iobase_Filename02_11		\
  test_iobase_Filename02_12		\
  test_iobase_Filename02_13		\
  test_iobase_Filename03		\
  test_iobase_Filename04		\
  test_iobase_Filename05		\
  test_iobase_Filename06		\
  test_iobase_Filename07		\
  test_iobase_Filename08		\
  test_iobase_Filename09		\
  test_iobase_IOChannel01		\
  test_iobase_IOSelector01		\
  test_iobase_IOSelector02		\
  test_iobase_IOSelector03		\
  test_iobase_IOStatus01		\
  test_iobase_InetServerSocket01	\
  test_iobase_InetSocket01		\
  test_iobase_LocalServerSocket01	\
  test_iobase_LocalSocket01		\
  test_iobase_MemoryStorage01		\
  test_iobase_NamedPipeServer01		\
  test_iobase_Pipe01			\
  test_iobase_PipeCmd01			\
  test_iobase_SocketPair01		\
  test_iobase_SubProcess01		\
  test_iobase_SubProcess02		\
  test_iobase_SubProcess03		\
  test_iobase_SubProcess04		\
  test_iobase_SubProcess05		\
  test_iobase_SubStorage01		\
  test_iobase_SubStorage02		\
  test_iobase_SubStorage03		\
  test_iobase_SubStorage04		\
  test_iobase_SubStorage05		\
  test_iotools_BufferInputStream01	\
  test_iotools_BufferOutputStream01	\
  test_iotools_CheckedInputStream01	\
  test_iotools_CheckedOutputStream01	\
  test_iotools_FilterInputStream01	\
  test_iotools_FilterOutputStream01	\
  test_iotools_IOChannelInputStream01	\
  test_iotools_IOChannelOutputStream01	\
  test_iotools_PipedInputStream01	\
  test_iotools_PipedOutputStream01	\
  test_iotools_PushBackInputStream01	\
  test_iotools_StdInputStream01		\
  test_iotools_StdInputStream02		\
  test_iotools_StdOutputStream01	\
  test_iotools_StdStorage01		\
  test_iotools_StdStorage02		\
  test_iotools_StdStorage03		\
  test_iotools_StdStorage04		\
  test_iotools_StdioStorage01		\
  test_iotools_StdioStorage02		\
  test_iotools_StdioStorage03		\
  test_iotools_StdioStorage04		\
  test_iotools_StorageInputStream01	\
  test_iotools_StorageOutputStream01	\
  test_utils_Argz01			\
  test_utils_AutoLoad01			\
  test_utils_BitIterator01		\
  test_utils_BitOps01			\
  test_utils_BitPattern01		\
  test_utils_BitTraits01		\
  test_utils_Callback01			\
  test_utils_Debug01			\
  test_utils_Environment01		\
  test_utils_Hook01			\
  test_utils_HookList01			\
  test_utils_HostInfo01			\
  test_utils_IntBits01			\
  test_utils_IntBits02			\
  test_utils_IntBits03			\
  test_utils_Log01			\
  test_utils_LogListener		\
  test_utils_LowMemoryPool01		\
  test_utils_MapIterator01		\
  test_utils_MapIterator02		\
  test_utils_MultiMethod01		\
  test_utils_MultiMethod02		\
  test_utils_MultiMethod03		\
  test_utils_ProcessInfo01		\
  test_utils_Range01			\
  test_utils_ResourceInfo01		\
  test_utils_SaveErrno01		\
  test_utils_SearchPath01		\
  test_utils_SearchPath02		\
  test_utils_Sequence01			\
  test_utils_SharedLibrary01		\
  test_utils_ShellEnvironment01		\
  test_utils_Signal01			\
  test_utils_Signal02			\
  test_utils_Signal03			\
  test_utils_StringFormat01		\
  test_utils_StringOps01		\
  test_utils_StringOps02		\
  test_utils_StringOps03		\
  test_utils_StringOps04		\
  test_utils_SystemInfo01		\
  test_utils_Time01			\
  test_utils_Time02			\
  test_utils_Time03			\
  test_utils_TimeInfo01			\
  test_utils_TimeInfo02			\
  test_utils_UUID01			\
  test_utils_UUID02			\
  test_utils_UUID03			\
  test_utils_UUID04			\
  test_utils_UUID05			\
  test_utils_Unwind01			\
  test_utils_UserInfo01			\
  test_utils_XTypeInfo01		\
  test_utils_exa_Debug			\
  test_utils_exa_Log			\
  test_utils_exa_ResourceInfo		\
  test_utils_exa_Signal			\
  test_zip_Adler3201			\
  test_zip_BZCompressor01		\
  test_zip_BZDecompressor01		\
  test_zip_BZIPInputStream01		\
  test_zip_BZIPOutputStream01		\
  test_zip_CPIOInputStream01		\
  test_zip_CPIOInputStream02		\
  test_zip_CPIOOutputStream00		\
  test_zip_CPIOOutputStream01		\
  test_zip_CPIOOutputStream02		\
  test_zip_CPIOOutputStream03		\
  test_zip_CRC3201			\
  test_zip_GZIPInputStream01		\
  test_zip_GZIPInputStream02		\
  test_zip_GZIPOutputStream01		\
  test_zip_InteractiveCompressor01	\
  test_zip_InteractiveDecompressor01	\
  test_zip_MD5Digest01			\
  test_zip_SHA1Digest01			\
  test_zip_TarInputStream01		\
  test_zip_TarInputStream02		\
  test_zip_TarInputStream03		\
  test_zip_TarOutputStream00		\
  test_zip_TarOutputStream01		\
  test_zip_ZCompressor01		\
  test_zip_ZDecompressor01		\
  test_zip_ZInputStream01		\
  test_zip_ZOutputStream01		\
  test_zip_ZipArchive01			\
  test_zip_ZipArchive02			\
  test_zip_ZipArchive03			\
  test_zip_ZipArchive04			\
  test_zip_ZipArchive05			\
  test_zip_ZipArchive06			\
  test_zip_ZipArchive07			\
  test_zip_ZipArchive08			\
  test_zip_ZipInputStream01		\
  test_zip_ZipInputStream02		\
  test_zip_ZipInputStream03		\
  test_zip_ZipOutputStream00		\
  test_zip_ZipOutputStream01		\
  test_zip_ZipUtils01

test_iobase_DirIterator01_SOURCES		= test/iobase/DirIterator01.cpp
test_iobase_DirIterator01_LDFLAGS		= -lclasslib

test_iobase_DirIterator02_SOURCES		= test/iobase/DirIterator02.cpp
test_iobase_DirIterator02_LDFLAGS		= -lclasslib

test_iobase_DirIterator03_SOURCES		= test/iobase/DirIterator03.cpp
test_iobase_DirIterator03_LDFLAGS		= -lclasslib

test_iobase_DirIterator04_SOURCES		= test/iobase/DirIterator04.cpp
test_iobase_DirIterator04_LDFLAGS		= -lclasslib

test_iobase_File01_SOURCES			= test/iobase/File01.cpp
test_iobase_File01_LDFLAGS			= -lclasslib

test_iobase_File02_SOURCES			= test/iobase/File02.cpp
test_iobase_File02_LDFLAGS			= -lclasslib

test_iobase_File03_SOURCES			= test/iobase/File03.cpp
test_iobase_File03_LDFLAGS			= -lclasslib

test_iobase_File04_SOURCES			= test/iobase/File04.cpp
test_iobase_File04_LDFLAGS			= -lclasslib

test_iobase_File05_SOURCES			= test/iobase/File05.cpp
test_iobase_File05_LDFLAGS			= -lclasslib

test_iobase_File06_SOURCES			= test/iobase/File06.cpp
test_iobase_File06_LDFLAGS			= -lclasslib

test_iobase_FileAcl01_SOURCES			= test/iobase/FileAcl01.cpp
test_iobase_FileAcl01_LDFLAGS			= -lclasslib

test_iobase_FileLock01_SOURCES			= test/iobase/FileLock01.cpp
test_iobase_FileLock01_LDFLAGS			= -lclasslib

test_iobase_FileMapping01_SOURCES		= test/iobase/FileMapping01.cpp
test_iobase_FileMapping01_LDFLAGS		= -lclasslib

test_iobase_FileMapping02_SOURCES		= test/iobase/FileMapping02.cpp
test_iobase_FileMapping02_LDFLAGS		= -lclasslib

test_iobase_FileMapping03_SOURCES		= test/iobase/FileMapping03.cpp
test_iobase_FileMapping03_LDFLAGS		= -lclasslib

test_iobase_Filename000_SOURCES			= test/iobase/Filename000.cpp
test_iobase_Filename000_LDFLAGS			= -lclasslib

test_iobase_Filename01_SOURCES			= test/iobase/Filename01.cpp
test_iobase_Filename01_LDFLAGS			= -lclasslib

test_iobase_Filename02_01_SOURCES		= test/iobase/Filename02_01.cpp
test_iobase_Filename02_01_LDFLAGS		= -lclasslib

test_iobase_Filename02_02_SOURCES		= test/iobase/Filename02_02.cpp
test_iobase_Filename02_02_LDFLAGS		= -lclasslib

test_iobase_Filename02_03_SOURCES		= test/iobase/Filename02_03.cpp
test_iobase_Filename02_03_LDFLAGS		= -lclasslib

test_iobase_Filename02_04_SOURCES		= test/iobase/Filename02_04.cpp
test_iobase_Filename02_04_LDFLAGS		= -lclasslib

test_iobase_Filename02_05_SOURCES		= test/iobase/Filename02_05.cpp
test_iobase_Filename02_05_LDFLAGS		= -lclasslib

test_iobase_Filename02_06_SOURCES		= test/iobase/Filename02_06.cpp
test_iobase_Filename02_06_LDFLAGS		= -lclasslib

test_iobase_Filename02_07_SOURCES		= test/iobase/Filename02_07.cpp
test_iobase_Filename02_07_LDFLAGS		= -lclasslib

test_iobase_Filename02_08_SOURCES		= test/iobase/Filename02_08.cpp
test_iobase_Filename02_08_LDFLAGS		= -lclasslib

test_iobase_Filename02_09_SOURCES		= test/iobase/Filename02_09.cpp
test_iobase_Filename02_09_LDFLAGS		= -lclasslib

test_iobase_Filename02_10_SOURCES		= test/iobase/Filename02_10.cpp
test_iobase_Filename02_10_LDFLAGS		= -lclasslib

test_iobase_Filename02_11_SOURCES		= test/iobase/Filename02_11.cpp
test_iobase_Filename02_11_LDFLAGS		= -lclasslib

test_iobase_Filename02_12_SOURCES		= test/iobase/Filename02_12.cpp
test_iobase_Filename02_12_LDFLAGS		= -lclasslib

test_iobase_Filename02_13_SOURCES		= test/iobase/Filename02_13.cpp
test_iobase_Filename02_13_LDFLAGS		= -lclasslib

test_iobase_Filename03_SOURCES			= test/iobase/Filename03.cpp
test_iobase_Filename03_LDFLAGS			= -lclasslib

test_iobase_Filename04_SOURCES			= test/iobase/Filename04.cpp
test_iobase_Filename04_LDFLAGS			= -lclasslib

test_iobase_Filename05_SOURCES			= test/iobase/Filename05.cpp
test_iobase_Filename05_LDFLAGS			= -lclasslib

test_iobase_Filename06_SOURCES			= test/iobase/Filename06.cpp
test_iobase_Filename06_LDFLAGS			= -lclasslib

test_iobase_Filename07_SOURCES			= test/iobase/Filename07.cpp
test_iobase_Filename07_LDFLAGS			= -lclasslib

test_iobase_Filename08_SOURCES			= test/iobase/Filename08.cpp
test_iobase_Filename08_LDFLAGS			= -lclasslib

test_iobase_Filename09_SOURCES			= test/iobase/Filename09.cpp
test_iobase_Filename09_LDFLAGS			= -lclasslib

test_iobase_IOChannel01_SOURCES			= test/iobase/IOChannel01.cpp
test_iobase_IOChannel01_LDFLAGS			= -lclasslib

test_iobase_IOSelector01_SOURCES		= test/iobase/IOSelector01.cpp
test_iobase_IOSelector01_LDFLAGS		= -lclasslib

test_iobase_IOSelector02_SOURCES		= test/iobase/IOSelector02.cpp
test_iobase_IOSelector02_LDFLAGS		= -lclasslib

test_iobase_IOSelector03_SOURCES		= test/iobase/IOSelector03.cpp
test_iobase_IOSelector03_LDFLAGS		= -lclasslib

test_iobase_IOStatus01_SOURCES			= test/iobase/IOStatus01.cpp
test_iobase_IOStatus01_LDFLAGS			= -lclasslib

test_iobase_InetServerSocket01_SOURCES		= test/iobase/InetServerSocket01.cpp
test_iobase_InetServerSocket01_LDFLAGS		= -lclasslib

test_iobase_InetSocket01_SOURCES		= test/iobase/InetSocket01.cpp
test_iobase_InetSocket01_LDFLAGS		= -lclasslib

test_iobase_LocalServerSocket01_SOURCES		= test/iobase/LocalServerSocket01.cpp
test_iobase_LocalServerSocket01_LDFLAGS		= -lclasslib

test_iobase_LocalSocket01_SOURCES		= test/iobase/LocalSocket01.cpp
test_iobase_LocalSocket01_LDFLAGS		= -lclasslib

test_iobase_MemoryStorage01_SOURCES		= test/iobase/MemoryStorage01.cpp
test_iobase_MemoryStorage01_LDFLAGS		= -lclasslib

test_iobase_NamedPipeServer01_SOURCES		= test/iobase/NamedPipeServer01.cpp
test_iobase_NamedPipeServer01_LDFLAGS		= -lclasslib

test_iobase_Pipe01_SOURCES			= test/iobase/Pipe01.cpp
test_iobase_Pipe01_LDFLAGS			= -lclasslib

test_iobase_PipeCmd01_SOURCES			= test/iobase/PipeCmd01.cpp
test_iobase_PipeCmd01_LDFLAGS			= -lclasslib

test_iobase_SocketPair01_SOURCES		= test/iobase/SocketPair01.cpp
test_iobase_SocketPair01_LDFLAGS		= -lclasslib

test_iobase_SubProcess01_SOURCES		= test/iobase/SubProcess01.cpp
test_iobase_SubProcess01_LDFLAGS		= -lclasslib

test_iobase_SubProcess02_SOURCES		= test/iobase/SubProcess02.cpp
test_iobase_SubProcess02_LDFLAGS		= -lclasslib

test_iobase_SubProcess03_SOURCES		= test/iobase/SubProcess03.cpp
test_iobase_SubProcess03_LDFLAGS		= -lclasslib

test_iobase_SubProcess04_SOURCES		= test/iobase/SubProcess04.cpp
test_iobase_SubProcess04_LDFLAGS		= -lclasslib

test_iobase_SubProcess05_SOURCES		= test/iobase/SubProcess05.cpp
test_iobase_SubProcess05_LDFLAGS		= -lclasslib

test_iobase_SubStorage01_SOURCES		= test/iobase/SubStorage01.cpp
test_iobase_SubStorage01_LDFLAGS		= -lclasslib

test_iobase_SubStorage02_SOURCES		= test/iobase/SubStorage02.cpp
test_iobase_SubStorage02_LDFLAGS		= -lclasslib

test_iobase_SubStorage03_SOURCES		= test/iobase/SubStorage03.cpp
test_iobase_SubStorage03_LDFLAGS		= -lclasslib

test_iobase_SubStorage04_SOURCES		= test/iobase/SubStorage04.cpp
test_iobase_SubStorage04_LDFLAGS		= -lclasslib

test_iobase_SubStorage05_SOURCES		= test/iobase/SubStorage05.cpp
test_iobase_SubStorage05_LDFLAGS		= -lclasslib

test_iotools_BufferInputStream01_SOURCES	= test/iotools/BufferInputStream01.cpp
test_iotools_BufferInputStream01_LDFLAGS	= -lclasslib

test_iotools_BufferOutputStream01_SOURCES	= test/iotools/BufferOutputStream01.cpp
test_iotools_BufferOutputStream01_LDFLAGS	= -lclasslib

test_iotools_CheckedInputStream01_SOURCES	= test/iotools/CheckedInputStream01.cpp
test_iotools_CheckedInputStream01_LDFLAGS	= -lclasslib

test_iotools_CheckedOutputStream01_SOURCES	= test/iotools/CheckedOutputStream01.cpp
test_iotools_CheckedOutputStream01_LDFLAGS	= -lclasslib

test_iotools_FilterInputStream01_SOURCES	= test/iotools/FilterInputStream01.cpp
test_iotools_FilterInputStream01_LDFLAGS	= -lclasslib

test_iotools_FilterOutputStream01_SOURCES	= test/iotools/FilterOutputStream01.cpp
test_iotools_FilterOutputStream01_LDFLAGS	= -lclasslib

test_iotools_IOChannelInputStream01_SOURCES	= test/iotools/IOChannelInputStream01.cpp
test_iotools_IOChannelInputStream01_LDFLAGS	= -lclasslib

test_iotools_IOChannelOutputStream01_SOURCES	= test/iotools/IOChannelOutputStream01.cpp
test_iotools_IOChannelOutputStream01_LDFLAGS	= -lclasslib

test_iotools_PipedInputStream01_SOURCES		= test/iotools/PipedInputStream01.cpp
test_iotools_PipedInputStream01_LDFLAGS		= -lclasslib

test_iotools_PipedOutputStream01_SOURCES	= test/iotools/PipedOutputStream01.cpp
test_iotools_PipedOutputStream01_LDFLAGS	= -lclasslib

test_iotools_PushBackInputStream01_SOURCES	= test/iotools/PushBackInputStream01.cpp
test_iotools_PushBackInputStream01_LDFLAGS	= -lclasslib

test_iotools_StdInputStream01_SOURCES		= test/iotools/StdInputStream01.cpp
test_iotools_StdInputStream01_LDFLAGS		= -lclasslib

test_iotools_StdInputStream02_SOURCES		= test/iotools/StdInputStream02.cpp
test_iotools_StdInputStream02_LDFLAGS		= -lclasslib

test_iotools_StdOutputStream01_SOURCES		= test/iotools/StdOutputStream01.cpp
test_iotools_StdOutputStream01_LDFLAGS		= -lclasslib

test_iotools_StdStorage01_SOURCES		= test/iotools/StdStorage01.cpp
test_iotools_StdStorage01_LDFLAGS		= -lclasslib

test_iotools_StdStorage02_SOURCES		= test/iotools/StdStorage02.cpp
test_iotools_StdStorage02_LDFLAGS		= -lclasslib

test_iotools_StdStorage03_SOURCES		= test/iotools/StdStorage03.cpp
test_iotools_StdStorage03_LDFLAGS		= -lclasslib

test_iotools_StdStorage04_SOURCES		= test/iotools/StdStorage04.cpp
test_iotools_StdStorage04_LDFLAGS		= -lclasslib

test_iotools_StdioStorage01_SOURCES		= test/iotools/StdioStorage01.cpp
test_iotools_StdioStorage01_LDFLAGS		= -lclasslib

test_iotools_StdioStorage02_SOURCES		= test/iotools/StdioStorage02.cpp
test_iotools_StdioStorage02_LDFLAGS		= -lclasslib

test_iotools_StdioStorage03_SOURCES		= test/iotools/StdioStorage03.cpp
test_iotools_StdioStorage03_LDFLAGS		= -lclasslib

test_iotools_StdioStorage04_SOURCES		= test/iotools/StdioStorage04.cpp
test_iotools_StdioStorage04_LDFLAGS		= -lclasslib

test_iotools_StorageInputStream01_SOURCES	= test/iotools/StorageInputStream01.cpp
test_iotools_StorageInputStream01_LDFLAGS	= -lclasslib

test_iotools_StorageOutputStream01_SOURCES	= test/iotools/StorageOutputStream01.cpp
test_iotools_StorageOutputStream01_LDFLAGS	= -lclasslib

test_utils_Argz01_SOURCES			= test/utils/Argz01.cpp
test_utils_Argz01_LDFLAGS			= -lclasslib

test_utils_AutoLoad01_SOURCES			= test/utils/AutoLoad01.cpp
test_utils_AutoLoad01_LDFLAGS			= -lclasslib

test_utils_BitIterator01_SOURCES		= test/utils/BitIterator01.cpp
test_utils_BitIterator01_LDFLAGS		= -lclasslib

test_utils_BitOps01_SOURCES			= test/utils/BitOps01.cpp
test_utils_BitOps01_LDFLAGS			= -lclasslib

test_utils_BitPattern01_SOURCES			= test/utils/BitPattern01.cpp
test_utils_BitPattern01_LDFLAGS			= -lclasslib

test_utils_BitTraits01_SOURCES			= test/utils/BitTraits01.cpp
test_utils_BitTraits01_LDFLAGS			= -lclasslib

test_utils_Callback01_SOURCES			= test/utils/Callback01.cpp
test_utils_Callback01_LDFLAGS			= -lclasslib

test_utils_Debug01_SOURCES			= test/utils/Debug01.cpp
test_utils_Debug01_LDFLAGS			= -lclasslib

test_utils_Environment01_SOURCES		= test/utils/Environment01.cpp
test_utils_Environment01_LDFLAGS		= -lclasslib

test_utils_Hook01_SOURCES			= test/utils/Hook01.cpp
test_utils_Hook01_LDFLAGS			= -lclasslib

test_utils_HookList01_SOURCES			= test/utils/HookList01.cpp
test_utils_HookList01_LDFLAGS			= -lclasslib

test_utils_HostInfo01_SOURCES			= test/utils/HostInfo01.cpp
test_utils_HostInfo01_LDFLAGS			= -lclasslib

test_utils_IntBits01_SOURCES			= test/utils/IntBits01.cpp
test_utils_IntBits01_LDFLAGS			= -lclasslib

test_utils_IntBits02_SOURCES			= test/utils/IntBits02.cpp
test_utils_IntBits02_LDFLAGS			= -lclasslib

test_utils_IntBits03_SOURCES			= test/utils/IntBits03.cpp
test_utils_IntBits03_LDFLAGS			= -lclasslib

test_utils_Log01_SOURCES			= test/utils/Log01.cpp
test_utils_Log01_LDFLAGS			= -lclasslib

test_utils_LogListener_SOURCES			= test/utils/LogListener.cpp
test_utils_LogListener_LDFLAGS			= -lclasslib

test_utils_LowMemoryPool01_SOURCES		= test/utils/LowMemoryPool01.cpp
test_utils_LowMemoryPool01_LDFLAGS		= -lclasslib

test_utils_MapIterator01_SOURCES		= test/utils/MapIterator01.cpp
test_utils_MapIterator01_LDFLAGS		= -lclasslib

test_utils_MapIterator02_SOURCES		= test/utils/MapIterator02.cpp
test_utils_MapIterator02_LDFLAGS		= -lclasslib

test_utils_MultiMethod01_SOURCES		= test/utils/MultiMethod01.cpp
test_utils_MultiMethod01_LDFLAGS		= -lclasslib

test_utils_MultiMethod02_SOURCES		= test/utils/MultiMethod02.cpp
test_utils_MultiMethod02_LDFLAGS		= -lclasslib

test_utils_MultiMethod03_SOURCES		= test/utils/MultiMethod03.cpp
test_utils_MultiMethod03_LDFLAGS		= -lclasslib

test_utils_ProcessInfo01_SOURCES		= test/utils/ProcessInfo01.cpp
test_utils_ProcessInfo01_LDFLAGS		= -lclasslib

test_utils_Range01_SOURCES			= test/utils/Range01.cpp
test_utils_Range01_LDFLAGS			= -lclasslib

test_utils_ResourceInfo01_SOURCES		= test/utils/ResourceInfo01.cpp
test_utils_ResourceInfo01_LDFLAGS		= -lclasslib

test_utils_SaveErrno01_SOURCES			= test/utils/SaveErrno01.cpp
test_utils_SaveErrno01_LDFLAGS			= -lclasslib

test_utils_SearchPath01_SOURCES			= test/utils/SearchPath01.cpp
test_utils_SearchPath01_LDFLAGS			= -lclasslib

test_utils_SearchPath02_SOURCES			= test/utils/SearchPath02.cpp
test_utils_SearchPath02_LDFLAGS			= -lclasslib

test_utils_Sequence01_SOURCES			= test/utils/Sequence01.cpp
test_utils_Sequence01_LDFLAGS			= -lclasslib

test_utils_SharedLibrary01_SOURCES		= test/utils/SharedLibrary01.cpp
test_utils_SharedLibrary01_LDFLAGS		= -lclasslib

libSharedLibrary01Lib_la_SOURCES		= test/utils/SharedLibrary01Lib.cpp

test_utils_ShellEnvironment01_SOURCES		= test/utils/ShellEnvironment01.cpp
test_utils_ShellEnvironment01_LDFLAGS		= -lclasslib

test_utils_Signal01_SOURCES			= test/utils/Signal01.cpp
test_utils_Signal01_LDFLAGS			= -lclasslib

test_utils_Signal02_SOURCES			= test/utils/Signal02.cpp
test_utils_Signal02_LDFLAGS			= -lclasslib

test_utils_Signal03_SOURCES			= test/utils/Signal03.cpp
test_utils_Signal03_LDFLAGS			= -lclasslib

test_utils_StringFormat01_SOURCES		= test/utils/StringFormat01.cpp
test_utils_StringFormat01_LDFLAGS		= -lclasslib

test_utils_StringOps01_SOURCES			= test/utils/StringOps01.cpp
test_utils_StringOps01_LDFLAGS			= -lclasslib

test_utils_StringOps02_SOURCES			= test/utils/StringOps02.cpp
test_utils_StringOps02_LDFLAGS			= -lclasslib

test_utils_StringOps03_SOURCES			= test/utils/StringOps03.cpp
test_utils_StringOps03_LDFLAGS			= -lclasslib

test_utils_StringOps04_SOURCES			= test/utils/StringOps04.cpp
test_utils_StringOps04_LDFLAGS			= -lclasslib

test_utils_SystemInfo01_SOURCES			= test/utils/SystemInfo01.cpp
test_utils_SystemInfo01_LDFLAGS			= -lclasslib

test_utils_Time01_SOURCES			= test/utils/Time01.cpp
test_utils_Time01_LDFLAGS			= -lclasslib

test_utils_Time02_SOURCES			= test/utils/Time02.cpp
test_utils_Time02_LDFLAGS			= -lclasslib

test_utils_Time03_SOURCES			= test/utils/Time03.cpp
test_utils_Time03_LDFLAGS			= -lclasslib

test_utils_TimeInfo01_SOURCES			= test/utils/TimeInfo01.cpp
test_utils_TimeInfo01_LDFLAGS			= -lclasslib

test_utils_TimeInfo02_SOURCES			= test/utils/TimeInfo02.cpp
test_utils_TimeInfo02_LDFLAGS			= -lclasslib

test_utils_UUID01_SOURCES			= test/utils/UUID01.cpp
test_utils_UUID01_LDFLAGS			= -lclasslib

test_utils_UUID02_SOURCES			= test/utils/UUID02.cpp
test_utils_UUID02_LDFLAGS			= -lclasslib

test_utils_UUID03_SOURCES			= test/utils/UUID03.cpp
test_utils_UUID03_LDFLAGS			= -lclasslib

test_utils_UUID04_SOURCES			= test/utils/UUID04.cpp
test_utils_UUID04_LDFLAGS			= -lclasslib

test_utils_UUID05_SOURCES			= test/utils/UUID05.cpp
test_utils_UUID05_LDFLAGS			= -lclasslib

test_utils_Unwind01_SOURCES			= test/utils/Unwind01.cpp
test_utils_Unwind01_LDFLAGS			= -lclasslib

test_utils_UserInfo01_SOURCES			= test/utils/UserInfo01.cpp
test_utils_UserInfo01_LDFLAGS			= -lclasslib

test_utils_XTypeInfo01_SOURCES			= test/utils/XTypeInfo01.cpp
test_utils_XTypeInfo01_LDFLAGS			= -lclasslib

test_utils_exa_Debug_SOURCES			= test/utils/exa_Debug.cpp
test_utils_exa_Debug_LDFLAGS			= -lclasslib

test_utils_exa_Log_SOURCES			= test/utils/exa_Log.cpp
test_utils_exa_Log_LDFLAGS			= -lclasslib

test_utils_exa_ResourceInfo_SOURCES		= test/utils/exa_ResourceInfo.cpp
test_utils_exa_ResourceInfo_LDFLAGS		= -lclasslib

test_utils_exa_Signal_SOURCES			= test/utils/exa_Signal.cpp
test_utils_exa_Signal_LDFLAGS			= -lclasslib

test_zip_Adler3201_SOURCES			= test/zip/Adler3201.cpp
test_zip_Adler3201_LDFLAGS			= -lclasslib

test_zip_BZCompressor01_SOURCES			= test/zip/BZCompressor01.cpp
test_zip_BZCompressor01_LDFLAGS			= -lclasslib

test_zip_BZDecompressor01_SOURCES		= test/zip/BZDecompressor01.cpp
test_zip_BZDecompressor01_LDFLAGS		= -lclasslib

test_zip_BZIPInputStream01_SOURCES		= test/zip/BZIPInputStream01.cpp
test_zip_BZIPInputStream01_LDFLAGS		= -lclasslib

test_zip_BZIPOutputStream01_SOURCES		= test/zip/BZIPOutputStream01.cpp
test_zip_BZIPOutputStream01_LDFLAGS		= -lclasslib

test_zip_CPIOInputStream01_SOURCES		= test/zip/CPIOInputStream01.cpp
test_zip_CPIOInputStream01_LDFLAGS		= -lclasslib

test_zip_CPIOInputStream02_SOURCES		= test/zip/CPIOInputStream02.cpp
test_zip_CPIOInputStream02_LDFLAGS		= -lclasslib

test_zip_CPIOOutputStream00_SOURCES		= test/zip/CPIOOutputStream00.cpp
test_zip_CPIOOutputStream00_LDFLAGS		= -lclasslib

test_zip_CPIOOutputStream01_SOURCES		= test/zip/CPIOOutputStream01.cpp
test_zip_CPIOOutputStream01_LDFLAGS		= -lclasslib

test_zip_CPIOOutputStream02_SOURCES		= test/zip/CPIOOutputStream02.cpp
test_zip_CPIOOutputStream02_LDFLAGS		= -lclasslib

test_zip_CPIOOutputStream03_SOURCES		= test/zip/CPIOOutputStream03.cpp
test_zip_CPIOOutputStream03_LDFLAGS		= -lclasslib

test_zip_CRC3201_SOURCES			= test/zip/CRC3201.cpp
test_zip_CRC3201_LDFLAGS			= -lclasslib

test_zip_GZIPInputStream01_SOURCES		= test/zip/GZIPInputStream01.cpp
test_zip_GZIPInputStream01_LDFLAGS		= -lclasslib

test_zip_GZIPInputStream02_SOURCES		= test/zip/GZIPInputStream02.cpp
test_zip_GZIPInputStream02_LDFLAGS		= -lclasslib

test_zip_GZIPOutputStream01_SOURCES		= test/zip/GZIPOutputStream01.cpp
test_zip_GZIPOutputStream01_LDFLAGS		= -lclasslib

test_zip_InteractiveCompressor01_SOURCES	= test/zip/InteractiveCompressor01.cpp
test_zip_InteractiveCompressor01_LDFLAGS	= -lclasslib

test_zip_InteractiveDecompressor01_SOURCES	= test/zip/InteractiveDecompressor01.cpp
test_zip_InteractiveDecompressor01_LDFLAGS	= -lclasslib

test_zip_MD5Digest01_SOURCES			= test/zip/MD5Digest01.cpp
test_zip_MD5Digest01_LDFLAGS			= -lclasslib

test_zip_SHA1Digest01_SOURCES			= test/zip/SHA1Digest01.cpp
test_zip_SHA1Digest01_LDFLAGS			= -lclasslib

test_zip_TarInputStream01_SOURCES		= test/zip/TarInputStream01.cpp
test_zip_TarInputStream01_LDFLAGS		= -lclasslib

test_zip_TarInputStream02_SOURCES		= test/zip/TarInputStream02.cpp
test_zip_TarInputStream02_LDFLAGS		= -lclasslib

test_zip_TarInputStream03_SOURCES		= test/zip/TarInputStream03.cpp
test_zip_TarInputStream03_LDFLAGS		= -lclasslib

test_zip_TarOutputStream00_SOURCES		= test/zip/TarOutputStream00.cpp
test_zip_TarOutputStream00_LDFLAGS		= -lclasslib

test_zip_TarOutputStream01_SOURCES		= test/zip/TarOutputStream01.cpp
test_zip_TarOutputStream01_LDFLAGS		= -lclasslib

test_zip_ZCompressor01_SOURCES			= test/zip/ZCompressor01.cpp
test_zip_ZCompressor01_LDFLAGS			= -lclasslib

test_zip_ZDecompressor01_SOURCES		= test/zip/ZDecompressor01.cpp
test_zip_ZDecompressor01_LDFLAGS		= -lclasslib

test_zip_ZInputStream01_SOURCES			= test/zip/ZInputStream01.cpp
test_zip_ZInputStream01_LDFLAGS			= -lclasslib

test_zip_ZOutputStream01_SOURCES		= test/zip/ZOutputStream01.cpp
test_zip_ZOutputStream01_LDFLAGS		= -lclasslib

test_zip_ZipArchive01_SOURCES			= test/zip/ZipArchive01.cpp
test_zip_ZipArchive01_LDFLAGS			= -lclasslib

test_zip_ZipArchive02_SOURCES			= test/zip/ZipArchive02.cpp
test_zip_ZipArchive02_LDFLAGS			= -lclasslib

test_zip_ZipArchive03_SOURCES			= test/zip/ZipArchive03.cpp
test_zip_ZipArchive03_LDFLAGS			= -lclasslib

test_zip_ZipArchive04_SOURCES			= test/zip/ZipArchive04.cpp
test_zip_ZipArchive04_LDFLAGS			= -lclasslib

test_zip_ZipArchive05_SOURCES			= test/zip/ZipArchive05.cpp
test_zip_ZipArchive05_LDFLAGS			= -lclasslib

test_zip_ZipArchive06_SOURCES			= test/zip/ZipArchive06.cpp
test_zip_ZipArchive06_LDFLAGS			= -lclasslib

test_zip_ZipArchive07_SOURCES			= test/zip/ZipArchive07.cpp
test_zip_ZipArchive07_LDFLAGS			= -lclasslib

test_zip_ZipArchive08_SOURCES			= test/zip/ZipArchive08.cpp
test_zip_ZipArchive08_LDFLAGS			= -lclasslib

test_zip_ZipInputStream01_SOURCES		= test/zip/ZipInputStream01.cpp
test_zip_ZipInputStream01_LDFLAGS		= -lclasslib

test_zip_ZipInputStream02_SOURCES		= test/zip/ZipInputStream02.cpp
test_zip_ZipInputStream02_LDFLAGS		= -lclasslib

test_zip_ZipInputStream03_SOURCES		= test/zip/ZipInputStream03.cpp
test_zip_ZipInputStream03_LDFLAGS		= -lclasslib

test_zip_ZipOutputStream00_SOURCES		= test/zip/ZipOutputStream00.cpp
test_zip_ZipOutputStream00_LDFLAGS		= -lclasslib

test_zip_ZipOutputStream01_SOURCES		= test/zip/ZipOutputStream01.cpp
test_zip_ZipOutputStream01_LDFLAGS		= -lclasslib

test_zip_ZipUtils01_SOURCES			= test/zip/ZipUtils01.cpp
test_zip_ZipUtils01_LDFLAGS			= -lclasslib

check-programs: $(check_PROGRAMS)
