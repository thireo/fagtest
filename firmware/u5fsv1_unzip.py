import numpy as np

file = 'firmware.img'

U5FS_SUPERBLOCK_MAGIC = 0x55354653 #"U5FS" 

data = np.fromfile(file,dtype='>u4')

sb_ind = -1
sb_version = np.dtype('>u4')
sb_blocksize = np.dtype('>u4')
sb_blockcount = np.dtype('>u4')
sb_rootnode = np.dtype('>u4')


i = 0
for d in data:
    i += 1
    #print(d)
    if(d == U5FS_SUPERBLOCK_MAGIC):
        print(d)
        print(i)
        sb_ind = i
    if(sb_ind != -1 and i == sb_ind+1):
        sb_version = d
        print("SuperBlock - Version:\t",sb_version)
    if(sb_ind != -1 and i == sb_ind+2):
        sb_blocksize = d
        print("SuperBlock - Blocksize:\t",sb_blocksize)
    if(sb_ind != -1 and i == sb_ind+3):
        sb_blockcount = d
        print("SuperBlock - Blockcount:\t",sb_blockcount)
    if(sb_ind != -1 and i == sb_ind+4):
        sb_rootnode = d
        print("SuperBlock - Rootnode:\t",sb_rootnode)
        #break
    #if(i>sb_ind):
    if(i > sb_ind and np.equal(np.mod(np.uint32(i-sb_ind),sb_blocksize),np.uint32(0))):
        print("BM ",d)
        #print("i- ",((i-sb_ind)%sb_blocksize))

#with open(file,'rb') as f:
#    print(f.read(4))