
/* tinyfs.c – minimal skeleton */

#include "tinyfs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

static int img_fd = -1;
static struct tfs_superblock sb;

/* block helpers */
static int read_block(uint32_t blk, void *buf){
    if(lseek(img_fd, blk * TFS_BLOCK_SIZE, SEEK_SET)<0) return -1;
    return read(img_fd, buf, TFS_BLOCK_SIZE);
}
static int write_block(uint32_t blk, const void *buf){
    if(lseek(img_fd, blk * TFS_BLOCK_SIZE, SEEK_SET)<0) return -1;
    return write(img_fd, buf, TFS_BLOCK_SIZE);
}

/* mkfs */
int tfs_format(const char *path, uint32_t total){
    int fd = open(path, O_RDWR|O_CREAT|O_TRUNC, 0644);
    if(fd<0){perror("open"); return -1;}
    if(lseek(fd, total*TFS_BLOCK_SIZE-1, SEEK_SET)<0){perror("lseek");return -1;}
    if(write(fd,"",1)!=1){perror("alloc");return -1;}

    struct tfs_superblock s={
        .magic=TFS_MAGIC,
        .total_blocks=total,
        .inode_table_start=1,
    };
    s.data_bitmap_start = 1 + (TFS_MAX_INODES*sizeof(struct tfs_inode)+TFS_BLOCK_SIZE-1)/TFS_BLOCK_SIZE;
    s.data_region_start = s.data_bitmap_start + 1;

    lseek(fd,0,SEEK_SET);
    write(fd,&s,sizeof s);
    close(fd);
    return 0;
}

/* mount/unmount */
int tfs_mount(const char *path){
    img_fd=open(path,O_RDWR);
    if(img_fd<0){perror("open img");return -1;}
    if(read(img_fd,&sb,sizeof sb)!=sizeof sb || sb.magic!=TFS_MAGIC){
        fprintf(stderr,"bad superblock\n");
        return -1;
    }
    return 0;
}
void tfs_unmount(void){ if(img_fd!=-1) close(img_fd); img_fd=-1; }

/* stubs */
int tfs_open(const char *p,int f,uint16_t m){return 3;}
ssize_t tfs_read(int fd,void *b,size_t c){return c;}
ssize_t tfs_write(int fd,const void *b,size_t c){return c;}
int tfs_unlink(const char *p){return 0;}
int tfs_fsync(int fd){return 0;}
