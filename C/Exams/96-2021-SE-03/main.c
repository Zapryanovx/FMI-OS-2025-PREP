#include <fcntl.h>
#include <err.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char* argv[]) {
	
	if ( argc != 3 ) {
        errx(1, "expected two args");
	}

	int fd1=open(argv[1], O_RDONLY);
	if ( fd1 < 0 ) {
        err(2, "couldnt open %s", argv[1]);
	}

	int fd2=open(argv[2], O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if ( fd2 < 0 ){
        err(3, "couldnt open %s", argv[2]);
	}

    struct stat st;
    if ( fstat(fd1, &st ) < 0 ) {
        err(4, "err while fstating");
    }

    uint32_t arrN = st.st_size / sizeof(uint16_t);
    if ( arrN > 524288 ) {
        err(5, "limit reached");
    }

    const char* incl = "#include <stdint.h>\n\n";
    if(write(fd2, incl, strlen(incl)) != (ssize_t) strlen(incl)){
        err(6, "err while writing to header file");
    }

    char arrNbuff[256];
    if(snprintf(arrNbuff, 256, "%s%u;\n", "const uint32_t arrN = ", arrN) < 0) {
        err(7, "err while writing array size to header");
    }

    if(write(fd2, arrNbuff, strlen(arrNbuff)) != (ssize_t) strlen(arrNbuff)) {
        err(8, "err while writing to header");
    }

    const char* arrBeg="const uint16_t arr[] = { ";
    if(write(fd2, arrBeg, strlen(arrBeg)) != (ssize_t) strlen(arrBeg)){
        err(9, "err while writing array to header");
    }

    for(uint32_t i = 0; i < arrN; i++) {
        uint16_t num;
        if(read(fd1, &num, sizeof(num)) != sizeof(num)) {
            err(10, "err while reading num from %s", argv[1]);
        }

        char numBuff[32];
        if(snprintf(numBuff, 32, "%u", num) < 0) {
            err(11, "err while saving to buff");
        }
        if(write(fd2, numBuff, strlen(numBuff)) != (ssize_t) strlen(numBuff)) {
            err(12, "err while writing to header");
        }

        if ( i != arrN - 1 ) {
            if(write(fd2, ", ", 2) != 2) {
                err(13, "err while writing to header");
            }   
        }
    }

    if(write(fd2, " };\n", 4) != 4){
        err(14, "err while writing to header");
    }

    close(fd1);
    close(fd2);
	return 0;
}
