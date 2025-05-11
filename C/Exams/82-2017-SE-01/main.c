#include <stdint.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <err.h>

typedef struct{
    uint16_t offset;
    uint8_t original;
    uint8_t new;
} diff;

int main(int argc, char* argv[]) {
	
	if(argc != 4){
        errx(1, "3 args expected");
	}  

    int fd1 = open(argv[1], O_RDONLY);
    if(fd1==-1){
        err(2, "error while opening %s", argv[1]);
    }

    int fd2 = open(argv[2], O_RDONLY);
    if(fd2==-1){
        err(3, "error while opening %s", argv[2]);
    }

    struct stat st1;
    if(fstat(fd1, &st1)==-1){
        err(4, "error while fstat");
    }
    
    struct stat st2;
    if(fstat(fd2, &st2)==-1){
        err(5, "error while fstat");
    }

    if(st1.st_size != st2.st_size){
        err(6, "files are with different size");
    }

    int patch = open(argv[3], O_WRONLY | O_TRUNC | O_CREAT, 0644);
    if(patch==-1){
        err(7, "error while opening %s", argv[3]);
    }
    
    uint8_t origByte;
    uint8_t newByte;

    int read_bytes1;
    int read_bytes2;
    
    for(int i = 0; i < st1.st_size; i++){
        if((read_bytes1=read(fd1, &origByte, sizeof(origByte))) != sizeof(origByte)){
            err(8, "error while reading from %s", argv[1]);
        }

        if((read_bytes2=read(fd2, &newByte, sizeof(newByte))) != sizeof(newByte)){
            err(9, "error while reading from %s", argv[2]);
        }

        if(origByte != newByte){
            diff d;
            d.offset = i;
            d.original = origByte;
            d.new = newByte;
            
            if(write(patch, &d, sizeof(d)) != sizeof(d)){
                err(10, "error while writing to %s", argv[3]);
            }
       }
    }

    close(fd1);
    close(fd2);
    close(patch);
	return 0;
}
