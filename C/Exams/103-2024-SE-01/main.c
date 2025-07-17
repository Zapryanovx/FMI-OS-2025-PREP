#include <fcntl.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <sys/stat.h>
#include <stdbool.h>

typedef struct {
    uint64_t next;
    char user_data[504];
} node_t;


int main(int argc, char* argv[]) {

    if( argc != 2 ) {
        errx(1, "one file expected");
    }
    
    int fd = open(argv[1], O_RDWR);
    if ( fd < 0 ) {
        err(2, "couldnt open %s", argv[1]);
    }
    
    struct stat st;
    if ( fstat(fd, &st) < 0 ) {
        err(3, "err while fstating");
    }
    
    if ( st.st_size % sizeof(node_t) != 0 ) {
        err(4, "invalid input file");
    }

    size_t size = st.st_size / sizeof(node_t);
    bool *arr = calloc(size, sizeof(bool));    
    if (!arr) {
        err(8, "calloc failed");
    }
    arr[0] = true;
    
    node_t node;
    int status;

    while((status = read(fd, &node, sizeof(node))) == sizeof(node)) {
        arr[node.next] = true;
        
        if(lseek(fd, node.next * sizeof(node), SEEK_SET) < 0 ) {
            err(5, "err while lseeking");
        }

        if(node.next == 0) {
            break;
        }
    }

    if( status < 0 ) {
        err(5, "err while reading from %s", argv[1]);
    }
    
    if(lseek(fd, 0, SEEK_SET) < 0){
        err(7, "err while lseeking");
    }

    uint8_t empty[sizeof(node_t)] = {0};
    for (size_t i = 0; i < size; i++) {
        if (!arr[i]) {
            off_t off = i * sizeof(node_t);
            if (lseek(fd, off, SEEK_SET) < 0) {    
                err(6, "err while lseeking");
            }
            if (write(fd, &empty, sizeof(empty)) != sizeof(empty)) {
                err(7, "err while writing zeros to %s", argv[1]);
            }
        }
    }


    close(fd);
    free(arr);
    return 0;
}

