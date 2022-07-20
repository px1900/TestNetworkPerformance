#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#define testCount 10000

int receiveLongLong(int fd, long long *randNum) {
	int64_t result;
	char* data = (char*) (&result);
	int left = sizeof(result);
	int rc;
    do {
        rc = read(fd, data, left);
        if (rc <= 0) { /* instead of ret */
			return -1;
        }
        else {
            data += rc;
            left -= rc;
        }
    }
    while (left > 0);
    *randNum = ntohl(result);
    return 0;
}

int main(int argc, char** argv){
	// Parse opt
	char* ip_address;
	if (argc == 2) {
		ip_address = argv[1];
	} else {
		ip_address = "127.0.0.1";
	}
	printf("send message to server %s\n", ip_address);
	
	// Allocate a 8G memory 
	char* memoryPool = (char*) malloc((long long)8*1024*1024*1024);
	//for(long long i = 0; i < (long long)8*1024*1024*1024; i++) {
	//	memoryPool[i] = 'a' + i%24;
	//}

    struct timeval start, end;
    int nCount = testCount;

    //创建套接字
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    //向服务器（特定的IP和端口）发起请求
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));  //每个字节都用0填充
    serv_addr.sin_family = AF_INET;  //使用IPv4地址
    serv_addr.sin_addr.s_addr = inet_addr(ip_address);  //具体的IP地址
    serv_addr.sin_port = htons(1234);  //端口

	// Once connected, the server starts timer
    int connect_res = connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    if ( connect_res < 0) {
		printf("connect failed \n");
    	return 0;
    }

	long long randNum = 0;
	while(receiveLongLong(sock, &randNum) == 0 ) {
		write(sock, memoryPool+randNum, 8*1024);
	}

	shutdown(sock, 0);
    //关闭套接字
    close(sock);
	printf("Finished\n");
    return 0;
}
