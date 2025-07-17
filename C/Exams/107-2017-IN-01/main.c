#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <sys/wait.h>

int main(void) {
    
    int cut_to_sort[2];
    if ( pipe(cut_to_sort) < 0 ) {
        err(1, "couldnt pipe to sort");
    }

    pid_t pid = fork();
    if ( pid < 0 ) {
        err(2, "couldnt fork");
    }
    
    if ( pid == 0 ) {
        
        close(cut_to_sort[0]);
        dup2(cut_to_sort[1], 1);
        close(cut_to_sort[1]);
        
        execlp("cut", "cut", "-d", ":", "-f", "7", "/etc/passwd", (char*)NULL);
        err(3, "coudlnt execlp");
    }

    close(cut_to_sort[1]);

    int sort_to_uniq[2];
    if ( pipe(sort_to_uniq) ) {
        err(4, "coudlnt pipe to uniq");
    }

    pid = fork();
    if ( pid < 0 ) {
        err(5, "couldnt fork");
    }

    if ( pid == 0 ) {
        close(sort_to_uniq[0]);

        dup2(cut_to_sort[0], 0);
        close(cut_to_sort[0]);

        dup2(sort_to_uniq[1], 1);
        close(sort_to_uniq[1]);

        execlp("sort", "sort", (char*)NULL);
        err(6, "couldnt execlp");
    }

    close(cut_to_sort[0]);
    close(sort_to_uniq[1]);

    int uniq_to_sort[2];
    if ( pipe(uniq_to_sort) < 0 ) {
        err(7, "couldnt pipe to sort");
    }

    pid = fork();
    if ( pid < 0 ) {
        err(8, "couldnt fork");
    }

    if ( pid == 0 ) {
        close(uniq_to_sort[0]);

        dup2(sort_to_uniq[0], 0);
        close(sort_to_uniq[0]);

        dup2(uniq_to_sort[1], 1);
        close(uniq_to_sort[1]);

        execlp("uniq", "uniq", "-c", (char*)NULL);
        err(9, "couldnt execlp");
    }

    close(sort_to_uniq[0]);
    close(uniq_to_sort[1]);

    pid = fork();
    if ( pid < 0 ) {
        err(11, "couldnt fork");
    }

    if ( pid == 0 ) {
        
        dup2(uniq_to_sort[0], 0);
        close(uniq_to_sort[0]);

        execlp("sort", "sort", "-n", (char*)NULL);
        err(12, "couldnt execlp");
    }

    close(uniq_to_sort[0]);

    for( int i = 0; i < 4; i++ ) {
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
