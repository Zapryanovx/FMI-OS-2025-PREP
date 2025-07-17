#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <err.h>

void swap(int fd, int i, int j) {
    
    if ( lseek(fd, i * sizeof(uint16_t), SEEK_SET) < 0 ) {
        err(11, "err while lseeking");
    }

    uint16_t lhs;
    if ( read(fd, &lhs, sizeof(lhs)) != sizeof(lhs)) {
        err(12, "err while reading");
    }

    if ( lseek(fd, j * sizeof(uint16_t), SEEK_SET) < 0 ) {
        err(13, "err while lseeking");
    }

    uint16_t rhs;
    if ( read(fd, &rhs, sizeof(rhs)) != sizeof(rhs)) {
        err(14, "err while reading");
    }

    if ( lseek(fd, i * sizeof(uint16_t), SEEK_SET) < 0 ) {
        err(15, "err while lseeking");
    }

    if ( write(fd, &rhs, sizeof(rhs)) != sizeof(rhs)) {
        err(16, "err while reading");
    }

    if ( lseek(fd, j * sizeof(uint16_t), SEEK_SET) < 0 ) {
        err(17, "err while lseeking");
    }

    if ( write(fd, &lhs, sizeof(lhs)) != sizeof(lhs)) {
        err(18, "err while reading");
    }
}

void sort(int output, int size) {
    
    int min_idx;
    uint16_t min_val;
    uint16_t curr;

    for ( int i = 0; i < size - 1; i++) {
           
        min_idx = i;
        if ( read(output, &min_val, sizeof(min_val)) < 0 ) {
            err(19, "err while reading");
        }

        for ( int j = i + 1; j < size; j++ ) {
            if ( read(output, &curr, sizeof(curr)) < 0 ) {
                err(20, "err while reading");
            }

            if ( curr < min_val ) {
                min_idx = j;
            }
        }

        if ( i != min_idx) {
            swap(output, i, min_idx);
        }
    }
}  

void copy(int input, int output) {
    
    uint16_t num;
    int status;

    while((status = read(input, &num, sizeof(num))) == sizeof(num)) {
        
        if ( write(output, &num, sizeof(num)) != sizeof(num)) {
            err(7, "err while writing to dist");
        }
    }

    if ( status < 0 ) {
        err(8, "err while reading from source");
    }
    
    if ( lseek(input, 0, SEEK_SET) < 0 ) {
        err(9, "err while lseeking");
    }

    if ( lseek(output, 0, SEEK_SET) < 0 ) {
        err(10, "err while lseeking");
    }
}

int main(int argc, char* argv[]) {
    
    if ( argc != 3 ){
        errx(1, "2 files expected");
    }   

    int input = open(argv[1], O_RDONLY);
    if ( input < 0 ) {
        err(2, "err while opening %s", argv[1]);
    }
    
    struct stat st;
    if ( fstat(input, &st) < 0 ) {
        err(3, "err while fstating");
    }

    if ( st.st_size % sizeof(uint16_t) != 0 ) {
        err(4, "invalid input file");
    }

    int output = open(argv[2], O_WRONLY | O_TRUNC | O_CREAT, 0644);
    if ( output < 0 ) {
        err(5, "err while opening/creating %s", argv[2]);
    }

    copy(input, output);
    close(output);

    int output_sort = open(argv[2], O_RDWR);
    if ( output_sort < 0 ) {
        err(6, "err whiel opening %s", argv[2]);
    }

    sort(output_sort, st.st_size / sizeof(uint16_t));
    close(input);
    close(output_sort);

    

	return 0;
}
