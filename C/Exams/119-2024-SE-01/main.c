#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <err.h>

int main(int argc, char* argv[]) {
	
	if ( argc != 4 ) {
        errx(1, "3 args expected");
	}

	uint8_t n = atoi(argv[2]);

	int rfd = open("/dev/urandom", O_RDONLY);
	if ( rfd < 0 ) {
        err(2, "couldnt open");
	}

	for ( uint8_t i = 0; i < n; i++ ) {
        
        uint16_t s;
	    if ( read(rfd, &s, sizeof(uint16_t)) != sizeof(uint16_t) ) {
            err(3, "couldnt read");
	    }

        uint16_t bytes[s];
        if ( read(rfd, bytes, s) != s ) {
            err(4, "couldnt read");
        }

        int pipefd[2];
        if ( pipe(pipefd) < 0 ) {
            err(5, "couldnt fork");
        }

        pid_t pid = fork();
        if ( pid < 0 ) {
            err(6, "couldnt fork");
        }

        if ( pid == 0 ) {
            int nullfd = open("/dev/null", O_WRONLY);
            if ( nullfd < 0 ) {
                err(7, "couldnt open");
            }

            close(pipefd[1]);
            
            if ( dup2(pipefd[0], 0) < 0 ) {
                err(8, "couldnt dup2");
            }
            close(pipefd[0]);

            if( dup2(nullfd, 1) < 0 || dup2(nullfd, 2) < 0 ) {
                err(9, "couldnt dup2");
            }  
            
            close(nullfd);
            execlp(argv[1], argv[1], (char*)NULL);
            err(10, "couldnt execlp");
        }

        close(pipefd[0]);
        
        if ( write(pipefd[1], bytes, s) != s ) {
            err(11, "couldnt write");
        }
        close(pipefd[1]);
        
        int status;
        if ( waitpid(pid, &status, 0) < 0 ) {
            err(12, "couldnt wait");
        }

        if(!WIFEXITED(status)) {
            int resfd = open(argv[3], O_WRONLY | O_TRUNC | O_CREAT, 0644);
            if ( resfd < 0 ) {
                err(13, "couldnt open");
            }  

            if ( write(resfd, bytes, s) != s ) {
                err(14, "couldnt write");
            }

            close(resfd);
            close(rfd);
            return 42;
        } 
    }
    
    int resfd = open(argv[3], O_WRONLY | O_TRUNC | O_CREAT, 0644);
    if ( resfd < 0 ) {
        err(15, "couldnt open");
    }

    close(resfd);
    close(rfd);
	return 0;
}
