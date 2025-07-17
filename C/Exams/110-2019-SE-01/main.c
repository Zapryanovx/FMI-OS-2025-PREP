#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <sys/wait.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    
    if ( argc < 3 ) {
        errx(1, "at least 2 args expected");
    }

    size_t dur = atoi(argv[1]);
    if ( dur < 1 || dur > 9 ) {
        errx(2, "invalid duration");
    }

    int fd = open("run.log", O_WRONLY | O_TRUNC | O_CREAT, 0644);
    if ( fd < 0 ) {
        err(3, "couldnt open run.log");
    }

    int cnt = 0;
    while(true) {
        
        pid_t pid = fork();
        size_t beg = time(NULL);
        
        if ( pid < 0 ) {
            err(4, "couldnt fork");
        }

        if ( pid == 0 ) {
            
            char** params = argv + 2;
            execvp(argv[2], params);
            err(5, "couldnt execvp");
        }

        int status;
        if ( wait(&status) < 0 ){
            err(6, "couldnt wait");
        }

        size_t now = time(NULL);

        int code = 0;
        if ( !WIFEXITED(status) ) {
            code = 129;
        } else {
            code = WEXITSTATUS(status);
        }
        
        char log[256];
        if( snprintf(log, 256, "%ld %ld %d\n", beg, now, code) < 0 ) {
            err(7, "couldnt snprintf");
        }

        if ( write(fd, &log, strlen(log)) != (ssize_t)strlen(log) ) {
            err(8, "err while writing to run.log");
        }

        if ( code != 0 || now - beg < dur) {
            cnt++;
        } else {
            cnt = 0;
        }

        if (cnt == 2) {
            break;
        }   
    }
    
    close(fd);
	return 0;
}
