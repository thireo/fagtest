#include "unzipper.h"
#include "math.h"

//#pragma scalar_storage_order big-endian

union byte4 {
        char byte[4];
        int numint;
        float numfloat;
};
int endianSwap4int(int a);

int main(void)
{
    u32 buffer;
    FILE * file_pointer = fopen("firmware.img","rb");
    int i = 0;
    printf("%lu",(u32) endianSwap4int(U5FS_SUPERBLOCK_MAGIC));
    
    
    u32 sb_addr = 0;
    u32 sb_ver = 0;
    u32 sb_size = 0;
    u32 sb_count = 0;
    u32 root_addr = 0;
    u32 bm_count = 0;

    while(fread(&buffer,1,4,file_pointer) == 4)
    {
        //printf("%lu\n",buffer);
        
        if(buffer == (u32) endianSwap4int(U5FS_SUPERBLOCK_MAGIC))// || buffer == 1397110101)
        {
            printf("\n%lu : 0x%08x",i, endianSwap4int(buffer)); // prints a series of bytes
            sb_addr = i;
            //return 1;
        }
        else if(sb_addr != 0 && i == sb_addr+1)
        {
            printf("\n%lu : 0x%08x",i, endianSwap4int(buffer)); // prints a series of bytes
            sb_ver = endianSwap4int(buffer);
        }
        else if(sb_addr != 0 && i == sb_addr+2)
        {
            printf("\n%lu : 0x%08x",i, endianSwap4int(buffer)); // prints a series of bytes
            sb_size = endianSwap4int(buffer);
        }
        else if(sb_addr != 0 && i == sb_addr+3)
        {
            printf("\n%lu : 0x%08x",i, endianSwap4int(buffer)); // prints a series of bytes
            sb_count = endianSwap4int(buffer);
        }
        else if(sb_addr != 0 && i == sb_addr+4)
        {
            printf("\n%lu : 0x%08x",i, endianSwap4int(buffer)); // prints a series of bytes
            root_addr = endianSwap4int(buffer);

            printf("\n---------------------\n");
            printf("SUPERBLOCK @\t%lu\n",sb_addr);
            printf("Version:\t%lu\n",sb_ver);
            printf("Block size:\t%lu\n",sb_size);
            printf("Block count:\t%lu\n",sb_count);
            bm_count = sb_count/(sb_size*8) + (sb_count % (sb_size*8) != 0); // Fast ceiling method returning int.
            printf("Bitmap Blocks:\t%lu\n",bm_count);
            printf("Rootnode:\t%lu @ 0x%08x\n",root_addr,root_addr*sb_size+sb_addr*4);
            printf("---------------------\n");

        }
        else if(sb_addr != 0 && i == sb_addr+sb_size)
        {
            printf("\n%lu : 0x%08x",i, endianSwap4int(buffer)); // prints a series of bytes
            u32 bitmaps[bm_count*sb_size];
            for(int k=0;k<sb_size;k++)
            {
                fread(&buffer,1,4,file_pointer);
                bitmaps[k] = endianSwap4int(buffer);
                //printf(BYTE_TO_BINARY_PATTERN"\n",BYTE_TO_BINARY(bitmaps[k]));
            }
        }
        else if(root_addr != 0 && i == root_addr*sb_size+sb_addr*4)
        {
            printf("\n%lu : 0x%08x\n",i, endianSwap4int(buffer)); // prints a series of bytes
            struct u5fs_dir_inode rootnode;
            
            rootnode.header.indirection = endianSwap4int(buffer);
            printf("0x%08x\n",rootnode.header.indirection);
            fread(&buffer,1,4,file_pointer);
            rootnode.header.owner.uid = endianSwap4int(buffer);
            printf("0x%08x\n",rootnode.header.owner.uid);
            fread(&buffer,1,4,file_pointer);
            rootnode.header.owner.gid = endianSwap4int(buffer);
            printf("0x%08x\n",rootnode.header.owner.gid);

            fread(&buffer,1,4,file_pointer);
            rootnode.header.atime.time_sec = endianSwap4int(buffer);
            fread(&buffer,1,4,file_pointer);
            rootnode.header.atime.time_nsec = endianSwap4int(buffer);
            printf("0x%08x\n",rootnode.header.atime.time_sec);
            
            fread(&buffer,1,4,file_pointer);
            rootnode.header.mtime.time_sec = endianSwap4int(buffer);
            fread(&buffer,1,4,file_pointer);
            rootnode.header.mtime.time_nsec = endianSwap4int(buffer);
            printf("0x%08x\n",rootnode.header.mtime.time_sec);

            fread(&buffer,1,4,file_pointer);
            rootnode.header.ctime.time_sec = endianSwap4int(buffer);
            fread(&buffer,1,4,file_pointer);
            rootnode.header.ctime.time_nsec = endianSwap4int(buffer);
            printf("0x%08x\n",rootnode.header.ctime.time_sec);

            fread(&buffer,1,2,file_pointer);
            rootnode.header.perm = endianSwap4int(buffer);
            printf("0x%08x\n",rootnode.header.links);

            fread(&buffer,1,2,file_pointer);
            rootnode.header.links = endianSwap4int(buffer);
            printf("0x%08x\n",rootnode.header.links);
            
            fread(&buffer,1,4,file_pointer);
            rootnode.header.size = endianSwap4int(buffer);
            printf("0x%08x\n",rootnode.header.size);

            fread(&buffer,1,4,file_pointer);
            u32 temp = endianSwap4int(buffer);
            printf("0x%08x\n",temp);

            return 1;
        }
        
        i++;
    }
    fclose(file_pointer);
}


int endianSwap4int(int a) 
{
    union byte4 un;
    int i;
    un.numint = a;

    /*printf("before:\n");
    for (i=0;i<4;i++) {
        printf("un[%d]=%02X\n", i, un.byte[i]);
    }*/

    // swap
    char c1 = un.byte[0];
    un.byte[0] = un.byte[3];
    un.byte[3] = c1;
    c1 = un.byte[1];
    un.byte[1] = un.byte[2];
    un.byte[2] = c1;

    /*printf("after:\n");
    for (i=0;i<4;i++) {
        printf("un[%d]=%02X\n", i, un.byte[i]);
    }*/
    return un.numint;
}