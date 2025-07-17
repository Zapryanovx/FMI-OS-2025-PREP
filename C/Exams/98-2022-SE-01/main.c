#include <stdint.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <err.h>
#include <unistd.h>
#include <stdio.h>

typedef struct {
    uint32_t magic;
    uint32_t count;
} data_header;

typedef struct {
    uint32_t magic1;
    uint16_t magic2;
    uint16_t reserved;
    uint64_t count;
} comparator_header;

typedef struct {
    uint16_t type;
    uint16_t reserved[3];
    uint32_t offset1;
    uint32_t offset2;
} comparator_data;

int main(int argc, char* argv[]) {
	
	if ( argc != 3 ) { 
        errx(1, "2 files expected");
	}

    int data = open(argv[1], O_RDONLY);
    if ( data < 0 ) {
        err(2, "couldnt open %s", argv[1]);
    }

    int comparator = open(argv[2], O_RDONLY);
    if ( comparator < 0 ) {
        err(3, "couldnt open %s", argv[2]);
    }

    data_header dh;
    if ( read(data, &dh, sizeof(dh) ) != sizeof(dh)) {
        err(4, "invalid data");
    }

    if( dh.magic != 0x21796F4A ) {
        err(5, "invalid data");
    }
    
    comparator_header ch;
    if ( read(comparator, &ch, sizeof(ch) ) != sizeof(ch)) {
        err(5, "invalid comparator");
    }

    if ( ch.magic1 != 0xAFBC7A37 || ch.magic2 != 0x1C27 ) {
        err(6, "invalid comparator");
    }
    
    struct stat st;
    if ( fstat(comparator, &st) < 0 ){
        err(7, "err while fstating");
    }
    
    if ( (st.st_size - sizeof(ch)) / sizeof(comparator_data) != ch.count ) {
        err(8, "count doesnt match sixtuples");
    }
    
    comparator_data cd;
    int bytes;
    while (( bytes = read(comparator, &cd, sizeof(cd))) != sizeof(cd)) {
        
        if ( lseek(data, sizeof(dh) + cd.offset1, SEEK_SET) < 0 ){
            err(9, "err while lseeking");
        }
            
        uint64_t left;
        if ( read(data, &left, sizeof(left)) != sizeof(left)) {
            err(10, "err while reading from data");
        }

        if ( lseek(data, sizeof(dh) + cd.offset2, SEEK_SET) < 0 ) {
            err(11, "err while lseeking");
        }

        uint64_t right;
        if ( read(data, &right, sizeof(right)) != sizeof(right)) {
            err(12, "err while reading from data");
        }

        if (cd.type == 0 && left <= right) {
            continue;
        }

        if ( lseek(data, sizeof(dh) + cd.offset1, SEEK_SET) < 0 ){
            err(13, "err while lseeking");
        }
        
        if ( write(data, &right, sizeof(right)) != sizeof(right)) {
            err(14, "err while writing to data");
        }
        
        if ( lseek(data, sizeof(dh) + cd.offset2, SEEK_SET) < 0 ){
            err(15, "err while lseeking");
        }
        
        if ( write(data, &left, sizeof(left)) != sizeof(left)) {
            err(16, "err while writing to data");
        }
    }

    if ( bytes < 0 ) {
        err(8, "err while reading from comparator");
    }

    close(data);
    close(comparator);

	return 0;
}
