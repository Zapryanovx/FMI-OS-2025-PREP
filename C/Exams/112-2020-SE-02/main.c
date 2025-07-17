#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <stdint.h>

int main(int argc, char* argv[]) {

    if ( argc != 3 ) {
        errx(1, "2 args expected");
    }

    int dst = open(argv[2], O_WRONLY | O_TRUNC | O_CREAT, 0644);
    if ( dst < 0 ) {
        err(3, "couldnt open %s", argv[2]);
    }
    
    int cat_to_decode[2];
    if ( pipe(cat_to_decode) < 0 ) {
        err(4, "couldnt pipe");
    }

    pid_t pid = fork();
    if ( pid < 0 ) {
        err(5, "couldnt fork");
    }

    if ( pid == 0 ) {
        close(cat_to_decode[0]);

        if ( dup2(cat_to_decode[1], 1) < 0 ) {
            err(6, "couldnt dup2");
        }

        close(cat_to_decode[1]);
        execlp("cat", "cat", argv[1], (char*)NULL);
        err(7, "couldnt execlp");
    }

    close(cat_to_decode[1]);

    dup2(cat_to_decode[0], 0);
    close(cat_to_decode[0]);

    uint8_t mask = 0x20;
    
    int status;
    uint8_t byte;

    while ( (status = read(0, &byte, sizeof(uint8_t))) == sizeof(uint8_t) ) {
        
        if ( byte == 0x7D ) {
            
            if ( read(0, &byte, sizeof(uint8_t)) != sizeof(uint8_t) ) {
                err(8, "err while reading");
            }

            uint8_t orig = byte ^ mask;
            if ( orig != 0x00 && orig != 0xFF && orig != 0x55 && orig != 0x7D ) {
                err(8, "invalid format");
            }   

            if ( write(dst, &orig, sizeof(uint8_t)) != sizeof(uint8_t) ) {
                err(10, "err while writing");
            }

        } else if ( byte != 0x55 ) {
     
            if ( write(dst, &byte, sizeof(uint8_t)) != sizeof(uint8_t) ) {
                err(11, "err while writing");
            }        
        }

    } if ( status < 0 ) {
        err(12, "err while reading");
    }

    close(dst);
    close(cat_to_decode[0]);
	return 0;
}
