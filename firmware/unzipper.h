#ifndef _BOB_
#define _BOB_

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "math.h"
#include <string.h>

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t   i8;
typedef int16_t  i16;
typedef int32_t  i32;
typedef int64_t  i64;

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0') 


#define U5FS_SUPERBLOCK_MAGIC 0x55354653LU /* "U5FS" */

struct u5fs_superblock_v1{
    u32 magic; /* Must contain U5FS_SUPERBLOCK_MAGIC */
    u32 version; /* Must contain U5FS_VERSION_1 for v1 */
    u32 blocksize; /* Block size , in bytes */
    u32 blockcount; /* Total number of blocks , including superblock */
    u32 rootnode; /* Index of root node block */
};

struct cred {
    u32 uid;
    u32 gid;
};

struct timespec {
    u32 time_sec;   /* number of whole seconds since 00:00:00 , January 1st, 1970 */
    u32 time_nsec;  /* fractional part of second , in nanoseconds */
};



struct u5fs_inode {
    u32 indirection;        /* pointer to 2nd-level block node , for large files */
    struct cred owner;      /* uid/gid of owner */
    struct timespec atime;  /* time of last file access */
    struct timespec mtime;  /* time of last file modification */
    struct timespec ctime;  /* time of last metadata change (chown/chmod/etc) */
    u16 perm;               /* permission bits */
    u16 links;              /* number of references to this file */
    u32 size;               /* file size in bytes */
};

struct u5fs_file_inode {
    struct u5fs_inode header;   /* file inodes start with the common u5fs_inode fields. */
    u32 blocks[];               /* ..followed by a list of all data blocks that back the file. */
};

struct u5fs_dir_inode {
    struct u5fs_inode header;    /* dir inodes start with the common u5fs_inode fields. */
    u8 dirent[];                 /* ..followed by a variable -length list of directory entries. */
                                /* (the .dirent data structure is header.size bytes long) */
                                /* see: struct u5fs_dentry */
};

struct u5fs_device_inode {
    struct u5fs_inode header;   /* dir inodes start with the common u5fs_inode fields */
    u16 padding;                /* for historical reasons , these 2 bytes are unused */
    u16 major;                  /* for device nodes: major number */
    u16 minor;                  /* for device nodes: minor number */
};

struct u5fs_symlink_inode {
    struct u5fs_inode header;   /* symlink inodes start with the common u5fs_inode fields */
    u16 size;                   /* symlink length */
    u8 name[];                  /* symlink name (NOT zero -terminated) */
};

struct u5fs_dentry {
    u32 dnode;  /* inode of file */
    u8 dtype;   /* enum u5fs_dnode_type */
    u8 name[];  /* zero -terminated string */
};

enum u5fs_version {
    U5FS_VERSION_0,     //* 0
    U5FS_VERSION_1      //* 1
};

enum u5fs_dtype {
    U5FS_DTYPE_NA,      //* 0 NOT USED
    U5FS_DTYPE_DIR,     //* 1
    U5FS_DTYPE_FILE,    //* 2
    U5FS_DTYPE_CDEV,    //* 3
    U5FS_DTYPE_BDEV,    //* 4
    U5FS_DTYPE_LNK,     //* 5
    U5FS_DTYPE_PIPE,    //* 6
    U5FS_DTYPE_SOCK     //* 7
};
#endif