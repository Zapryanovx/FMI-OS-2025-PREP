#include <sys/stat.h>
#include <unistd.h>
#include <stdint.h>
#include <err.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>

typedef struct {
    uint32_t magic;
    uint32_t packet_count;
    uint64_t original_size;
} header_t;

int main(int argc, char* argv[]) {
	
    if (argc != 3) {
        errx(1, "2 files expected");
    }

    int compressed = open(argv[1], O_RDONLY);
    if ( compressed < 0 ) {
        err(2, "couldnt open %s", argv[1]);
    }

    int original = open(argv[2], O_WRONLY | O_TRUNC | O_CREAT, 0644);
    if ( original < 0 ) {
        err(3, "couldnt open/create %s", argv[2]);
    }

    header_t hd;
    if ( read(compressed, &hd, sizeof(hd)) != sizeof(hd)) {
        err(4, "couldnt read header");
    }

    if ( hd.magic != 0x21494D46 ) {
        err(5, "invalid header");
    }

    dprintf(1, "%d\n", hd.packet_count);

    for(uint32_t i = 0; i < hd.packet_count; i++) {
        
        uint8_t data_byte;
        bool type;
        uint8_t next;
        if(read(compressed, &data_byte, sizeof(data_byte)) != sizeof(data_byte)) {
            err(6, "err while reading from compressed version");
        }  

        type = data_byte >> 7;
        next = data_byte & ~(1 << 7);
        
        dprintf(1, "%d : %d", type, next);

        if(!type) {
            for(int j = 0; j < next + 1; j++) {
                uint8_t byte;
                
                if(read(compressed, &byte, sizeof(byte)) != sizeof(byte)) {
                    err(6, "err while reading from the compressed version");
                }

                if(write(original, &byte, sizeof(byte)) != sizeof(byte)) {
                    err(7, "err while writing to original");
                }

            }
        }

        if(type) {
            uint8_t byte;
            
            if(read(compressed, &byte, sizeof(byte)) != sizeof(byte)) {
                err(8, "err while reading from the compressed version");
            }
    
            for(int j = 0; j < next + 1; j++) {
                if(write(original, &byte, sizeof(byte)) != sizeof(byte)) {
                    err(9, "err while writing to original");
                }
            }
        }
    

    }

    close(compressed);
    close(original);
	return 0;
}
