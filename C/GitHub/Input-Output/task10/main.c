#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <stdint.h>
#include <string.h>

int main(int argc, char* argv[]) {
	
    if(argc!=2){
        errx(1, "1 arg expected");
    }

    const char* fileName=argv[1];
    
    int file=open(fileName, O_RDONLY);
    if(file==-1){
        err(2, "%s couldnt be opened", fileName);
    }

    uint8_t maxByte=0;
    uint8_t byte=0;
    int bytes;

    while((bytes=read(file, &byte, sizeof(byte))) > 0){
        if(byte > maxByte){
            maxByte=byte;
        }
    }

    if(bytes==-1){
        err(3, "error while reading from %s", fileName);
    }

    char buff[32];
    int len=snprintf(buff, sizeof(buff), "%d\n", maxByte);
    if(len < 0 || len >= (int)sizeof(buff)){
        err(4, "error while formatting");
    }
    
    if(write(1, buff, len) != len){
        err(5, "error while writing to stdout");
    }

    close(file);
	exit(0);
}
