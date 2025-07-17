#include <sys/stat.h>
#include <stdint.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <err.h>

void print_prefix(int line) {
    
    char buff[256];
    int size = snprintf(buff, 256, "%d: ", line);            
    if( write(1, buff, size) != size )
    {
        err(4, "err while writing to stdin");
    }
 
}

int find_fd(int i, char* argv[]) {
    
    int fd = -1;
    if ( argv[i][0] == '-') {
            fd = 0;
    }

    else {
        fd = open(argv[i], O_RDONLY);
        if ( fd < 0 ) {
            err(1, "couldnt open %s", argv[i]);
        }
    }

    return fd;
}

void process_fd(int fd, bool is_n, int *line) {
    
    uint8_t curr;
    int status;
    bool need_prefix = is_n;

    while (( status = read(fd, &curr, sizeof(curr))) == sizeof(curr)) {
        if (is_n && need_prefix) {
            print_prefix(*line);
            need_prefix = false;
        }

        if (write(1, &curr, 1) != 1) {
            err(3, "err while writing to stdout");
        }

        if (is_n && curr == '\n') {
            (*line)++;
            need_prefix = true;
        }
    }

    if ( status < 0 ) {
        err(10, "err while reading");
    }
}

int main(int argc, char* argv[]) {
	
    bool is_n = false;
    if( argc > 1 ) {
        if ( strcmp(argv[1], "-n") == 0) {
            is_n = true;
        }
    }
    
    int start = is_n ? 2 : 1;
    int line = 1;
    
    if (argc <= start) {
        process_fd(0, is_n, &line);
        return 0;
    }

    for (int i = start; i < argc; i++) {

        int fd = (argv[i][0] == '-') 
                 ? 0 
                 : open(argv[i], O_RDONLY);
    
        if (fd < 0) {
           err(1, "couldnt open %s", argv[i]);
        }

        process_fd(fd, is_n, &line);
    
        if ( fd != 0 ) {
            close(fd);
        }
    }

	return 0;
}
