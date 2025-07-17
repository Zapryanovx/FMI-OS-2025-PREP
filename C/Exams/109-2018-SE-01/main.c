#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <fcntl.h>
#include <err.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
    
    if ( argc != 2 ) {
        errx(1, "1 arg expected");
    }   

    int find_to_sort[2];
    if ( pipe(find_to_sort) < 0 ) {
        err(2, "couldnt pipe");
    }

    pid_t pid = fork();
    if ( pid < 0 ) {
        err(3, "coudlnt fork");
    }

    if ( pid == 0 ) {
        close(find_to_sort[0]);

        dup2(find_to_sort[1], 1);
        close(find_to_sort[1]);

        execlp("find", "find", argv[1], "-type", "f", "-printf", "%p %T@\n", (char*)NULL);
        err(4, "couldnt execlp");
    }

    close(find_to_sort[1]);    

    int sort_to_head[2];
    if ( pipe(sort_to_head) < 0 ) {
        err(5, "couldnt pipe");
    }

    pid = fork();
    if ( pid < 0 ) {
        err(6, "couldnt fork");
    }

    if ( pid == 0 ) {
        close(sort_to_head[0]);

        dup2(find_to_sort[0], 0);
        close(find_to_sort[0]);

        dup2(sort_to_head[1], 1);
        close(sort_to_head[1]);

        execlp("sort", "sort", "-k2nr", (char*)NULL);
        err(7, "couldnt execlp");
    }

    close(sort_to_head[1]);
    close(find_to_sort[0]);
    
    int head_to_cut[2];
    if ( pipe(head_to_cut) < 0 ) {
        err(8, "coudlnt pipe");
    }

    pid = fork();
    if ( pid < 0 ) {
        err(9, "couldnt fork");
    }

    if ( pid == 0 ) {
        close(head_to_cut[0]);
        
        dup2(sort_to_head[0], 0);
        close(sort_to_head[0]);
            
        dup2(head_to_cut[1], 1);
        close(head_to_cut[1]);

        execlp("head", "head", "-n", "1", (char*)NULL);
        err(10, "couldnt execlp");
    }

    close(head_to_cut[1]);
    close(sort_to_head[0]);

    pid = fork();
    if ( pid < 0 ) {
        err(11, "couldnt fork");
    }

    if ( pid == 0 ) {
        
        dup2(head_to_cut[0], 0);
        close(head_to_cut[0]);

        execlp("cut", "cut", "-d", " ", "-f", "1", (char*)NULL);
        err(12, "couldnt execlp");
    }

    close(head_to_cut[1]);

    for (int i = 0; i < 5; i++) {
        int status;
        wait(&status);

        if( !WIFEXITED(status) ) {
            warnx("child was killed");
        } else if ( WEXITSTATUS(status) != 0 ) {
            warnx("exit status != 0");
        }
    }

	return 0;
}
