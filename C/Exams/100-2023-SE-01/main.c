#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>
#include <unistd.h>
#include <err.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
	
    if ( argc != 3 ) {
        errx(1, "2 files expected");
    }

    int stream = open(argv[1], O_RDONLY);
    if ( stream < 0 ) {
        err(2, "couldnt open %s", argv[1]);
    }

    int msg = open(argv[2], O_WRONLY | O_TRUNC | O_CREAT, 0644);
    if ( msg < 0 ) {
        err(3, "couldnt open %s", argv[2]);
    }

    uint8_t curr;
    int bytes;
    while ((bytes = read(stream, &curr, sizeof(curr))) == sizeof(curr)) {
        
        uint8_t data[256];        
        if ( curr == 0x55 ) {

            data[0] = curr;            
    
            if ( read(stream, &data[1], sizeof(data[1])) != sizeof(data[1])) {
                err(5, "err while reading from %s", argv[1]);
            }

            if ( data[1] < 3 ) {
                if ( lseek(stream, -1, SEEK_CUR) < 0 ) {
                    err(9, "err while lseeking");
                }
                continue;
            }
            
            uint8_t check_sum = data[0] ^ data[1];
            for(int i = 2; i < data[1] - 1; i++) {
                
                if ( read(stream, &data[i], sizeof(data[i])) != sizeof(data[i])) {
                    err(6, "err while reading from %s", argv[1]);
                }

                check_sum ^= data[i];
            }

            uint8_t target;
            if ( read(stream, &target, sizeof(target)) != sizeof(target)) {
                err(7, "err while reading from %s", argv[1]);
            }

            if ( target == check_sum ) {
                data[data[1] - 1] = target;
                
                for ( int j = 0; j < data[1]; j++) {
                    if ( write(msg, &data[j], sizeof(data[j])) != sizeof(data[j])) {
                        err(8, "err while writing to %s", argv[2]);
                    }
                }
            }

            else {
                if ( lseek(stream, -data[1] + 1, SEEK_CUR) < 0 ) {
                    err(9, "err while lseeking");
                }
            }
        }
    }

    if ( bytes < 0 ) {
        err(4, "err while reading from %s", argv[1]);
    }
    
    close(stream);
    close(msg);
	return 0;
}
