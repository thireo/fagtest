#include "unzipper.h"


//#pragma scalar_storage_order big-endian

union byte4 {
        char byte[4];
        int numint;
        float numfloat;
};
int endianSwap4int(int a);
u32 get_dentry(struct u5fs_dentry * dest, u8 * src);

int main(void)
{
    u32 buffer;
    FILE * file_pointer = fopen("firmware.img","rb");
    int i = 0;
    //printf("%lu",(u32) endianSwap4int(U5FS_SUPERBLOCK_MAGIC));

    u32 sb_addr = 0;
    u32 sb_ver = 0;
    u32 sb_size = 0;
    u32 sb_count = 0;
    u32 root_addr = 0;
    u32 bm_count = 0;

    struct u5fs_superblock_v1 sb;


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
            sb.version = endianSwap4int(buffer);
        }
        else if(sb_addr != 0 && i == sb_addr+2)
        {
            printf("\n%lu : 0x%08x",i, endianSwap4int(buffer)); // prints a series of bytes
            sb.blocksize = endianSwap4int(buffer);
        }
        else if(sb_addr != 0 && i == sb_addr+3)
        {
            printf("\n%lu : 0x%08x",i, endianSwap4int(buffer)); // prints a series of bytes
            sb.blockcount = endianSwap4int(buffer);
        }
        else if(sb_addr != 0 && i == sb_addr+4)
        {
            printf("\n%lu : 0x%08x",i, endianSwap4int(buffer)); // prints a series of bytes
            sb.rootnode = endianSwap4int(buffer);

            printf("\n---------------------\n");
            printf("SUPERBLOCK @\t%lu\n",sb_addr);
            printf("Version:\t%lu\n",sb.version);
            printf("Block size:\t%lu\n",sb.blocksize);
            printf("Block count:\t%lu\n",sb.blockcount);
            bm_count = sb.blockcount/(sb.blocksize*8) + (sb.blockcount % (sb.blocksize*8) != 0); // Fast ceiling method returning int.
            printf("Bitmap Blocks:\t%lu\n",bm_count);
            printf("Rootnode:\t%lu @ 0x%08x\n",sb.rootnode,sb.rootnode*sb.blocksize+sb_addr*4);
            printf("---------------------\n");
            break;
        }
        /*else if(sb_addr != 0 && i == sb_addr+sb.blocksize)
        {
            printf("\n%lu : 0x%08x",i, endianSwap4int(buffer)); // prints a series of bytes
            u32 bitmaps[bm_count*sb.blocksize];
            for(int k=0;k<sb.blocksize;k++)
            {
                fread(&buffer,1,4,file_pointer);
                bitmaps[k] = endianSwap4int(buffer);
                //printf(BYTE_TO_BINARY_PATTERN"\n",BYTE_TO_BINARY(bitmaps[k]));
            }
        }
        else if(root_addr != 0 && i == root_addr*sb.blocksize+sb_addr*4)
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
        }*/
        i++;
    }

    printf("\n----------0x%08x--------",ftell(file_pointer));
    fseek(file_pointer,sb_addr*4+sb.blocksize,SEEK_SET);
    printf("\n----------0x%08x--------",ftell(file_pointer));

    u32 bitmaps[bm_count*sb.blocksize];
    for(int k=0;k<sb.blocksize;k++)
    {
        fread(&buffer,1,4,file_pointer);
        bitmaps[k] = endianSwap4int(buffer);
        //printf(BYTE_TO_BINARY_PATTERN"\n",BYTE_TO_BINARY(bitmaps[k]));
    }


    

    fseek(file_pointer,sb_addr*4+sb.blocksize*sb.rootnode,SEEK_SET);
    printf("\n----------0x%08x--------",ftell(file_pointer));

    u8 *tempdata = (u8 *) malloc(sb.blocksize);
    fread(tempdata,1,sb.blocksize,file_pointer);
    for(int h=0;h<sb.blocksize;h++)
    {
        //tempdata[h] = endianSwap4int(tempdata[h]);
        //printf("\n:::::::0x%08x::::::",tempdata[h]);
    }

    struct u5fs_dir_inode rootnode;

    memcpy(&rootnode,tempdata,sizeof(rootnode));


    printf("\n0x%08x",rootnode.header.size);
    
    
    u32 index = 0;
    struct u5fs_dentry temp;
    /*memcpy(&temp,&tempdata[sizeof(rootnode)],5);
    int length = sscanf((u8 *)&tempdata[sizeof(rootnode)+5],"%s",temp.name);
    temp.dnode = endianSwap4int(temp.dnode);*/
    index += get_dentry(&temp,&tempdata[sizeof(rootnode)]);
    printf("\n...........%lu,%u,%s...........",temp.dnode,temp.dtype,temp.name);    
    
    struct u5fs_dentry temp2;
    index += get_dentry(&temp2,&tempdata[sizeof(rootnode)+index]);
    printf("\n...........%lu,%u,%s...........",temp2.dnode,temp2.dtype,temp2.name);


    struct u5fs_dentry temp3;
    index += get_dentry(&temp3,&tempdata[sizeof(rootnode)+index]);
    printf("\n...........%lu,%u,%s...........",temp3.dnode,temp3.dtype,temp3.name);

    struct u5fs_dentry temp4;
    index += get_dentry(&temp4,&tempdata[sizeof(rootnode)+index]);
    printf("\n...........%lu,%u,%s...........",temp4.dnode,temp4.dtype,temp4.name);

    struct u5fs_dentry temp5;
    index += get_dentry(&temp5,&tempdata[sizeof(rootnode)+index]);
    printf("\n...........%lu,%u,%s...........",temp5.dnode,temp5.dtype,temp5.name);
    
    return 1;
    
    
    printf("...........%lu,%u,%s...........",temp.dnode,temp.dtype,temp.name);

    fclose(file_pointer);
}

u32 get_dentry(struct u5fs_dentry * dest, u8 * src)
{
    memcpy(dest,src,5);
    int length = sscanf((u8 *)src+5,"%s",dest->name);
    dest->dnode = endianSwap4int(dest->dnode);
    return 5+strlen(dest->name)+1;
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