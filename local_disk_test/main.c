#define _GNU_SOURCE

#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <malloc.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

int main() {
	long long randNum[100000];
	struct timeval start, end;
    time_t t;
    ssize_t alignment;
    int testCount = 5000;

	// open file to be read
    char *filename = (char*)malloc(1024);
    strcpy(filename, "/scratch1/pang65/tfile.txt\0");
	printf("filename = %s\n", filename);
    int fd = open(filename, O_RDWR|O_DIRECT);
    if (fd == -1)
        printf("open failed \n");
    free(filename);

	// Initialize the random number list
    srand((unsigned) time(&t));
	for(int i = 0; i < testCount; i++) 
		randNum[i] = rand();

	// read/write size for each access
    ssize_t accessSize = 8*1024;
    alignment = 4*1024;

	// allocate an aligned memory for O_DIRECT flag
    char * buff;
    buff = (char*)memalign(alignment * 2, accessSize + alignment);
    if (buff == NULL)
        printf("memalign failed \n");
    buff += alignment;

    int ncount = testCount;

    /******* Only for pwrite ******/
    for(int i = 0; i < accessSize; i++) {
        buff[i] = 'a'+(i%20);
    }
    /*****************************/

    gettimeofday(&start, NULL);
    while(ncount--) {
		// seek range is [0, 7G], in this way, each access will starts 
		//     from the head of a block (size = 4096)
        ssize_t seekpos = randNum[ncount]%((ssize_t)7*1024*1024/4);
    	if (lseek(fd, seekpos*4*1024, SEEK_SET) == -1)
		    printf("lseek error \n");

    	ssize_t readBytes = write(fd, buff, accessSize);
        if(readBytes != accessSize) {
            printf("read failed, result = %ld\n", readBytes);
            break;
        }
    }
    gettimeofday(&end, NULL);
    long seconds = (end.tv_sec - start.tv_sec);
    long micros = ((seconds * 1000000) + end.tv_usec) - (start.tv_usec);
    printf("The elapsed time is %ld seconds and %ld micros\n", seconds/testCount, micros/testCount);

	// if the ncount is -1, then succeed
    printf("ncount = %d\n", ncount);
    fsync(fd);
    close(fd);
}
