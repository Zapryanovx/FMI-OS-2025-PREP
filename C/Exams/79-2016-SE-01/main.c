#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <err.h>
#include <stdint.h>
#include <sys/stat.h>
#include <unistd.h>

typedef struct{
    uint32_t pos;
    uint32_t len;
} pair;

void writeTo(int to, int from, const pair data){

    int start=lseek(from, data.pos * sizeof(uint32_t), SEEK_SET);
    if(start==-1){
        err(8, "error while lseeking");
    }

    struct stat st2;
    if(fstat(from, &st2) == -1){
        err(9, "error while fstat");
    }

    uint32_t maxPos = st2.st_size / sizeof(uint32_t);
    if(data.pos >= maxPos || data.pos + data.len > maxPos){
        err(10, "out of range");
    }

    uint32_t curr;
    for(uint32_t i = 0; i < data.len; i++){
        if(read(from, &curr, sizeof(curr)) != sizeof(curr)){
            err(11, "error while reading");
        }

        if(write(to, &curr, sizeof(curr)) != sizeof(curr)){
            err(12, "error while writing");
        }
    }
}

int main(int argc, char* argv[]) {

    if(argc != 4){
        errx(1, "3 binary files expected");
    }

    int f1=open(argv[1], O_RDONLY);
    if(f1==-1){
        err(2, "err opening %s", argv[1]);
    }

    struct stat st;
    if(fstat(f1, &st) == -1){
        err(3, "err while stating %s", argv[1]);
    }

    if(st.st_size % (2 * sizeof(uint32_t)) != 0){
        err(4, "f1 is invalid");
    }

    int f2=open(argv[2], O_RDONLY);
    if(f2==-1){
        err(5, "err opening %s", argv[2]);
    }

    int f3=open(argv[3], O_WRONLY | O_TRUNC | O_CREAT, 0644);
    if(f3==-1){
        err(6, "err opening %s", argv[3]);
    }

    pair curr;
    int bytes;
    while((bytes = read(f1, &curr, sizeof(curr))) > 0){
        writeTo(f3, f2, curr);
    }

    if(bytes == -1){
        err(7, "err while reading from %s", argv[1]);
    }

    close(f1);
    close(f2);
    close(f3);
    return 0;
}

