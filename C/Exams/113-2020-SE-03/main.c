#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <err.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct {
    char path[8];
    uint32_t offset;
    uint32_t length;
} triplet;

int main(int argc, char* argv[]) {

    if ( argc != 2 ) {
        errx(1, "1 arg expected");
    }

    int fd = open(argv[1], O_RDONLY);
    if ( fd < 0 ) {
        err(2, "couldnt open %s", argv[1]);
    }
    
    struct stat st;
    if ( fstat(fd, &st) < 0 ) {
        err(3, "couldnt fstat");
    }

    if ( st.st_size % sizeof(triplet) != 0 ) {
        err(4, "invalid file format");
    }

    int status;
    int cnt = 0;
    triplet t;

    int pipes[8][2];
    pid_t pids[8];

    while((status = read(fd, &t, sizeof(triplet))) == sizeof(triplet)) {
        if ( cnt >= 8 ) {
            err(5, "limit reached");
        }

        if ( pipe(pipes[cnt]) < 0 ) {
            err(6, "couldnt pipe");
        }

        pid_t pid = fork();
        if ( pid < 0 ) {
            err(7, "couldnt fork");
        }

        if ( pid == 0 ) {
            close(pipes[cnt][0]);
    
            int curr_fd = open(t.path, O_RDONLY);
            if ( curr_fd < 0 ) {
                err(8, "couldnt open triplet file");
            }   

            if ( lseek(curr_fd, t.offset * sizeof(uint16_t), SEEK_SET) < 0 ) {
                err(9, "err while lseeking");
            }

            uint16_t curr = 0;
            uint16_t res_file = 0;
            for ( uint16_t i = 0; i < t.length; i++ ) {
                
                if ( read(curr_fd, &curr, sizeof(uint16_t)) != sizeof(uint16_t) ) {
                    err(10, "err while reading");
                }

                res_file ^= curr;
            }

            if ( write(pipes[cnt][1], &res_file, sizeof(uint16_t)) != sizeof(uint16_t) ) {
                err(11, "err while writing");
            }
            
            close(pipes[cnt][1]);
            close(curr_fd);
            exit(0);   
        } 
        
        close(pipes[cnt][1]);
        pids[cnt] = pid;
        cnt++;
              
    } if ( status < 0 ) {
        err(11, "err while reading");
    }

    uint16_t final_res = 0;
    for ( int i = 0; i < cnt; i++ ) {
        uint16_t child_res = 0;

        if ( read(pipes[i][0], &child_res, sizeof(uint16_t)) != sizeof(uint16_t) ) {
            err(12, "err while reading from child");
        } 

        final_res ^= child_res;
        close(pipes[i][0]);
    }
    
    for ( int i = 0; i < cnt; i++ ) {
        int temp;
        if ( waitpid(pids[i], &temp, 0) < 0 ) {
            err(13, "couldnt waitpid");
        }

        if ( !WIFEXITED(status) ) {
            warnx("child %d was killed", pids[i]);
        } else if ( WEXITSTATUS(status) != 0 ) {
            warnx("exit status != 0 in %d", pids[i]);
        }
    }

    char buff[1024];
    if ( snprintf(buff, 1024, "%04X\n", final_res) < 0 ) {
        err(14, "couldnt snprintf");
    }

    if ( write(1, buff, strlen(buff)) != (ssize_t)strlen(buff) ) {
        err(15, "err while writing to stdout");
    }
    
	return 0;
}
