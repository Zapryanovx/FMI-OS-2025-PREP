#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <err.h>
#include <stdio.h>
#include <string.h>

typedef struct {
    uint32_t magic;
    uint8_t header_version;
    uint8_t data_version;
    uint16_t count;
    uint32_t reserved[2];
} header_p;

typedef struct {
    uint16_t offset;
    uint8_t orig;
    uint8_t new;
} data00_t;

typedef struct {
    uint32_t offset;
    uint16_t orig;
    uint16_t new;
} data01_t;

void copy_file(int f1, int f2) {
    
    uint8_t byte;
    int status;

    while((status = read(f1, &byte, sizeof(byte))) == sizeof(byte)) {
        if ( write(f2, &byte, sizeof(byte)) != sizeof(byte) ){
            err(8, "err while writing to dist");
        }
    }

    if ( status < 0 ) {
        err(7, "err while reading from source");
    }

    if ( lseek(f1, 0, SEEK_SET) < 0 || lseek(f2, 0, SEEK_SET) < 0 ) {
        err(8, "err while lseeking");
    }
}

void patch_00(int f1, int f2, int patch) {
    
    struct stat st;
    if ( fstat(f1, &st) < 0 ) {
        err(9, "err while fstating");
    }

    if ( st.st_size % sizeof(uint8_t) != 0 ) {
        err(10, "invalid source");
    }

    data00_t d;
    int status;

    while((status = read(patch, &d, sizeof(d))) == sizeof(d)) {
        
        if ( lseek(f1, d.offset * sizeof(uint8_t), SEEK_SET) < 0 ) {
            err(12, "err while lseeking");
        }

        uint8_t curr;
        if ( read(f1, &curr, sizeof(curr)) != sizeof(curr)) {
            err(13, "err while reading");
        }

        if ( curr != d.orig ) {
            err(14, "data original and actual byte doesnt match");
        }

        if ( lseek(f2, d.offset * sizeof(uint8_t), SEEK_SET) < 0 ) {
            err(15, "err while lseeking");
        }

        if ( write(f2, &d.new, sizeof(d.new)) != sizeof(d.new)) {
            err(16, "err while writing to dist");
        }
    }

    if ( status < 0 ) {
        err(11, "err while reading from patch");
    }
}

void patch_01(int f1, int f2, int patch) {
       
    struct stat st;
    if ( fstat(f1, &st) < 0 ) {
        err(11, "err while fstating");
    }

    if ( st.st_size % sizeof(uint16_t) != 0 ) {
        err(12, "invalid source");
    }

    data01_t d;
    int status;

    while((status = read(patch, &d, sizeof(d))) == sizeof(d)) {
        
        if ( lseek(f1, d.offset * sizeof(uint16_t), SEEK_SET) < 0 ) {
            err(17, "err while lseeking");
        }

        uint16_t curr;
        if ( read(f1, &curr, sizeof(curr)) != sizeof(curr)) {
            err(18, "err while reading");
        }

        if ( curr != d.orig ) {
            err(19, "data original and actual byte doesnt match");
        }

        if ( lseek(f2, d.offset * sizeof(uint16_t), SEEK_SET) < 0 ) {
            err(20, "err while lseeking");
        }

        if ( write(f2, &d.new, sizeof(d.new)) != sizeof(d.new)) {
            err(21, "err while writing to dist");
        }
    }

    if ( status < 0 ) {
        err(22, "err while reading from patch");
    }
}

int main(int argc, char* argv[]) {
	
	if ( argc != 4 ) {
        errx(1, "3 files expected");
	}    

    int patch = open(argv[1], O_RDONLY);
    if ( patch < 0 ) {
        err(2, "err while opening %s", argv[1]);
    }

    int f1 = open(argv[2], O_RDONLY);
    if ( f1 < 0 ) {
        err(3, "err while opening %s", argv[2]);
    }

    int f2 = open(argv[3], O_WRONLY | O_TRUNC | O_CREAT, 0664);
    if ( f2 < 0 ) {
        err(4, "err while creating/opening %s", argv[3]);
    }

    header_p hp;
    if ( read(patch, &hp, sizeof(hp)) != sizeof(hp) ) {
        err(5, "invalid patch header");
    }
    
    if ( hp.magic != 0xEFBEADDE || hp.header_version != 0x01) {
        err(6, "invalid patch header");
    }
    
    if ( hp.data_version != 0x00 && hp.data_version != 0x01 ) {
        err(7, "invalid patch header");
    }
    
    copy_file(f1, f2);

    if ( hp.data_version == 0x00 ) {
        patch_00(f1, f2, patch);
    }

    else {
        patch_01(f1, f2, patch);
    }

    close(patch);
    close(f1);
    close(f2);

	return 0;
}
