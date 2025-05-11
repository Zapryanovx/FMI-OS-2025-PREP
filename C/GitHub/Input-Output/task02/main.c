#include <stdlib.h>
#include <fcntl.h>
#include <err.h>
#include <unistd.h>

int main(int argc, char* argv[]) {

    if(argc != 2) {
        errx(1, "one filename arg expected.");
    }

    int file = open(argv[1], O_RDONLY);
    if(file == -1) {
        errx(2, "couldnt open file [%s]", argv[1]);
    }

    int counter=0;
    int bytes=0;
    char ch;
    while((bytes = read(file, &ch, sizeof(ch))) > 0) {
        if(write(1, &ch, sizeof(ch)) != 1) {
            errx(3, "error in writing to stdout");
        }

        if(ch=='\n') {
            counter++;
        }

        if(counter==10) {
            break;
        }
    }

    if(bytes == -1) {
        errx(4, "error in reading from %s", argv[1]);
    }

    if(close(file) == -1) {
        errx(5, "couldnt close %s", argv[1]);
    }

    exit(0);
}
