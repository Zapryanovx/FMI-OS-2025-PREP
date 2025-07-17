#include <sys/stat.h>
#include <unistd.h>
#include <stdint.h>
#include <err.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

typedef struct {
    uint32_t id;
    uint16_t reserved[2];
    uint32_t beg;
    uint32_t end;
} session;

double get_avg_val(const session* sessions, int size) {

    uint32_t res = 0;

    for(int i = 0; i < size; i++) {
        res += sessions[i].end - sessions[i].beg;
    }

    return 1.00 * res / size;
}

double get_disp(const session* sessions, int size) {

    double avg = get_avg_val(sessions, size);
    double sum_sq = 0.0;

    for (int i = 0; i < size; i++) {
        double dur = (double)(sessions[i].end - sessions[i].beg);
        double diff = dur - avg;
        sum_sq += diff * diff;
    }
    return sum_sq / size;
}

int get_users_size(int fd) {

    struct stat st;
    if ( fstat(fd, &st) < 0 ) {
        err(3, "err while fstating");
    }

    if ( st.st_size % sizeof(session) != 0 ) {
        err(4, "invalid file");
    }

    return st.st_size / sizeof(session);
}

void read_sessions(int fd, session* sessions, int size) {

    for(int i = 0; i < size; i++) {
        if(read(fd, &sessions[i], sizeof(sessions[i])) != sizeof(sessions[i])) {
            err(5, "err while reading users");
        }
    }

    if ( lseek(fd, 0, SEEK_SET) < 0 ) {
        err(6, "err while lseeking");
    }
}

bool contains(uint32_t id, uint32_t users[][2], int size) {
    for (int i = 0; i < size; i++) {
        if (users[i][0] == id) {
            return true;
        }
    }
    return false;
}

void update(uint32_t id, uint32_t users[][2], int size, uint32_t dur) {
    for (int i = 0; i < size; i++) {
        if (users[i][0] == id) {
            if (users[i][1] < dur) {
                users[i][1] = dur;
            }
            break;
        }
    }
}

int find_users(int fd, uint32_t users[][2]) {

    session s;
    int status;
    int size = 0;

    while((status = read(fd, &s, sizeof(s))) == sizeof(s)) {

        uint64_t dur = s.end - s.beg;
        if(contains(s.id, users, size)) {
            update(s.id, users, size, dur);
        }

        else {
            users[size][0] = s.id;
            users[size][1] = dur;
            size++;
        }
    }

    if ( status < 0 ) {
        err(7, "err while reading");
    }

    return size;
}

void filter_users(uint32_t users[][2], int users_size, double disp) {
    for (int i = 0; i < users_size; i++) {
        double dur = (double)users[i][1];
        if (dur * dur > disp) {
            char buf[50];
            snprintf(buf, 50, "%u %u\n", users[i][0], users[i][1]);
            write(1, buf, strlen(buf));
        }
    }
}

int main(int argc, char* argv[]) {

        if ( argc != 2 ) {
        errx(1, "1 file expected");
        }

        int fd = open(argv[1], O_RDONLY);
        if ( fd < 0 ) {
        err(2, "err while opening %s", argv[1]);
        }

        int size = get_users_size(fd);
    session sessions[16384];
    read_sessions(fd, sessions, size);

    double disp = get_disp(sessions, size);

    uint32_t users[2048][2];
    int users_size = find_users(fd, users);

    filter_users(users, users_size, disp);



        return 0;
}

