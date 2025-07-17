#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <err.h>

int main(int argc, char* argv[]) {
	
	if ( argc != 4 ) {
        errx(1, "2 files expected");
	}

	int scl = open(argv[1], O_RDONLY);
    if ( scl < 0 ) {
        err(2, "couldnt open %s", argv[1]);
    }

    int sdl = open(argv[2], O_RDONLY);
    if ( sdl < 0 ) {
        err(3, "couldnt open %s", argv[2]);
    }

    struct stat st_scl;
    if ( fstat(scl, &st_scl) < 0 ) {
        err(4, "err while fstating");
    }

    struct stat st_sdl;
    if ( fstat(sdl, &st_sdl) < 0 ) {
        err(5, "err while fstating");
    }

    if ((size_t) st_scl.st_size * 8 != st_sdl.st_size / sizeof(uint16_t)) {
        err(6, "not equal els in two files");
    }

    int filtered = open(argv[3], O_WRONLY | O_TRUNC | O_CREAT, 0664);
    if ( filtered < 0 ) {
        err(7, "couldnt create/open %s", argv[3]);
    }

    uint8_t byte;
    int status;
    uint16_t curr;
    
    while((status = read(scl, &byte, sizeof(byte))) == sizeof(byte)) {
        
        for(int i = 7; i >= 0; i--) {
            if(read(sdl, &curr, sizeof(curr)) != sizeof(curr)) {
                err(9, "err while reading from %s", argv[2]);
            }

            if ( byte & (1 << i)) {
                if(write(filtered, &curr, sizeof(curr)) != sizeof(curr)){
                    err(10, "err while writing to res file");
                }
            }
        }
    }  

    if ( status < 0 ) {
        err(8, "err while reading from %s", argv[1]);
    }

	return 0;
}
