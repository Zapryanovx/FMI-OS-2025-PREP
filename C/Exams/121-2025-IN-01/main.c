#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <err.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    uint16_t ram;
    uint16_t register_count;
    char filename[8];
} triplet;

typedef struct {
    uint8_t opcode;
    uint8_t op1;
    uint8_t op2;
    uint8_t op3;
} instructions;

size_t getFileSize(int fd) { 
    struct stat st;
    if ( fstat(fd, &st) < 0 ) {
        err(1, "couldnt fstat");
    }

    return st.st_size;
}

void handle(triplet* t) {
    int fd = open(t->filename, O_RDWR);
    if ( fd < 0 ) {
        err(9, "couldnt open");
    }

    uint8_t reg[512];
    uint16_t regCount = t->register_count;

    uint8_t ram[32];
    uint16_t ramSize = t->ram;

    if ( read(fd, reg, regCount) != regCount ) {
        err(10, "couldnt read");
    }

    if ( read(fd, ram, ramSize) != ramSize ) {
        err(11, "couldnt read");
    }

    instructions ins;
    int status;

    while ( (status=read(fd, &ins, sizeof(ins))) == sizeof(ins) ) {
        uint8_t code = ins.opcode;
        if ( code == 0 ) {
            reg[ins.op1] = reg[ins.op2] & reg[ins.op3];
        } else if ( code == 1 ) {
            reg[ins.op1] = reg[ins.op2] | reg[ins.op3];
        } else if ( code == 2 ) {
            reg[ins.op1] = reg[ins.op2] + reg[ins.op3];
        } else if ( code == 3 ) {
            reg[ins.op1] = reg[ins.op2] * reg[ins.op3];
        } else if ( code == 4 ) {
            reg[ins.op1] = reg[ins.op2] ^ reg[ins.op3];
        } else if ( code == 5 ) {
            if ( write(1, &ins.op1, sizeof(ins.op1)) != sizeof(ins.op1) ) {
                err( 12, "couldnt write");
            }
        } else if ( code == 6 ) {
            sleep(reg[ins.op1]);
        } else if ( code == 7 ) {
            reg[ins.op1] = ram[reg[ins.op2]];
        } else if ( code == 8 ) {
            ram[reg[ins.op2]] = reg[ins.op1];
        } else if ( code == 9 ) {
            if ( reg[ins.op1] != reg[ins.op2] ) {
                if ( lseek(fd, regCount + ramSize + (ins.op3 - 1) * sizeof(ins), SEEK_SET) < 0 ) {
                    err(13, "couldnt lseek");
                }
            }
        } else if ( code == 10 ) {
            reg[ins.op1] = ins.op2;
        } else if ( code == 11 ) {
            ram[reg[ins.op1]] = ins.op2;
        } else {
            errx(14, "invalid code");
        }
    
    } if ( status < 0 ) {
        err(15, "couldnt read");
    }
    
    if ( lseek(fd, 0, SEEK_SET) ) {
        err(16, "couldnt lseek");
    } 

    if ( write(fd, reg, regCount) != regCount ) {
        err(17, "couldnt write");
    }

    if ( write(fd, ram, ramSize) != ramSize ) {
        err(18, "couldnt write");
    }

    close(fd);
}

int main(int argc, char* argv[]) {
	
	if ( argc != 2 ) {
        errx(1, "1 arg expected");
	}

    int fd = open(argv[1], O_RDONLY);
    if ( fd < 0 ) {
        err(2, "couldnt open");
    }

    size_t size = getFileSize(fd);
    if ( size % sizeof(triplet) != 0 ) {
        errx(3, "incorrect file format");
    }

    size_t n = size / sizeof(triplet);

    for ( size_t i = 0; i < n; i++ ) {
        
        triplet t; 
        if ( read(fd, &t, sizeof(t)) != sizeof(t) ) {
            err(4, "couldnt read");
        }

        if ( t.ram > 512 || t.register_count > 32 ) {
            errx(5, "incorrect data found in file");
        }   

        pid_t pid = fork();
        if ( pid < 0 ) {
            err(6, "couldnt fork");
        }

        if ( pid == 0 ) {
            handle(&t);
        }
    }

    while(wait(NULL) > 0) {}
	return 0;
}
