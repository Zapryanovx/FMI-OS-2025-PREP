#include <sys/stat.h>
#include <fcntl.h>
#include <err.h>
#include <unistd.h>
#include <stdint.h>

void read_val(int fd, int start, uint32_t* min_val) {
    if(lseek(fd, start, SEEK_SET) < 0) {
        err(8, "err while lseeking");
    }
   
    if(read(fd, min_val, sizeof(*min_val)) != sizeof(*min_val)) {
        err(9, "err while reading");
    }

 }

void swap_vals(int output, int min_idx, int i) {
    
    
    uint32_t lhs;
    uint32_t rhs;

    if( lseek(output, min_idx * sizeof(uint32_t), SEEK_SET) < 0 ) {
        err(10, "err while lseeking");
    }

    if( read(output, &lhs, sizeof(lhs)) < 0 ) {
        err(11, "err while writing");
    }
    
    if( lseek(output, i * sizeof(uint32_t), SEEK_SET) < 0 ) {
        err(12, "err while lseeking");
    }

    if( read(output, &rhs, sizeof(lhs)) < 0 ) {
        err(13, "err while writing");
    }
     
    if( lseek(output, min_idx * sizeof(uint32_t), SEEK_SET) < 0 ) {
        err(14, "err while lseeking");
    }

    if( write(output, &rhs, sizeof(lhs)) < 0 ) {
        err(15, "err while writing");
    }
    
    if( lseek(output, i * sizeof(uint32_t), SEEK_SET) < 0 ) {
        err(16, "err while lseeking");
    }

    if( write(output, &lhs, sizeof(lhs)) < 0 ) {
        err(17, "err while writing");
    }
}

void sort(int output, int size) {

    uint32_t min_val;
    int min_idx;
    
    uint32_t curr;

    for(int i = 0; i < size - 1; i++){
        
        min_idx = i;
        read_val(output, i * sizeof(uint32_t), &min_val);
    
        for(int j = i + 1; j < size; j++) {
            read_val(output, j * sizeof(uint32_t), &curr);
            
            if(curr < min_val) {
                min_idx = j;
                min_val = curr;
            }
        }

        if ( min_idx != i ) {
            swap_vals(output, min_idx, i);
        }
    }
}

void copy(int input, int output) {
	
	uint32_t num;
	int status;

	while((status = read(input, &num, sizeof(num))) == sizeof(num)) {
        
        if(write(output, &num, sizeof(num)) < 0) {
            err(6, "err while writing to dist");
        }
    }

	if ( status < 0 ) {
        err(7, "err while reading from source");
	}
    
    if(lseek(output, 0, SEEK_SET) < 0) {
        err(8, "err while lseeking");
    }   

    close(output);
}

int main(int argc, char* argv[]) {
	
	if ( argc != 3 ) {
        errx(1, "2 files expected");
	}

	int input = open(argv[1], O_RDONLY);
	if ( input < 0 ) {
        err(2, "couldnt open source");
	}

	struct stat st;
	if ( fstat(input, &st) < 0 ) {
        err(3, "err while fstating");
	}

	if ( st.st_size % sizeof(uint32_t) != 0 ) {
        err(4, "invalid input file");
	}

	int output = open(argv[2], O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if ( output < 0 ) {
        err(5, "couldnt open dist");
	}

    copy(input,output);
    
    int output_cpy = open(argv[2], O_RDWR);
    if ( output_cpy < 0 ) {
        err(6, "couldnt open dist");
    }

    sort(output_cpy, st.st_size / sizeof(uint32_t));
    close(input);
    close(output_cpy);
    
	return 0;
}
