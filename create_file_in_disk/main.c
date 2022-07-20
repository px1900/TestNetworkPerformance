#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <malloc.h>
#include <unistd.h>
#include <fcntl.h>

#define _GNU_SOURCE

int main() {
	int fd = open("/scratch1/pang65/tfile.txt", O_CREAT|O_RDWR);
    char buff [8192];
    for(int i = 0; i < 8192; i++) {
        buff[i] = 'a'+(i%20);
    }

    __off_t _off;
    for(int i = 0; i < 1024*1024; i++) {
        _off = (__off_t)i*8*1024;
        ssize_t _size = pwrite(fd, buff, 8192, _off);
    }
    fsync(fd);
    close(fd);
}
