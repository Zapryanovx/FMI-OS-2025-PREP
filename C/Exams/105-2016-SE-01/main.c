#include <sys/stat.h>
#include <fcntl.h>
#include <err.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
	
    if ( argc != 2 ) {
        errx(1, "1 arg expected");
    }

    int pfd[2];
    if ( pipe(pfd) < 0 ) {
        err(2, "couldnt create pipe");
    }

    pid_t pid = fork();
    if ( pid < 0 ) {
        err(3, "couldnt fork");
    }

    if ( pid == 0 ) {
        close(pfd[0]);
        
        if(dup2(pfd[1], 1) < 0) {
            err(4, "couldnt dup2");
        }

        close(pfd[1]);
        execlp("cat", "cat", argv[1], (char*)NULL);
        err(5, "couldnt execlp");
    }
    
    wait(NULL);

    close(pfd[1]);
    if(dup2(pfd[0], 0) < 0) {
        err(6, "couldnt dup2");
    }
    
    close(pfd[0]);
    execlp("sort", "sort", (char*)NULL);
    err(7, "couldnt execlp");

	return 0;
}
