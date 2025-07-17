#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>
#include <err.h>
#include <string.h>

void remove_symbols(int argc, char* argv[]) {
    
    if ( argc != 3 ) {
        errx(2, "-d expects one str");
    }

    int status;
    uint8_t byte;
    
    while (( status=read(0, &byte, sizeof(byte))) == sizeof(byte)) {
        
        bool is_del=false;
        for ( size_t i = 0; i < strlen(argv[2]); i++) {
            if ( byte == argv[2][i] ) {
                is_del = true;
            }
        }

        if ( is_del == false ) {
            if ( write(1, &byte, sizeof(byte)) != sizeof(byte) ){
                err(3, "err while writing to stdout");
            }
        }
    }

    if ( status < 0 ) {
        err(4, "err while reading from stdin");
    }
}

void squeeze_repeats(int argc, char* argv[]) {
    
    if ( argc != 3 ) {
        errx(2, "-s expects one str");
    }
    
    int status;
    uint8_t byte, prev;
    bool first=true;

    while (( status=read(0, &byte, sizeof(byte))) == sizeof(byte)) {
        
        if ( first || byte != prev || strchr(argv[2], byte) == NULL) {                 
            if ( write(1, &byte, sizeof(byte)) != sizeof(byte) ){
                err(3, "err while writing to stdout");
            }
            
            prev=byte;
            first = false;
        }  
    }

    if ( status < 0 ) {
        err(4, "err while reading from stdin");
    }    
}

void change_symbols(int argc, char* argv[]) {
    
    if ( argc != 3 ) {
        errx(5, "invalid params");
    }
    
    if ( strlen(argv[1]) != strlen(argv[2])) {
        errx(6, "invalid params");
    }

    int status;
    uint8_t byte;

    while (( status=read(0, &byte, sizeof(byte))) == sizeof(byte)) {
        if ( strchr(argv[1], byte) != NULL ) {

            int at = strchr(argv[1], byte) - argv[1];
            if ( write(1, &argv[2][at], sizeof(argv[2][at])) != sizeof(argv[2][at])) {
                err(14, "err while writing to stdout");
            }
        }

        else {
           if ( write(1, &byte, sizeof(byte)) != sizeof(byte)) {
                err(14, "err while writing to stdout");
            }

        }
    }

    if ( status < 0 ) {
        err(4, "err while reading from stdin");
    }
}

int main(int argc, char* argv[]) {
    
    if ( argc > 4 || argc == 1 ) {
        errx(1, "1 to 3 args expected");
    }
    
    if ( strcmp(argv[1], "-d") == 0) {
        remove_symbols(argc, argv);
    }

    else if ( strcmp(argv[1], "-s") == 0) {
        squeeze_repeats(argc, argv);
    }
    
    else { 
        change_symbols(argc, argv);
    }

    return 0;
}
