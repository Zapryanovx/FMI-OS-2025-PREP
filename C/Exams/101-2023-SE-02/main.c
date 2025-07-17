#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>
#include <unistd.h>
#include <err.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

void read_word(int dict, char word[], int m) {
    
    if ( lseek(dict, m, SEEK_SET) < 0) {
        err(4, "err while lseeking");
    }

    int bytes;
    uint8_t curr;
    bool is_found=false;
    
    while (( bytes = read(dict, &curr, sizeof(curr))) == sizeof(curr)) {
        if ( curr == 0 ) {
            is_found=true;
            break;
        }
    }

    if ( bytes < 0 ) {
        err(5, "err while reading");
    }
    
    int at = 0;
    if ( is_found == true ) {
        while (( bytes = read(dict, &curr, sizeof(curr))) == sizeof(curr)) {
            if ( curr == '\n' ) {
                break;
            }

            word[at++] = curr;
        }

        if ( bytes < 0 ) {
            err(6, "err while reading");
        }
    }

    word[at] = '\0';
}

void print_desc(int dict) {

    uint8_t curr;
    int bytes;
    while (( bytes = read(dict, &curr, sizeof(curr))) == sizeof(curr)) { 
        if ( curr == '\n' ) {
            if ( write(1, &curr, sizeof(curr)) != sizeof(curr)) {
                err(5, "err while reading to stdout");
            }
            break;
        }

        if ( write(1, &curr, sizeof(curr)) != sizeof(curr)) {
            err(5, "err while reading to stdout");
        }
    }

}

int main(int argc, char* argv[]) {
    
    if ( argc != 3 ) {
        errx(1, "word and a file expected");
    }

    int dict = open(argv[2], O_RDONLY);
    if ( dict < 0 ) {
        err(2, "couldnt open %s", argv[2]);
    }

    int l = lseek(dict, 0, SEEK_SET);
    int r = lseek(dict, -1, SEEK_END);
    
    while( l < r) {
        
        int m = l + (r - l) / 2;
        char word[64];
        read_word(dict, word, m);

        if(strcmp(word, argv[1]) == 0) {
            print_desc(dict);
            close(dict);
            return 0;
        }

        else if(strcmp(word, argv[1]) < 0) {
            l = m + 1;
        }

        else {
            r = m - 1;
        }
    }

    write(1, "couldnt find word\n", 19);
	close(dict);
	return 0;
}
