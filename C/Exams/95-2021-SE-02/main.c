#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <err.h>
#include <stdio.h>
#include <stdbool.h>

void swap_bytes(uint16_t* bytes) {
    
    uint16_t lhs = *bytes >> 8;
    uint16_t rhs = *bytes << 8;
    *bytes = lhs | rhs;
}

int main(int argc, char* argv[]) {

    if ( argc != 3 ) {
        errx(1, "2 files expected");
    }

    int input = open(argv[1], O_RDONLY);
    if ( input < 0 ) {
        err(2, "couldnt open %s", argv[1]);
    }

    int output = open(argv[2], O_WRONLY | O_TRUNC | O_CREAT, 0644);
    if ( output < 0 ) {
        err(3, "couldnt open %s", argv[2]);
    }

    uint16_t bytes;
    int status;

    while((status = read(input, &bytes, sizeof(bytes))) == sizeof(bytes)) {
        
        swap_bytes(&bytes);
        uint8_t new = 0;

        for(int i = 15; i >= 1; i-=2){
            bool is_one = bytes & (1 << i);
            if(is_one) {
                new |= 1 << (i / 2);
            }
        }

        if(write(output, &new, sizeof(new)) != sizeof(new)) {
            err(4, "err while writing to %s", argv[2]);
        }
    }

    if ( status < 0 ) {
        err(5, "err while reading from %s", argv[1]);
    }

	return 0;
}
