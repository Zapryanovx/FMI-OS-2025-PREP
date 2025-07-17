#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <err.h>
#include <stdbool.h>

int main(int argc, char* argv[]) {
    
    if ( argc == 1 ) {
        errx(1, "at least 1 arg expected");
    }

    int pids[4096];
    int cnt = 0;

    int pipefd[2];
    if ( pipe(pipefd) < 0 ) {
        err(2, "couldnt pipe");
    }   

    for ( int i = 1; i < argc; i++ ) {
        
        pid_t pid = fork();
        if ( pid < 0 ) {
            err(3, "couldnt fork"); 
        }

        if ( pid == 0 ) {
            close(pipefd[0]);

            if ( dup2(pipefd[1], 1) < 0 ) {
                err(4, "couldnt dup2");
            }
            close(pipefd[1]);

            execlp(argv[i], argv[i], (char*)NULL);
            exit(26);
        } 

        pids[cnt++] = pid;
    }

    close(pipefd[1]);

    const char* signal = "found it!";

    int status;
    char byte;
    int len = 0;
    
    bool is_found_it = false;
    while ( (status=read(pipefd[0], &byte, sizeof(byte))) == sizeof(byte) ) {

        if ( byte == signal[len] ) {
            len++;
        } else {
            len = 0;
        }

        if ( len == (int)strlen(signal) ) {
            is_found_it = true;
            for ( int j = 0; j < cnt; j++ ) {
                if ( kill(pids[j], SIGTERM ) < 0 ) {
                    err(6, "couldnt kill");
                }
            }

            exit(0);
        }

    } if ( status < 0 ) {
        err(6, "couldnt read");
    }

    close(pipefd[0]);
    if ( is_found_it ) {
        exit(0);
    }

    if ( wait(NULL) > 0 ) {}
    
    exit(1);
}
