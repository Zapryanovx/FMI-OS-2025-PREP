#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <err.h>

int main(int argc, char* argv[]) {

    if ( argc != 2 ) {
        errx(1, "1 arg expected");
    }

    const char* file_path = "fifo";

    int fd = open(file_path, O_RDONLY);
    if ( fd < 0 ) {
        err(2, "couldnt open %s", file_path);
    }

    if ( dup2(fd, 0) < 0 ) {
        err(3, "couldnt dup2");
    }
    
    close(fd);

    if ( unlink(file_path) < 0) {
        err(4, "couldnt unlink");
    }

    execlp(argv[1], argv[1], (char*)NULL);
    err(5, "couldnt execvp");

	return 0;
}
