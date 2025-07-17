#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>
#include <unistd.h>
#include <err.h>
#include <stdio.h>

uint16_t transform(uint8_t byte) {
    
    uint16_t res = 0;
    int at = 15;

    for(int i = 7; i >= 0; i--) {
        
        if (( byte & (1 << i)) > 0) {
            res |= (1 << at);
        }

        else {
            res |= (1 << (at - 1));
        }

        at-=2;
    }

    return res;
}

int main(int argc, char* argv[]) {
	
    if ( argc != 3 ) {
        errx(1, "2 args expected");
    }

    int input = open(argv[1], O_RDONLY);
    if ( input < 0 ) {
        err(2, "couldnt open %s", argv[1]);
    }

    int output = open(argv[2], O_WRONLY | O_TRUNC | O_CREAT, 0644);
    if ( output < 0 ) {
        err(3, "couldnt open %s", argv[2]);
    }

    int status;
    uint8_t byte;

    while (( status = read(input, &byte, sizeof(byte))) == sizeof(byte)) {
        
        uint16_t new = transform(byte);
        
        if ( write(output, &new, sizeof(new)) < 0 ) {
            err(4, "err while writing to %s", argv[2]);
        }
    }
    
    if ( status < 0 ) {
        err(5, "err while reading from %s", argv[1]);
    }
    
    close(input);
    close(output);
	return 0;
}
