#include <sys/stat.h>
#include <fcntl.h>
#include <err.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

char buff[256];
int size = 0;

void read_text(void) {
    
    uint8_t curr;
    int status;

    while (( status = read(0, &curr, sizeof(curr))) == sizeof(buff[size])) {
        if(curr == '\n') {
            break;
        }

        if( size == sizeof(buff) ) {
            break;
        }

        buff[size++] = curr;
    }  

    if ( status < 0 ) {
        err(3, "err while reading from stdin");
    }
}

void print_range(int l, int r) {    
  
    while ( l <= r ) {
        if ( write(1, &buff[l], sizeof(buff[l])) != sizeof(buff[l])) {
            err(4, "err while writing to stdout");
        }

        l++;
    }
    
    uint8_t new_line = '\n';
    if ( write(1, &new_line, sizeof(new_line)) != sizeof(new_line)) {
         err(5, "err while writing to stdout");
    }
}

void c_flag_cmd(char* argv[]) {
    if ( strlen(argv[2]) == 1) {

        int l = atoi(argv[2]) - 1;
        int r = l;
        read_text();
        print_range(l, r);
    }

    else if( strlen(argv[2]) == 3 && *(argv[2] + 1) == '-') {
        
        int l = atoi(argv[2]) - 1;
        int r = atoi(argv[2] + 2) - 1;
        read_text();
        print_range(l, r);
    }

    else {
        err(2, "invalid range formsize");
    }
}

void find_fields(int* l, int* r, uint8_t sep) {

    int i = 0; 
    int cnt = 1;
    int start = 0;
    int end = -1;

    int N = *l;
    while (i < size) {
        if (buff[i] == sep) {
            cnt++;
            if (cnt == N+1) {
                end = i - 1;
                break;
            }
        }

        if (cnt == N && buff[i] == sep) {
            start = i + 1;
        }

        i++;
    }

    if (end < 0) {
        end = size - 1;
    }

    *l = start;
    *r = end;
}

void d_flag_cmd(char* argv[]) {
    
    if ( strcmp(argv[3], "-f") != 0) {
        err(6, "invalid cmd formsize");
    }
   
    if ( strlen(argv[4]) == 1) {
 
        int l = atoi(argv[4]);
        int r = l;
        uint8_t sep = argv[2][0];

        read_text();
        find_fields(&l, &r, sep);
        print_range(l, r);  
    }

    else if( strlen(argv[4]) == 3 && *(argv[4] + 1) == '-') {
        
        int l = atoi(argv[4]);
        int r = atoi(argv[4] + 2);
        uint8_t sep = argv[2][0];
                
        int startL=l, endL=l;
        int startR=r, endR=r;
        
        read_text();
        find_fields(&startL, &endL, sep);
        find_fields(&startR, &endR, sep);
        print_range(startL, endR);
    }

    else {
        err(2, "invalid range formsize");
    }
}

int main(int argc, char* argv[]) {
	
	if ( argc != 3 && argc != 5 ) {
        errx(1, "2 or 4 args expected");
	}

	if ( argc == 3 && strcmp(argv[1], "-c") == 0) {
        c_flag_cmd(argv);
	}

	else if ( argc == 5 && strcmp(argv[1], "-d") == 0) {
        d_flag_cmd(argv);
	}

	return 0;
}
