#include <sys/stat.h>
#include <fcntl.h>
#include <err.h>
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>

typedef struct {
    uint8_t padding_start[4];
    uint16_t count;
    uint8_t padding_cnt[10];
} header;

typedef struct {
    uint16_t beg;
    uint16_t cnt;
} interval;

typedef struct {
    interval intervals[4];
} complect;

void validate(int fd, int size) {
    
    header h;
    if ( read(fd, &h, sizeof(h)) != sizeof(h) ) {
        err(7, "err while reading header3213");
    }

    dprintf(1, "%ld\n", lseek(fd, 0, SEEK_CUR));

    struct stat st;
    if ( fstat(fd, &st) == -1 ) {
        err(8, "err while fstating");
    }
    
    if (( st.st_size - sizeof(h)) % size != 0 ) {
        err(9, "invalid format");
    }
}

int main(int argc, char* argv[]) {
	
	if ( argc != 7 ) {
        errx(1, "6 files expected");
	}

	int affix=open(argv[1], O_RDONLY);
	if ( affix == -1 ) {
        err(1, "couldnt open %s", argv[1]);
	}
    validate(affix, sizeof(complect));

    int postfix=open(argv[2], O_RDONLY);
    if ( postfix == -1 ) {
        err(2, "couldnt open %s", argv[2]);
    }
    validate(postfix, sizeof(uint32_t));

    int prefix=open(argv[3], O_RDONLY);
    if ( prefix == -1 ) {
        err(3, "couldnt open %s", argv[3]);
    }
    validate(prefix, sizeof(uint8_t));

    int infix=open(argv[4], O_RDONLY);
    if ( infix == -1 ) {
        err(4, "couldnt open %s", argv[4]);
    }
    validate(infix, sizeof(uint16_t));

    int suffix=open(argv[5], O_RDONLY);
    if ( suffix == -1 ) {
        err(5, "couldnt open %s", argv[5]);
    }
    validate(suffix, sizeof(uint64_t));
    
    int crucifixus = open(argv[6], O_WRONLY | O_TRUNC | O_CREAT, 0644);
    if ( crucifixus == -1 ) {
        err(6, "couldnt open %s", argv[6]);
    }
    
    if ( lseek(crucifixus, 16, SEEK_SET) == -1 ) {
        err(1000, "err while lseeking"); 
    }

    int bytes;
    complect c;
    
    uint16_t count=0;
    while (( bytes = read(affix, &c, sizeof(c))) == sizeof(c)) {            
        
        dprintf(1, "%d\n", c.intervals[0].cnt);
       
        if ( lseek(postfix, sizeof(header) + c.intervals[0].beg * sizeof(uint32_t), SEEK_SET) == -1 ) {
            err(15, "err while lseeking");
        }
       
        uint32_t n1;
        for ( uint8_t i = 0; i < c.intervals[0].cnt; i++ ) {

            if ( read(postfix, &n1, sizeof(uint32_t)) == -1) {
                err(11, "err while reading from postfix");
            } 

            if ( write(crucifixus, &n1, sizeof(uint32_t)) == -1) {
                err(12, "err while writing to crucifixus");
            }

        }

        if ( lseek(prefix, sizeof(header) + c.intervals[1].beg * sizeof(uint8_t), SEEK_SET) == -1 ) {
            err(15, "err while lseeking");
        }

        uint8_t n2;
        for ( uint8_t i = 0; i < c.intervals[1].cnt; i++ ) {
            if ( read(prefix, &n2, sizeof(uint8_t)) == -1) {
                err(14, "err while reading from prefix");
            }
            
            if ( write(crucifixus, &n2, sizeof(uint8_t)) == -1) {
                err(15, "err while writing to crucifixus");
            }

        }
        
        if( lseek(infix, sizeof(header) + c.intervals[2].beg * sizeof(uint16_t), SEEK_SET) == -1 ) {
            err(20, "err while lseeking");
        } 

        uint16_t n3;
        for ( uint8_t i = 0; i < c.intervals[2].cnt; i++ ) {
            if ( read(infix, &n3, sizeof(uint16_t)) == -1) {
                err(17, "err while reading from infix");
            }
            
            if ( write(crucifixus, &n3, sizeof(uint16_t)) == -1) {
                err(18, "err while writing to crucifixus");
            }
        }

        if ( lseek(suffix, sizeof(header) + c.intervals[3].beg * sizeof(uint64_t), SEEK_SET) == -1 ) {
            err(23, "err while lseeking");
        } 

        uint64_t n4;
        for ( uint8_t i = 0; i < c.intervals[3].cnt; i++ ) {
            if ( read(suffix, &n4, sizeof(uint64_t)) == -1) {
                err(20, "err while reading from suffix");
            }
            
            if ( write(crucifixus, &n4, sizeof(uint64_t)) == -1) {
                err(21, "err while writing to crucifixus");
            }
        }
    }

    if ( bytes < 0 ) {
        err(22, "err while reading from affix");
    }

    //validation
    count=(lseek(crucifixus, 0, SEEK_END) - sizeof(header)) / sizeof(uint8_t);
    
    if ( lseek(crucifixus, 4, SEEK_SET) == -1 ) {
        err(1000, "err while lseeking"); 
    }
    
    if ( write(crucifixus, &count, sizeof(uint16_t)) == -1) {
            err(21, "err while writing to crucifixus");
    }

    close(affix);
    close(postfix);
    close(prefix);
    close(infix);
    close(suffix);
    close(crucifixus);

	return 0;
}
