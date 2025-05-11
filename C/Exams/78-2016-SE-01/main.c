#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <err.h>
#include <stdint.h>

int compare_bytes(const void* lhs, const void* rhs){
    return (*(const uint8_t*)lhs - *(const uint8_t*)rhs);
}

int main(int argc, char* argv[]) {

    if(argc != 2){
        errx(1, "one binary file expected");
    }

    int file=open(argv[1], O_RDONLY);
    if(file==-1){
        err(2, "error in opening file");
    }

    struct stat st;
    if(fstat(file, &st) == -1){
        err(3, "couldnt stat");
    }

    uint64_t size = st.st_size;
    uint8_t bytes[size];
    uint8_t curr;
    for(uint64_t i = 0; i < size; i++){
        if(read(file, &curr, sizeof(curr)) != sizeof(curr)) {
            err(4, "err while reading");
        }

        bytes[i] = curr;
    }

    close(file);
    
    qsort(bytes, size, sizeof(bytes[0]), compare_bytes);

    int file2=open(argv[1], O_WRONLY | O_TRUNC);
    if(file2==-1){
        err(5, "err while opening file");
    }

    for(uint64_t i = 0; i < size; i++){
        if(write(file2, &bytes[i], sizeof(bytes[i])) != sizeof(bytes[i])){
            err(6, "err while saving sorted bytes");
        }
    }

    close(file2);
	exit(0);
}
