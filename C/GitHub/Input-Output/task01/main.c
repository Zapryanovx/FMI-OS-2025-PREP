#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <err.h>
#include <fcntl.h>

int main(int argc, char* argv[]) {

    if (argc != 3) {
        errx(1, "2 files expected");
    }

    int from = open(argv[1], O_RDONLY);
    if (from < 0) {
        errx(2, "source file [%s] couldnt be opened.", argv[1]);
    }

    int to = open(argv[2], O_WRONLY | O_TRUNC | O_CREAT, 0644);
    if (to < 0) {
        errx(3, "dist file [%s] couldnt be opened.", argv[2]);
    }  

    char ch;
    int flag;
    while((flag = read(from, &ch, sizeof(ch))) > 0) {
        if (write(to, &ch, sizeof(ch)) != 1) {
            errx(4, "error while copying to %s.", argv[2]);
        }
    }

    if (flag == -1) {
        errx(5, "error while reading from %s.", argv[1]);
    }

    if(close(from) == -1){
        errx(6, "error while closing %s.", argv[1]);
    }

    if(close(to) == -1){
        errx(7, "error while closing %s.", argv[2]);
    }
    
    exit(0);
}
