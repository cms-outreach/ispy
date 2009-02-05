#ifndef CLASSLIB_CPIO_CONSTANTS_H
# define CLASSLIB_CPIO_CONSTANTS_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/zip/ZipUtils.h"

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Constants related to CPIO archives.  */
struct CPIOConstants : ZipUtils
{
    // CPIO archive formats are not very standard.  Apparently POSIX.1
    // does define it (though I haven't yet found the specs).  The ASCII
    // version has standard mode bits.  The binary format is completely
    // platform-specific, including the byte order (see stat(2)).

    // Header magic code and trailer string
    static const char *TRAILER;		//< Magic last-member file name
    enum {
	ODC_MAGIC	= 070707,	//< POSIX.1 archive
	SVR4_MAGIC	= 070701,	//< SVR4 archive
	SVR4C_MAGIC	= 070702	//< SVR4 archive with checksum
    };

    // Unix file mode bits
    enum Mode {
	SUID		= 04000,	//< Set-UID bit
	SGID		= 02000,	//< Set-GID bit
	SVTX		= 01000,	//< Sticky text bit
	UREAD		= 00400,	//< User read bit
	UWRITE		= 00200,	//< User write bit
	UEXEC		= 00100,	//< User execute bit
	GREAD		= 00040,	//< Group read bit
	GWRITE		= 00020,	//< Group write bit
	GEXEC		= 00010,	//< Group execute bit
	OREAD		= 00004,	//< Other read bit
	OWRITE		= 00002,	//< Other write bit
	OEXEC		= 00001		//< Other execute bit
    };

    // Offsets into the archive header: ascii and binary
    enum {
	MAGIC_LEN		= 6,
	TRAILER_LEN		= 11,

	ODC_MAGIC_OFFSET	= 0,	ODC_MAGIC_LENGTH	= 6,
	ODC_DEV_OFFSET		= 6,	ODC_DEV_LENGTH		= 6,
	ODC_INO_OFFSET		= 12,	ODC_INO_LENGTH		= 6,
	ODC_MODE_OFFSET		= 18,	ODC_MODE_LENGTH		= 6,
	ODC_UID_OFFSET		= 24,	ODC_UID_LENGTH		= 6,
	ODC_GID_OFFSET		= 30,	ODC_GID_LENGTH		= 6,
	ODC_NLINK_OFFSET	= 36,	ODC_NLINK_LENGTH	= 6,
	ODC_RDEV_OFFSET		= 42,	ODC_RDEV_LENGTH		= 6,
	ODC_MTIME_OFFSET	= 48,	ODC_MTIME_LENGTH	= 11,
	ODC_NAMELEN_OFFSET	= 59,	ODC_NAMELEN_LENGTH	= 6,
	ODC_SIZE_OFFSET		= 65,	ODC_SIZE_LENGTH		= 11,
	ODC_HEADER_LEN		= 76,

	SVR_MAGIC_OFFSET	= 0,	SVR_MAGIC_LENGTH	= 6,
	SVR_INO_OFFSET		= 6,	SVR_INO_LENGTH		= 8,
	SVR_MODE_OFFSET		= 14,	SVR_MODE_LENGTH		= 8,
	SVR_UID_OFFSET		= 22,	SVR_UID_LENGTH		= 8,
	SVR_GID_OFFSET		= 30,	SVR_GID_LENGTH		= 8,
	SVR_NLINK_OFFSET	= 38,	SVR_NLINK_LENGTH	= 8,
	SVR_MTIME_OFFSET	= 46,	SVR_MTIME_LENGTH	= 8,
	SVR_SIZE_OFFSET		= 54,	SVR_SIZE_LENGTH		= 8,
	SVR_DEVMAJOR_OFFSET	= 62,	SVR_DEVMAJOR_LENGTH	= 8,
	SVR_DEVMINOR_OFFSET	= 70,	SVR_DEVMINOR_LENGTH	= 8,
	SVR_RDEVMAJOR_OFFSET	= 78,	SVR_RDEVMAJOR_LENGTH	= 8,
	SVR_RDEVMINOR_OFFSET	= 86,	SVR_RDEVMINOR_LENGTH	= 8,
	SVR_NAMELEN_OFFSET	= 94,	SVR_NAMELEN_LENGTH	= 8,
	SVR_CHECKSUM_OFFSET	= 102,	SVR_CHECKSUM_LENGTH	= 8,
	SVR_HEADER_LEN		= 110,
	SVR_TRAILER_PAD		= 3,

	BIN_MAGIC_OFFSET	= 0,	BIN_MAGIC_LENGTH	= 2,  // short
	BIN_DEV_OFFSET		= 2,	BIN_DEV_LENGTH		= 2,  // short
	BIN_INO_OFFSET		= 4,	BIN_INO_LENGTH		= 2,  // ushort
	BIN_MODE_OFFSET		= 6,	BIN_MODE_LENGTH		= 2,  // ushort
	BIN_UID_OFFSET		= 8,	BIN_UID_LENGTH		= 2,  // ushort
	BIN_GID_OFFSET		= 10,	BIN_GID_LENGTH		= 2,  // ushort
	BIN_NLINK_OFFSET	= 12,	BIN_NLINK_LENGTH	= 2,  // short
	BIN_RDEV_OFFSET		= 14,	BIN_RDEV_LENGTH		= 2,  // short
	BIN_MTIME_OFFSET	= 16,	BIN_MTIME_LENGTH	= 4,  // short[2]
	BIN_NAMELEN_OFFSET	= 20,	BIN_NAMELEN_LENGTH	= 2,  // short
	BIN_FILELEN_OFFSET	= 22,	BIN_FILELEN_LENGTH	= 4,  // short[2]

	BIN_HEADER_LEN		= 26,

	// Maximum values for numeric values
	SMALL_MAX		= 0777777,
	LARGE_MAX		= 07777777777,
	SVR_NAMELEN_MAX		= 1024
    };
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace lat
#endif // CLASSLIB_CPIO_CONSTANTS_H
