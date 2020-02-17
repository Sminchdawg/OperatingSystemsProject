#ifndef PARTITIONFILE_H
#define PARTITIONFILE_H
/*
struct PartitionFile {
    int fd;
    VDIHeader header;
    uint32_t *map;
    int64_t cursor;
};

struct PartitionFile *partitionOpen(struct VDIFile *,struct PartitionEntry);
void partitionClose(struct PartitionFile *f);
ssize_t partitionRead(struct PartitionFile *f,void *buf,size_t count);
ssize_t partitionWrite(struct PartitionFile *f,void *buf,size_t count);
off_t partitionSeek(struct PartitionFile *f,off_t offset,int anchor);
*/
#endif // PARTITIONFILE_H
