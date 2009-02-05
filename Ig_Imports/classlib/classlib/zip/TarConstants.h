#ifndef CLASSLIB_TAR_CONSTANTS_H
# define CLASSLIB_TAR_CONSTANTS_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/zip/ZipUtils.h"

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Constants related to TAR archives.  */
struct TarConstants : ZipUtils
{
    // Unix file mode bits
    enum Mode {
	SUID		= 04000,//< Set-UID bit
	SGID		= 02000,//< Set-GID bit
	SVTX		= 01000,//< Sticky text bit
	UREAD		= 00400,//< User read bit
	UWRITE		= 00200,//< User write bit
	UEXEC		= 00100,//< User execute bit
	GREAD		= 00040,//< Group read bit
	GWRITE		= 00020,//< Group write bit
	GEXEC		= 00010,//< Group execute bit
	OREAD		= 00004,//< Other read bit
	OWRITE		= 00002,//< Other write bit
	OEXEC		= 00001 //< Other execute bit
    };

    // Archive member type
    enum Type {
	REGULAR		= '0',	//< Regular file
	HARD_LINK	= '1',	//< Hard link to another archive member
	SYM_LINK	= '2',	//< Symbolic link: no data, only link
	CHAR_DEVICE	= '3',	//< Character device: no data, only dev numbers
	BLOCK_DEVICE	= '4',	//< Block device: no data, only dev numbers
	DIRECTORY	= '5',	//< Directory: no data
	FIFO		= '6',	//< FIFO: no data, only name
	CONTIGUOUS	= '7'	//< Contiguously stored (regular) file

	// Known but unsupported:
	//  GNU_DIR_DUMP = 'D'	//< Directory entry with the file names at
	// 			//  the time the dump was made
	//  GNU_LONGLINK = 'K'	//< Next member in archive has long link name
	//  GNU_LONGNAME = 'L'	//< Next member in archive has long name
	//  GNU_MULTIVOL = 'M'	//< Continuation of a file on another volume
	//  GNU_NAMES    = 'N'	//< Filenames that did not fit in the main hdr
	//  GNU_SPARSE   = 'S'	//< Sparse file
	//  GNU_VOLHDR   = 'V'	//< Tape/volume header; ignore on extraction
    };

    // Header magic and version number strings
    static const char	*MAGIC;	//< TAR archive magic (POSIX 1003.1-90)
    static const char	*VERSION;//< Header version identification string

    // Offsets into the central directory headers
    enum {
	NAME_OFFSET	= 0,	NAME_LENGTH	= 100,
	MODE_OFFSET	= 100,	MODE_LENGTH	= 8,
	UID_OFFSET	= 108,	UID_LENGTH	= 8,
	GID_OFFSET	= 116,	GID_LENGTH	= 8,
	SIZE_OFFSET	= 124,	SIZE_LENGTH	= 12,
	MTIME_OFFSET	= 136,	MTIME_LENGTH	= 12,
	CHKSUM_OFFSET	= 148,	CHKSUM_LENGTH	= 8,
	TYPE_OFFSET	= 156,	TYPE_LENGTH	= 1,
	LINK_OFFSET	= 157,	LINK_LENGTH	= 100,
	MAGIC_OFFSET	= 257,	MAGIC_LENGTH	= 6,
	VERSION_OFFSET	= 263,	VERSION_LENGTH	= 2,
	UNAME_OFFSET	= 265,	UNAME_LENGTH	= 32,
	GNAME_OFFSET	= 297,	GNAME_LENGTH	= 32,
	DEVMAJOR_OFFSET	= 329,	DEVMAJOR_LENGTH	= 8,
	DEVMINOR_OFFSET	= 329,	DEVMINOR_LENGTH	= 8,
	PREFIX_OFFSET	= 345,	PREFIX_LENGTH	= 155,

	HEADER_SIZE	= 512,

	// Maximum values for numeric values
	SMALL_MAX	= 0777777,	// 6 octal digits, space, null: uid...
	LARGE_MAX	= 07777777777   // 11 octal digits, space: size, mtime
    };
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace lat
#endif // CLASSLIB_TAR_CONSTANTS_H
