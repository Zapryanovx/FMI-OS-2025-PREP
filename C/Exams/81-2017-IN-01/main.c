#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include <err.h>
#include <stdbool.h>
#include <stdio.h>

typedef struct {
    uint16_t offset;
    uint8_t len;
    uint8_t dummy;
} triplet;

bool isUpper(uint8_t byte){
    return byte >= 0x41 && byte <= 0x5A;
}

int main(int argc, char* argv[]) {
	
    if(argc != 5){
        errx(1, "2 files expected");
    }

	int f1_dat = open(argv[1], O_RDONLY);
	if(f1_dat==-1){
        err(2, "err while opening %s", argv[1]);
	}

	struct stat statf1_dat;
    if(fstat(f1_dat, &statf1_dat)==-1){
        err(3, "err while fstat");
    }

    int f1_idx = open(argv[2], O_RDONLY);
    if(f1_idx==-1){
        err(4, "err while opening %s", argv[2]);
    }

    struct stat statf1_idx;
    if(fstat(f1_idx, &statf1_idx)==-1){
        err(5, "err while fstat");
    }

    if(statf1_idx.st_size % sizeof(triplet) != 0){
        err(6, "%s is invalid", argv[2]);
    }
    
    int f2_dat = open(argv[3], O_WRONLY | O_TRUNC | O_CREAT, 0644);
    if(f2_dat==-1){
        err(7, "err while opening %s", argv[3]);
    }
    
    int f2_idx = open(argv[4], O_WRONLY | O_TRUNC | O_CREAT, 0644);
    if(f2_idx==-1){
        err(8, "err while opening %s", argv[4]);
    }

    triplet trf1;
    uint8_t byte;
    int read_bytes;
        
    triplet trf2;
    trf2.offset=0;
    trf2.len=0;
    trf2.dummy=0;

    while((read_bytes = read(f1_idx, &trf1, sizeof(trf1))) > 0) {        
        printf("here");
        if(lseek(f1_dat, trf1.offset, SEEK_SET)==-1){
            err(9, "out of range");
        }

        if(trf1.offset + trf1.len > statf1_dat.st_size){
            err(10, "out of range");
        }

        bool isValid=true;
        for(int i = 0; i < trf1.len; i++){
            if(read(f1_dat, &byte, sizeof(byte)) != sizeof(byte)){
                err(11, "err while reading from %s", argv[1]);
            }

            if(i==0 && !isUpper(byte)) {
                isValid=false;
                break;
            }

            if(write(f2_dat, &byte, sizeof(byte)) != sizeof(byte)){
                err(12, "err while writing to %s", argv[3]);
            }
            
            
        }

        if(isValid){
            trf2.len = trf1.len;
            if(write(f2_idx, &trf2, sizeof(trf2)) != sizeof(trf2)){
                err(13, "err while writing to %s", argv[4]);
           }

           trf2.offset += trf1.len;
        }
    }

    if(read_bytes==-1){
        err(14, "err while reading from %s", argv[2]);
    }

    close(f1_dat);
    close(f1_idx);
    close(f2_dat);
    close(f2_idx);
	return 0;
}
