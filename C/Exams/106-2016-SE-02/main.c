#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

int main(void) {

while(true) {

        write(1, "$", 1);

        char cmd[4096];
        int size;
        if ((size=read(0, cmd, 4096)) < 0 ) {
            err(1, "err while reading cmd");
        }
        
        cmd[size - 1] = '\0';

        if(!strcmp("exit", cmd)) {
            break;
        }
    
        pid_t pid = fork();
        if ( pid < 0 ) {
            err(2, "couldnt fork");
        }

        if(pid == 0) {
            execlp(cmd, cmd, (char*)NULL);
            err(3, "couldnt execlp");
        }
        
        wait(NULL);
    }
    
	return 0;
}
