
#ifndef TINYFS_H
#define TINYFS_H

#include <stdint.h>
#include <time.h>

#define TFS_BLOCK_SIZE      4096
#define TFS_MAGIC           0x54465953  /* 'TFYS' */
#define TFS_MAX_INODES      256
#define TFS_DIRECT_PTRS     6

struct tfs_superblock {
    uint32_t magic;
    uint32_t total_blocks;
    uint32_t inode_table_start;
    uint32_t data_bitmap_start;
    uint32_t data_region_start;
};

struct tfs_inode {
    uint16_t mode;
    uint16_t links;
    uint32_t size;
    uint32_t direct[TFS_DIRECT_PTRS];
    uint64_t atime;
    uint64_t mtime;
};

/* API stubs */
int  tfs_format(const char *image_path, uint32_t total_blocks);
int  tfs_mount(const char *image_path);
void tfs_unmount(void);

int  tfs_open(const char *path, int flags, uint16_t mode);
ssize_t tfs_read(int fd, void *buf, size_t count);
ssize_t tfs_write(int fd, const void *buf, size_t count);
int  tfs_unlink(const char *path);
int  tfs_fsync(int fd);

#endif /* TINYFS_H */
