#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <err.h>

int main(void) {
    
    const char* source="/etc/passwd";
    const char* reformatted="passwd";
    
    int from=open(source, O_RDONLY);
    if(from==-1){
        errx(1, "error while opening %s", source);
    }

    int to=open(reformatted, O_WRONLY | O_TRUNC | O_CREAT, 0644);
    if(to==-1){
        close(from);
        errx(2, "error while opening %s", reformatted);
    }

    char ch;
    int bytes;
    while((bytes=read(from, &ch, sizeof(ch))) > 0){
        if(ch==':'){
            if(write(to, "?", 1) != 1){
                close(from);
                close(to);
                errx(3, "error while replacing :");
            }
        }

        else
        {
            if(write(to, &ch, sizeof(ch)) != 1){
                close(from);
                close(to);
                errx(4, "error while copying from %s", source);
            }
        }   
    }
    
    if(bytes==-1){
        close(from);
        close(to);
        errx(4, "error while reading from %s", reformatted);
    }

    close(from);
    close(to);
    exit(0);
}
