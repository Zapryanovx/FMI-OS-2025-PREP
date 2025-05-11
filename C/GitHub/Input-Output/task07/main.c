#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <libgen.h>

int main(int argc, char* argv[]) {
	
    if(argc < 3) {
        errx(1, "at least 2 filenames expected as args."); 
    }

    struct stat st;
    if(stat(argv[argc - 1], &st) == -1 || !S_ISDIR(st.st_mode)){
        errx(2, "last argument must be an existing dir");
    }

    for(int i=1; i<argc-1; i++){
        int from=open(argv[i], O_RDONLY);
        if(from==-1){
            errx(3, "cannot open %s", argv[i]);
        }

        char* base=basename(argv[i]);

        char path[4096];
        strcpy(path, argv[argc - 1]);
        strcat(path, "/");
        strcat(path, base);
    
        int to=open(path, O_WRONLY | O_TRUNC | O_CREAT, 0644);
        if(to==-1){

            close(from);
            errx(3, "error while creating copy file");
        }

        char ch;
        int bytes;
        while((bytes=read(from, &ch, sizeof(ch))) > 0){
            if(write(to, &ch, sizeof(ch)) != 1){
                close(from);
                close(to);
                errx(4, "error while copying");
            }
        }

        close(from);
        close(to);
    }

	exit(0);
}
