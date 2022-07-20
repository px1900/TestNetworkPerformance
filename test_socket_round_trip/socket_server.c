#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <time.h>
#define testCount 10000

int main(int argc, char** argv){
	// Parse opt
	char* ip_address;
	if (argc == 2) {
		ip_address = argv[1];
	} else {
		ip_address = "127.0.0.1";
	}
	printf("socket server listening ip %s\n", ip_address);

	// Initialize random number in advance
    // Each access will send memoryPool [rand : rand+8192-1]
    long long randNum[100000];
    for(int i = 0; i < testCount; i++) {
        randNum[i] = rand() % ( (long long)8*1024*1024*1024 - 8*1024);
    }   

    //创建套接字
    int serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    //将套接字和IP、端口绑定
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));  //每个字节都用0填充
    serv_addr.sin_family = AF_INET;  //使用IPv4地址
    serv_addr.sin_addr.s_addr = inet_addr(ip_address);  //具体的IP地址
    serv_addr.sin_port = htons(1234);  //端口
    bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    //进入监听状态，等待用户发起请求
    listen(serv_sock, 20);
    //接收客户端请求
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size = sizeof(clnt_addr);
	printf("rand Done\n");
    int clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);

	
    struct timeval start, end;
    char buff[8192];

	// Starts timer
	gettimeofday(&start, NULL);

	long long totalBytes = 0;
	
	long long totalReceived = 0;
	int nCount = testCount;
	while(nCount--) {
		int64_t randN = htonl(randNum[nCount]);
		char* data = (char*) &randN;
		write(clnt_sock, data, sizeof(randN));
		
		int readBytesCount;
		int left = 8192;
		do{
			readBytesCount = read(clnt_sock, buff, 8192);
			totalReceived += readBytesCount;
			left -= readBytesCount;
		} while(left>0);

	}
	gettimeofday(&end, NULL);

	printf("nCount = %d, totalReceived = %lld\n", nCount, totalReceived);
    long seconds = (end.tv_sec - start.tv_sec);
    long micros = ((seconds * 1000000) + end.tv_usec) - (start.tv_usec);
    printf("The elapsed time is %ld seconds and %ld micros\n", seconds/testCount, micros/testCount);

	shutdown(clnt_sock, 0);
	shutdown(serv_sock, 0);
    //关闭套接字
    close(clnt_sock);
    close(serv_sock);
	printf("Finished\n");
    return 0;
}
