#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <err.h>
#include <string.h>

void createFile(char* fileName) {
    
    char hashed[256];
    strcpy(hashed, fileName);
    strcat(hashed, ".hash");

    int md5sum[2];
    if ( pipe(md5sum) < 0 ){
        err(7, "couldnt pipe");
    }

    pid_t pid = fork();
    if ( pid < 0 ){
        err(8, "couldnt fork");
    }

    if ( pid == 0 ) {
        
        close(md5sum[0]);

        if ( dup2(md5sum[1], 1) < 0 ) {
            err(9, "couldnt dup2");
        }
        close(md5sum[1]);

        execlp("md5sum", "md5sum", fileName, (char*)NULL);
        err(10, "couldnt execlp");
    }

    close(md5sum[1]);

    char md5[128];
    int len = 0;

    int status;
    char byte;

    while ( (status=read(md5sum[0], &byte, sizeof(byte))) == sizeof(byte) ) {
        if (byte != ' ' ) {
            md5[len++] = byte;
        } else {
            md5[len++] = '\n';
            md5[len] = '\0';
            break;
        }       
    } if ( status < 0 ) {
        err(11, "couldnt read");
    }

    int fd = open(hashed, O_WRONLY | O_TRUNC | O_CREAT, 0644);
    if ( fd < 0 ) {
        err(12, "couldnt open");
    }

    if ( write(fd, md5, strlen(md5)) != (ssize_t)strlen(md5) ) {
        err(13, "couldnt write");
    }
    
    close(md5sum[0]);
    close(fd);
}


int main(int argc, char* argv[]) {
    
    if ( argc != 2 ) {
        errx(1, "2 args expected");
    }

    int find[2];
    if ( pipe(find) < 0 ) {
        err(2, "couldnt pipe");
    }

    pid_t pid = fork();
    if ( pid < 0 ) {
        err(3, "couldnt fork");
    }   
    
    if ( pid == 0 ) {
        
        close(find[0]);

        if ( dup2(find[1], 1) < 0 ) {
            err(4, "couldnt dup2");
        }
        close(find[1]);

        execlp("find", "find", argv[1], "-type", "f", "!", "-name", "*.hash", (char*)NULL);
        err(5, "couldnt execlp");
    }
    
    close(find[1]);
    
    char fileName[128];
    int len = 0;
    char byte;
    int status;

    while ( (status=read(find[0], &byte, sizeof(byte))) == sizeof(byte) ) {
        
        if ( byte == '\n' ) {
            fileName[len] = '\0';
            createFile(fileName);
            len = 0;
            memset(fileName, 0, 128);
        } else {
            fileName[len++] = byte;
        }
    } if ( status < 0 ) {
        err(15, "couldnt read");
    }
    
    while(wait(NULL) > 0) {}
    

	return 0;
}
