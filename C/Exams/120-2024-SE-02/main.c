#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <err.h>
#include <sys/wait.h>

pid_t pids[10];
int n;

int findIdx(pid_t pid) {
    
    for ( int i = 0; i < n; i++ ) {
        if ( pids[i] == pid ) {
            return i;
        }
    }

    return -1;
}

int main(int argc, char* argv[]) {

    if ( argc < 2 || argc > 11 ) {
        errx(1, "1 to 10 args expected");
    }

    n = argc - 1;
    for ( int i = 0; i < n; i++ ) {
        
        pid_t pid = fork();
        if ( pid < 0 ) {
            err(2, "couldnt fork");
        }
        
        if ( pid == 0 ) {
            execlp(argv[i], argv[i], (char*)NULL);
            err(3, "couldnt execlp");
        }

        pids[i] = pid;
    }

    int cnt = 0;
    while ( cnt < n ) {
        
        int status;
        pid_t pid = wait(&status);
        
        if ( pid < 0 ) {
            err(4, "couldnt wait");
        }

        int idx = findIdx(pid);
        if ( idx == -1 ) {
            continue;
        }

        if ( WIFEXITED(status) && WEXITSTATUS(status) == 0 ) {
            cnt++;
            pids[idx] = -1;
        
        } else if ( WIFEXITED(status) && WEXITSTATUS(status) != 0 ) {
            
            int newPid = fork();
            if ( newPid < 0 ) {
                err(5, "couldnt fork");
            }   

            if ( newPid == 0 ) {
                execlp(argv[idx + 1], argv[idx + 1], (char*)NULL);
                err(6, "couldnt execlp");
            }

            pids[idx] = newPid;
        
        } else {
            
            for ( int i = 0; i < n; i++ ) {
                if ( pids[i] > 0 ) {
                        kill(pids[i], SIGTERM);
                        if ( waitpid(pids[i], NULL, 0 ) < 0 ) {
                            err(7, "couldnt wait");
                        }
                }
            }
            
            exit(idx + 1);
        }

    }

	return 0;
}
