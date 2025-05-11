#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <err.h>

int main(int argc, char* argv[]) {

        if(argc != 2) {
        errx(1, "1 filename expected.");
        }

        int file=open(argv[1], O_RDONLY);
        if(file == -1) {
        errx(2, "%s couldnt be opened", argv[1]);
        }

        int lines=0;
        int words=0;
        int bytes=0;
        char ch;

    int state;
        while((state=read(file, &ch, sizeof(ch))) > 0){
        if(ch == '\n'){
            lines++;
            words++;
        }

        else if(ch == ' '){
            words++;
        }

        bytes++;
        }

    if(state == -1) {
        errx(3, "error while reading from %s", argv[1]);
    }

    printf("File %s has:\n%d lines.\n%d words.\n%d bytes.\n", argv[1], lines, words, bytes);

    if(close(file) == -1){
        errx(4, "couldnt close %s", argv[1]);
    }

        exit(0);
}

