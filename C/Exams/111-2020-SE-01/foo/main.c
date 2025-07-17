#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <errno.h>

int main(int argc, char* argv[]) {
    
    if ( argc != 2 ) {
        errx(1, "1 arg expected");
    }

    const char* fifo_path = "fifo";
    if ( mkfifo(fifo_path, 0666) < 0) {
        if ( errno != EEXIST ) {
            err(2, "couldnt mkfifo");
        }
    }

    int fd = open(fifo_path, O_WRONLY);
    if ( fd < 0 ) {
        err(3, "couldnt open %s", fifo_path);
    }

    if ( dup2(fd, 1) < 0 ) {
        err(4, "couldnt dup2");
    }
    
    close(fd);
    execlp("cat", "cat", argv[1]);
    err(5, "couldnt execlp");

	return 0;
}
