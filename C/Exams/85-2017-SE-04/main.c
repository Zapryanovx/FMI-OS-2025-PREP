#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdint.h>
#include <err.h>

void readFromFd(int fd) {
    
    uint8_t byte;
    int res;
    
    while ((res = read(fd, &byte, sizeof(byte))) == sizeof(byte)) {
        if(write(1, &byte, sizeof(byte)) == -1) {
            err(1, "couldnt write to fd");
        }
    }
}

int main(int argc, char* argv[]) {
	
    if(argc == 1) {
        readFromFd(0); 
        return 0;
    }
    
    int fd;
    for(int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            fd = 0;
        } 

        else {
            fd = open(argv[i], O_RDONLY);
            if(fd == -1){
                err(2, "couldnt open file %s", argv[i]);
            }
        }
        
        readFromFd(fd);
        close(fd);
    }
    
	return 0;
}
