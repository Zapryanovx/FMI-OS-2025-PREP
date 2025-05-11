#include <err.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    
    for(int i=1; i<argc; i++){
        
        int file=open(argv[i], O_RDONLY);
        if(file==-1){
            errx(1, "%s couldnt be opened", argv[i]);
        }

        char ch;
        int state;
        while((state=read(file, &ch, sizeof(ch))) > 0) {
            if(write(1, &ch, sizeof(ch)) != 1) {
                errx(2, "err while writing to stdout");
            }
        }

        if(state==-1){
            errx(3, "err while reading from %s", argv[i]);
        }

        if(close(file)==-1){
            errx(5, "%s couldnt be closed", argv[i]);
        }
    }        
    
    char new_line = '\n';
    if(write(1, &new_line, sizeof(char))==-1){
        errx(4, "err while pasting new line");
    }

    exit(0);
}
