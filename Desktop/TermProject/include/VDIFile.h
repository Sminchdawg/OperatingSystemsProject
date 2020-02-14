#ifndef VDIFILE_H
#define VDIFILE_H
#include <cstdint>
#include <unistd.h>
#include <fcntl.h>

typedef struct VDIHeader
{
    // PRE HEADER
    /** Just text info about image type, for eyes only. */
    char            szFileInfo[64];
    /** The image signature (VDI_IMAGE_SIGNATURE). */
    uint32_t        u32Signature;
    /** The image version (VDI_IMAGE_VERSION). */
    uint32_t        u32Version;


    // VDIHEADER1PLUS
    /** Size of this structure in bytes. */
    uint32_t        cbHeader;
    /** The image type (VDI_IMAGE_TYPE_*). */
    uint32_t        u32Type;
    /** Image flags (VDI_IMAGE_FLAGS_*). */
    uint32_t        fFlags;
    /** Image comment. (UTF-8) */
    char            szComment[256];
    /** Offset of blocks array from the beginning of image file.
     * Should be sector-aligned for HDD access optimization. */
    uint32_t        offBlocks;
    /** Offset of image data from the beginning of image file.
     * Should be sector-aligned for HDD access optimization. */
    uint32_t        offData;
    /** Legacy image geometry (previous code stored PCHS there). */
    uint32_t LegacyGeometry[4];
    /** Was BIOS HDD translation mode, now unused. */
    uint32_t        u32Dummy;
    /** Size of disk (in bytes). */
    uint64_t        cbDisk;
    /** Block size. (For instance VDI_IMAGE_BLOCK_SIZE.) Should be a power of 2! */
    uint32_t        cbBlock;
    /** Size of additional service information of every data block.
     * Prepended before block data. May be 0.
     * Should be a power of 2 and sector-aligned for optimization reasons. */
    uint32_t        cbBlockExtra;
    /** Number of blocks. */
    uint32_t        cBlocks;
    /** Number of allocated blocks. */
    uint32_t        cBlocksAllocated;
    /** UUID of image. */
    uint8_t uuidCreate[16];
    /** UUID of image's last modification. */
    uint8_t uuidModify[16];
    /** Only for secondary images - UUID of previous image. */
    uint8_t uuidLinkage[16];
    /** Only for secondary images - UUID of previous image's last modification. */
    uint8_t uuidParentModify[16];
    /** LCHS image geometry (new field in VDI1.2 version. */
    uint32_t LCHSGeometry[4];
} VDIHEADER1PLUS, *PVDIHEADER1PLUS;

struct VDIFile {
    int fd;
    VDIHeader header;
    uint32_t *map;
    int64_t cursor;
};

struct VDIFile *vdiOpen(char *fn);
ssize_t vdiRead(struct VDIFile *f, void *buf, size_t count);
off_t vdiSeek(VDIFile *f, off_t offset, int anchor);
void vdiClose (struct VDIFile *f);

#endif // VDIFILE_H
