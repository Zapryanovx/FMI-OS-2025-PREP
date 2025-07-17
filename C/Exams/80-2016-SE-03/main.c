#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <err.h>

int get_file_size(int fd) {
    
    struct stat st;
    if ( fstat(fd, &st) < 0 ) {
        err(3, "err while stating");
    }

    if ( st.st_size / sizeof(uint32_t) > 100000000 ) {
        err(4, "too many numbers");
    }

    return  st.st_size / sizeof(uint32_t);
} 

void wrapper_lseek(int fd, int offset) {
    
    if ( lseek(fd, offset * sizeof(int32_t), SEEK_SET) < 0 ) {
        err(5, "err while lseeking");
    }
}

void wrapper_read(int fd, uint32_t* n) {
    if ( read(fd, n, sizeof(uint32_t)) < 0 ) {
        err(6, "err while reading");
    }
}

void wrapper_write(int fd, uint32_t* n) {
    if ( write(fd, n, sizeof(uint32_t)) < 0 ) {
        err(7, "err while writing");
    }
}

void selection_sort(int fd) {
    
    int size = get_file_size(fd);
    
    uint32_t n1;
    uint32_t n2;

    int min_idx;
    uint32_t min_val;

    for ( int i = 0; i < size - 1; i++ ) {

        wrapper_lseek(fd, i);
        wrapper_read(fd, &n1);
        
        min_idx = i;
        min_val = n1;

        for( int j = i + 1; j < size; j++ ) {
            
            wrapper_lseek(fd, j);
            wrapper_read(fd, &n2);
        
            if ( n2 < n1 ) {
                min_idx = j;
                min_val = n2;
           }
        }

        if ( i != min_idx ) {
            wrapper_lseek(fd, i);
            wrapper_write(fd, &min_val);

            wrapper_lseek(fd, min_idx);
            wrapper_write(fd, &n1);
        }
    }
}

int main(int argc, char* argv[]) {

    if ( argc != 2 ) {
        errx(1, "1 arg expected");
    }

    int fd = open(argv[1], O_RDWR);
    if ( fd < 0 ) {
        err(2, "couldnt open %s", argv[1]);
    }

    selection_sort(fd);
  	return 0;
}
