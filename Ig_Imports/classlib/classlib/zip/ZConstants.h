#ifndef CLASSLIB_Z_CONSTANTS_H
# define CLASSLIB_Z_CONSTANTS_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "classlib/zip/ZipUtils.h"

namespace lat {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Constants related to ZLIB compression and ZIP archives.  */
struct ZConstants : ZipUtils
{
    // ZLIB constants
    // - Compression method and strategy (FIXME: data type: ascii/binary?)
    enum Method {
	STORED				= 0, //< Stored as is
	DEFLATED			= 8  //< Stored using deflate
    };
    enum Strategy {
	DEFAULT				= 0, //< Default compression strategy
	FILTERED			= 1, //< FIXME
	HUFFMAN_ONLY			= 2  //< FIXME
    };

    // - Compression levels
    enum { BEST_COMPRESSION		= 9 }; //< Best compression level
    enum { DEFAULT_COMPRESSION		= 6 }; //< Default compression level
    enum { BEST_SPEED			= 1 }; //< Fastest compression level

    // - Other settings
    enum { DEFAULT_WINDOW_BITS		= 15 };
    enum { DEFAULT_MEM_LEVEL		= 8 };

    // ZIP archive constants
    // - Archive version required (and made)
    enum { ARCHIVE_VERSION		= 20 };

    // - Magic header constants
    enum { DIR_HEADER_MAGIC		= 0x02014b50 };
    enum { ENTRY_HEADER_MAGIC		= 0x04034b50 };
    enum { END_HEADER_MAGIC		= 0x06054b50 };
    enum { ZIP_MAGIC_LEN		= 4 };
    enum { MAX_VAR_LEN			= 0xffff };	//< Max variable-width field length
    enum { MAX_SIZE			= 0xffffffff };	//< Max size of things


    // - Offsets into the central directory headers
    enum {
	DIR_MAGIC_OFF		= 0,
	DIR_VMADE_OFF		= 4,	DIR_VMADE_LEN		= 2,
	DIR_VREQD_OFF		= 6,	DIR_VREQD_LEN		= 2,
	DIR_FLAG_OFF		= 8,	DIR_FLAG_LEN		= 2,
	DIR_METHOD_OFF		= 10,	DIR_METHOD_LEN		= 2,
	DIR_DATE_OFF		= 12,	DIR_DATE_LEN		= 4,
	DIR_CRC32_OFF		= 16,	DIR_CRC32_LEN		= 4,
	DIR_CSIZE_OFF		= 20,	DIR_CSIZE_LEN		= 4,
	DIR_USIZE_OFF		= 24,	DIR_USIZE_LEN		= 4,
	DIR_NAMELEN_OFF		= 28,	DIR_NAMELEN_LEN		= 2,
	DIR_EXTRALEN_OFF	= 30,	DIR_EXTRALEN_LEN	= 2,
	DIR_COMMENTLEN_OFF	= 32,	DIR_COMMENTLEN_LEN	= 2,
	DIR_DISK_START_OFF	= 34,	DIR_DISK_START_LEN	= 2,
	DIR_INT_ATTR_OFF	= 36,	DIR_INT_ATTR_LEN	= 2,
	DIR_EXT_ATTR_OFF	= 38,	DIR_EXT_ATTR_LEN	= 4,
	DIR_ENTRY_POS_OFF	= 42,	DIR_ENTRY_POS_LEN	= 4,
	DIR_HEADER_SIZE		= 46
    };

    // - Offsets into the end-of-archive header
    enum{
	END_MAGIC_OFF		= 0,
	END_DISK_OFF		= 4,	END_DISK_LEN		= 2,
	END_DIR_DISK_OFF	= 6,	END_DIR_DISK_LEN	= 2,
	END_DISK_HDRS_OFF	= 8,	END_DISK_HDRS_LEN	= 2,
	END_TOTAL_HDRS_OFF	= 10,	END_TOTAL_HDRS_LEN	= 2,
	END_DIR_SIZE_OFF	= 12,	END_DIR_SIZE_LEN	= 4,
	END_DIR_OFFSET_OFF	= 16,	END_DIR_OFFSET_LEN	= 4,
	END_COMMENTLEN_OFF	= 20,	END_COMMENTLEN_LEN	= 2,
	END_HEADER_SIZE		= 22
    };

    // - Offsets into the local entry headers
    enum {
	ENTRY_MAGIC_OFF		= 0,
	ENTRY_VREQD_OFF		= 4,	ENTRY_VREQD_LEN		= 2,
	ENTRY_FLAG_OFF		= 6,	ENTRY_FLAG_LEN		= 2,
	ENTRY_METHOD_OFF	= 8,	ENTRY_METHOD_LEN	= 2,
	ENTRY_DATE_OFF		= 10,	ENTRY_DATE_LEN		= 4,
	ENTRY_CRC32_OFF		= 14,	ENTRY_CRC32_LEN		= 4,
	ENTRY_CSIZE_OFF		= 18,	ENTRY_CSIZE_LEN		= 4,
	ENTRY_USIZE_OFF		= 22,	ENTRY_USIZE_LEN		= 4,
	ENTRY_NAMELEN_OFF	= 26,	ENTRY_NAMELEN_LEN	= 2,
	ENTRY_EXTRALEN_OFF	= 28,	ENTRY_EXTRALEN_LEN	= 2,
	ENTRY_HEADER_SIZE	= 30
    };

    // GZIP file format constants
    enum { GZIP_MAGIC = 0x8b1f };
    enum {
	GZHDR_MAGIC_OFF  = 0, GZHDR_MAGIC_LEN  = 2,
	GZHDR_METHOD_OFF = 2, GZHDR_METHOD_LEN = 1,
	GZHDR_FLAGS_OFF  = 3, GZHDR_FLAGS_LEN  = 1,
	GZHDR_TIME_OFF   = 4, GZHDR_TIME_LEN   = 4,
	GZHDR_XFLAGS_OFF = 8, GZHDR_XFLAGS_LEN = 1,
	GZHDR_OS_OFF     = 9, GZHDR_OS_LEN     = 1,
	GZHDR_SIZE       = 10,

	// The standard header can be followed by:
	//   - extra field: 2-byte len + data (flags & EXTRA_FIELD)
	//   - original file name: EOF-terminated string (flags & ORIG_NAME)
	//   - comment: EOF-terminaed string (flags & COMMENT)
	//   - header crc: 2-byte value (flags & HEAD_CRC)
	//
	// This is followed by data, and finally 4-byte CRC-32 and a
	// 4-byte original uncompressed data length (FIXME: .gz files
	// can be concatenated back-to-back, need to reset total_out
	// in z_stream and reinit crc32).

	GZHDR_FLAG_ASCII	= 1,  // bit 0: file is probably ascii text
	GZHDR_FLAG_HEAD_CRC	= 2,  // bit 1: header crc16 present
	GZHDR_FLAG_EXTRA_FIELD	= 4,  // bit 2: extra field present
	GZHDR_FLAG_ORIG_NAME	= 8,  // bit 3: original file name present
	GZHDR_FLAG_COMMENT	= 16, // bit 4: file comment present
	GZHDR_FLAG_RESERVED	= 0xe0 // bits 5-7: reserved
    };
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace lat
#endif // CLASSLIB_Z_CONSTANTS_H
