#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <stdint.h>
#include <string.h>

int main(int argc, char* argv[]) {
	
    if(argc!=3){
        errx(1, "2 args expected");
    }

    const char* flag=argv[1];
    const char* fileName=argv[2];

    if(strcmp(flag, "--min") && strcmp(flag, "--max") && strcmp(flag, "--print")){
        errx(2, "option must be one of (--min, --max, --print)");
    }
    
    int file=open(fileName, O_RDONLY);
    if(file==-1){
        err(3, "%s couldnt be opened", fileName);
    }

    uint16_t max=0;
    uint16_t min=UINT16_MAX;
    uint16_t num;
    int bytes;

    while((bytes=read(file, &num, sizeof(num))) > 0){
        
        if(!strcmp(flag, "--min")){
            if(num < min){
                min = num;
            }
        }

        else if(!strcmp(flag, "--max")){
            if(num > max){
                max = num;
            }
        }

        else{
            char buff[32];
            int len=snprintf(buff, sizeof(buff), "%d\n", num);
            if(len < 0 || len >= (int)sizeof(buff)){
                err(4, "format error");
            }

            if(write(1, buff, len) != len){
                 err(5, "write to stdout error");
            }
        }
    }

    if(bytes==-1){
        err(6, "error while reading from %s", fileName);
    }

    if(!strcmp(flag, "--max")){
        char buff[32];
        int len=snprintf(buff, sizeof(buff), "%d\n", max);
        if(len < 0 || len >= (int)sizeof(buff)){
            err(7, "format error");
        }

        if(write(1, buff, len) != len){
            err(8, "write to stdout error");
        }
    }

    if(!strcmp(flag, "--min")){
        char buff[32];
        int len=snprintf(buff, sizeof(buff), "%d\n", min);
        if(len < 0 || len >= (int)sizeof(buff)){
            err(9, "format error");
        }

        if(write(1, buff, len) != len){
            err(10, "write to stdout error");
        }
    }

    close(file);
	exit(0);
}
