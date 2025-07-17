#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {

    if ( argc != 3 ) {
        errx(1, "2 args expected");
    }
    
    int n = atoi(argv[1]);
    int d = atoi(argv[2]);
    
    if ( n < 0 || n > 9 || d < 0 || d > 9 ) {
        errx(2, "invalid params");
    }
    
    int ding_to_dong[2];
    int dong_to_ding[2];

    if ( pipe(ding_to_dong) < 0 ) {
        err(3, "couldnt pipe");
    }

    if ( pipe(dong_to_ding) < 0 ) {
        err(4, "couldnt pipe");
    }

    pid_t pid = fork();
    if ( pid < 0 ) {
        err(3, "couldnt fork");
    }

    if ( pid == 0 ) {
        close(ding_to_dong[1]);
        close(dong_to_ding[0]);

        char flag;
        int status;

        while (( status = read(ding_to_dong[0], &flag, 1)) == 1 ) {

            if ( write(1, "DONG\n", 5) != 5 ) {
                err(4, "couldnt write");
            }

            if ( write(dong_to_ding[1], "c", 1) != 1 ) {
                err(5, "couldnt write");
            }

        } if ( status < 0 ) {
            err(6, "err while reading");
        }

        close(dong_to_ding[1]);
        close(ding_to_dong[0]);
    
    } else {
        
        close(dong_to_ding[1]);
        close(ding_to_dong[0]);

        char flag;
        for ( int i = 0; i < n; i++ ) {
            
            if ( write(1, "DING\n", 5) != 5 ) {
                err(7, "couldnt write");
            }

            if ( write(ding_to_dong[1], "p", 1) != 1 ) {
                err(8, "couldnt write");
            }

            if ( read(dong_to_ding[0], &flag, sizeof(flag)) != sizeof(flag) ) {
                err(9, "couldnt read");
            }

            sleep(d);
        }
    
    }   

   	return 0;
}
