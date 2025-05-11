#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char* argv[]) {
	
	if(argc != 3) {
        errx(1, "2 filenames expected as args,");
	}

	int from=open(argv[1], O_RDONLY);
	if(from == -1){
        errx(2, "%s couldnt be opened", argv[1]);
	}

	int to=open(argv[2], O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if(to == -1){
        errx(3, "%s couldnt be opened", argv[2]);
	}

	char ch;
	int bytes;
	while((bytes=read(from, &ch, sizeof(ch))) > 0) {
        if(write(to, &ch, sizeof(ch)) != 1) {
            errx(4, "error while copying");
        }
    }

    if(bytes==-1){
        errx(5, "error while reading from %s", argv[1]);
    }
    
    if(close(from) == -1){
        errx(6, "%s couldnt be closed", argv[1]);
    }

    if(close(to) == -1){
        errx(7, "%s couldnt be closed", argv[2]);
    }


    exit(0);
}
