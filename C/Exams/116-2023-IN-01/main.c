#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

const char* L[] = {"tic ", "tac ", "toe\n"};

void next(int from, int to, int limit) {
   
    int curr;
    int status;
    while ( (status=read(from, &curr, sizeof(curr))) == sizeof(curr) ) {
        curr++;
        if (curr > limit) {
            if ( write(to, &curr, sizeof(curr)) != sizeof(curr) ) {
                err(5, "couldnt write");
            }

            close(from);
            close(to);
            exit(0);
        }

        const char* word = L[(curr - 1) % 3];
        if ( write(1, word, strlen(word)) != (ssize_t)strlen(word) ) {
            err(6, "couldnt write");
        }

        fsync(1);
        if ( write(to, &curr, sizeof(curr)) != sizeof(curr) ) {
            err(7, "couldnt write");
        }
    }
}

int main(int argc, char* argv[]) {
    
    if (argc != 3) {
        errx(1, "2 args expected");
    }

    int nc = atoi(argv[1]);
    int wc = atoi(argv[2]);
    
    if (nc < 1 || nc > 7 || wc < 1 || wc > 35) {
        errx(2, "invalid params");
    }

    int pipe_to[8][2];
    for (int i = 0; i <= nc; i++) {
        if ( pipe(pipe_to[i]) < 0 ) {
            err(3, "couldnt pipe");
        }
    }

    for (int i = 0; i < nc; i++) {
        
        pid_t pid = fork();
        if (pid < 0) {
            err(4, "coudlnt fork");
        }

        if (pid == 0) {
            for (int j = 0; j <= nc; j++) {
                if (j == i) {
                    close(pipe_to[j][1]);
                } else if (j == i + 1) {
                        close(pipe_to[j][0]);
                } else {
                    close(pipe_to[j][0]);
                    close(pipe_to[j][1]);
                }
            }

            next(pipe_to[i][0], pipe_to[i + 1][1], wc);
        }
    }

    for (int i = 0; i <= nc; i++) {
        if (i == 0) {
            close(pipe_to[i][0]);
        } else if (i == nc) {
            close(pipe_to[i][1]);
        } else {
            close(pipe_to[i][0]);
            close(pipe_to[i][1]);
        }
    }

    int cnt = 0;
    if (write(pipe_to[0][1], &cnt, sizeof(cnt)) != sizeof(cnt)) {
        err(8, "init write");
    }

    next(pipe_to[nc][0], pipe_to[0][1], wc);
    return 0;
}

