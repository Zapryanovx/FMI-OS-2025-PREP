#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <sys/wait.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    
    char cmd[5];
    if ( argc == 1 ) {
        strcpy(cmd, "echo");
    }
    
    else { 
        if ( strlen(argv[1]) > 4 ) {
            errx(1, "invalid cmd");
        }   

        strcpy(cmd, argv[1]);
    }

    cmd[4] = '\0';
    
    char params[2][5];        
    
    char ch;
    int status;
    int currParam = 0;
    int currSymbol = 0;
    pid_t pid;

    while ( (status = read(0, &ch, sizeof(ch))) == sizeof(ch) ) {
        if ( ch == ' ' ) {
            if ( currParam == 1 ) {
                errx(8, "too many params");
            }
                
            if ( currSymbol == 0 ) {
                continue;
            }

            params[currParam][currSymbol] = '\0';
            currParam++;
            currSymbol = 0;
        } else if ( ch == '\n' ) {       
            if ( currSymbol > 0) {
                params[currParam][currSymbol] = '\0';
            } else {
                currParam = -1;
            }
            
            if ( currParam == 0 && !strcmp(params[0], "exit") ) {
                break;
            }
 
            pid = fork();
            if ( pid < 0 ) {
                err(3, "couldnt fork");
            }
    
            if ( pid == 0 ) {                    
                if ( currParam == -1 ) {
                    execlp(cmd, cmd, (char*)NULL);
                    err(5, "couldnt execlp");
                } else if ( currParam == 0 ) {
                    execlp(cmd, cmd, params[0], (char*)NULL);
                    err(6, "couldnt execlp");
                } else {
                        execlp(cmd, cmd, params[0], params[1], (char*)NULL);
                        err(7, "couldnt execlp");
                    }
                } else {
                    
                    int temp;
                    wait(&temp);
                    
                    if ( !WIFEXITED(temp) ) {
                        warnx("child was killed");
                    } else if ( WEXITSTATUS(temp) != 0 ) {
                        warnx("exit status != 0");
                    }

                    currParam=0;
                    currSymbol=0;
                    memset(params, 0, sizeof(params));
                } 
                
            } else {
                if ( currSymbol >= 4 ) {
                    err(4, "invalid cmd");
                }

                params[currParam][currSymbol++] = ch;
            }
        }
        
        if ( status < 0 ) {
            err(2, "err while reading from stdin");
        }
    
    
	return 0;
}
