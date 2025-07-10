/*
 * mkfs_main.c
 * ---------------------------
 * Simple front-end that calls tfs_format().
 *
 * Usage:
 *      ./mkfs.tfs <image-file> <blocks>
 * Example:
 *      ./mkfs.tfs disk.img 256   # 256 × 4096-byte blocks ≈ 1 MiB
 */
#include "tinyfs.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (argc != 3) {
        fprintf(stderr,
            "Usage: %s <image-file> <total-blocks>\n"
            "Example: %s disk.img 256\n",
            argv[0], argv[0]);
        return 1;
    }

    const char *img  = argv[1];
    uint32_t blocks  = (uint32_t) strtoul(argv[2], NULL, 0);

    if (blocks == 0) {
        fprintf(stderr, "total-blocks must be > 0\n");
        return 1;
    }

    if (tfs_format(img, blocks) != 0) {
        perror("tfs_format");
        return 1;
    }

    printf("TinyFS image '%s' created: %u blocks (%.2f MiB)\n",
           img, blocks, blocks * TFS_BLOCK_SIZE / 1048576.0);
    return 0;
}