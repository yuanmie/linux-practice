/*
 *dgram.c :将udp建立连接的代码封装
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>

#define HOSTLEN 256
int make_internet_address();

int make_dgram_server_socket(int portnum)
{
     struct sockaddr_in saddr;
     char hostname[HOSTLEN];
     int sock_id;

     //向内核申请一个socket
     sock_id = socket(PF_INET, SOCK_DGRAM, 0);
     if (sock_id == -1)
	  return -1;

     //获取主机名
     gethostname(hostname, HOSTLEN);
     make_internet_address(hostname, portnum, &saddr);

     //绑定地址
     if (bind(sock_id, (struct sockaddr *)&saddr, sizeof(saddr)) != 0)
	  return -1;

     return sock_id;
}

/*
 *返回一个socket
 */
int make_dgram_client_socket()
{
     return socket(PF_INET, SOCK_DGRAM, 0);
}

/*
 *构造一个互联网socket地址，用主机名和端口
 */
int make_internet_address(char *hostname, int port, struct sockaddr_in *addrp)
{
     struct hostent *hp;
     bzero((void*)addrp, sizeof(struct sockaddr_in));
     hp = gethostbyname(hostname);
     if (hp == NULL)
	  return -1;
     bcopy((void *)hp->h_addr, (void *)&addrp->sin_addr, hp->h_length);
     addrp->sin_port = htons(port);
     addrp->sin_family = AF_INET;
     return 0;
}

/*
 *提取主机名和端口从互联网socket地址中
 */
int get_internet_address(char *host, int len, int *portp, struct sockaddr_in *addrp)
{
     strncpy(host, inet_ntoa(addrp->sin_addr), len);
     *portp = ntohs(addrp->sin_port);
     return 0;
}
