#include <stdio.h>
#include <err.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char* argv[]) {
	
    if(argc != 3){
        errx(1, "two files expected");
    }

    int f1 = open(argv[1], O_RDONLY);
    if(f1 == -1){
        errx(2, "%s couldnt be opened", argv[1]);
    }  

    int f2 = open(argv[2], O_RDONLY);
    if(f2 == -1){
        errx(3, "%s couldnt be opened", argv[2]);
    }
    
    int temp = open("tempfile", O_RDWR | O_TRUNC | O_CREAT, 0644);
    if(temp == -1){
        errx(4, "err while creating temp file");
    }
    
    char ch;
    int state;
    while((state=read(f1, &ch, sizeof(ch))) > 0) {
        if(write(temp, &ch, sizeof(ch)) != 1){
            errx(5, "err while reading from %s", argv[1]);
        }
    }
    
    if(close(f1) == -1){
        errx(6, "couldnt close %s", argv[1]);
    }

    if(lseek(temp, 0, SEEK_SET) == -1){
        errx(7, "couldnt seek to start of tempfile");
    }
    
    int f1Two=open(argv[1], O_WRONLY | O_TRUNC);
    if(f1Two == -1){
        errx(8, "couldnt open %s", argv[1]);
    }

    while((state=read(f2, &ch, sizeof(ch))) > 0) {
        if(write(f1Two, &ch, sizeof(ch)) != 1) {
            errx(9, "err while writing to %s", argv[1]);
        }
    }
    
    if(close(f1Two) == -1){
        errx(10, "err while closing %s", argv[1]);
    }

    if(close(f2) == -1){
        errx(11, "couldnt close %s", argv[2]);
    }

    int f2Two = open(argv[2], O_WRONLY | O_TRUNC);
    if(f2Two == -1) {
        errx(12, "couldnt open %s", argv[2]);
    }

    while((state=read(temp, &ch, sizeof(ch))) > 0){
        if(write(f2Two, &ch, sizeof(ch)) != 1){
            errx(13, "err while writing to %s", argv[2]);
        }
    }

    if(close(f2Two) == -1){
        errx(14, "err while closing %s", argv[2]);
    }

    if(close(temp) == -1){
        errx(15, "err while closing tempfile");
    }
    
    unlink("tempfile");
	exit(0);
}
