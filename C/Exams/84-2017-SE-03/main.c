#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <err.h>


typedef struct {
    uint16_t offset;
    uint8_t orig;
    uint8_t new;
} triplet;

void cpy_file(int f1, int f2) {
    
    int status;
    uint8_t byte;

    while (( status = read(f1, &byte, sizeof(byte))) == sizeof(byte)) {
        if ( write(f2, &byte, sizeof(byte)) != sizeof(byte) ) {
            err(15, "err while writing to dist");
        }
    }

    if ( status < 0 ) {
        err(16, "err while reading from source");
    }
}

void patch_file(int patch, int f1, int f2) {

    triplet tr;
    int status;

    while (( status = read(patch, &tr, sizeof(tr))) == sizeof(tr)) {
        
        if ( lseek(f1, tr.offset, SEEK_SET) < 0 ) {
            err(9, "err while lseeking");
        }

        uint8_t byte;
        if ( read(f1, &byte, sizeof(byte)) != sizeof(byte) ) {
            err(10, "err while reading from source");
        }

        if ( byte != tr.orig ) {
            err(11, "byte in triplet and actual byte doesnt match");
        }

        if ( lseek(f2, tr.offset, SEEK_SET) < 0 ) {
            err(12, "err while lseeking");
        }   

        if ( write(f2, &tr.new, sizeof(tr.new)) != sizeof(tr.new) ) {
            err(13, "err while writing to dist");
        }
    }
    
    if ( status < 0 ) {
        err(14, "err while reading from source");
    }

}

int main(int argc, char* argv[]) {
	    
	if ( argc != 4 ) {
        errx(1, "3 args expected");
	}

	int patch = open(argv[1], O_RDONLY);
	if ( patch < 0 ) {
        err(2, "couldnt open %s", argv[1]);
	}

	struct stat st_patch;
	if ( fstat(patch, &st_patch) < 0 ) {
        err(3, "err while stating");
	}

	if ( st_patch.st_size % sizeof(triplet) != 0 ) {
        err(4, "invalid patch file");
	}
	
	int f1 = open(argv[2], O_RDONLY);
	if ( f1 < 0 ) {
        err(5, "couldnt open %s", argv[2]);
	}

	struct stat st_f1;
	if ( fstat(patch, &st_f1) < 0 ) {
        err(6, "err while stating");
	}

	if ( st_f1.st_size % sizeof(uint8_t) != 0 ) {
        err(7, "invalid source file");
    }

 	int f2 = open(argv[3], O_WRONLY | O_TRUNC | O_CREAT, 0664);
	if ( f2 < 0 ) {
        err(8, "couldnt open %s", argv[3]);
	}
    
    cpy_file(f1,f2);
    patch_file(patch, f1, f2);
	return 0;
}
