#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

#define PORTNUM 13000
#define HOSTLEN 256
#define oops(msg) {perror(msg); exit(1);}

int main(int argc, char *argv[])
{
     struct sockaddr_in saddr;
     struct hostent *hp;
     char hostname[HOSTLEN];
     int sock_id, sock_fd;
     FILE *sock_fp;
     time_t thetime;

     /*
      *step1:获得socket
      */
     sock_id = socket(PF_INET, SOCK_STREAM, 0);
     if (sock_id == -1)
	  oops("socket");

     /*
      *step2:为socket绑定地址，地址是主机+端口
      */
     bzero((void *)&saddr, sizeof(saddr)); //清空结构
     gethostname(hostname,HOSTLEN); //获得主机信息
     hp = gethostbyname(hostname);

     bcopy((void *)hp->h_addr, (void *)&saddr.sin_addr,
	   hp->h_length);
     saddr.sin_port = htons(PORTNUM); //填充端口
     saddr.sin_family = AF_INET; //填充地址簇

     if (bind(sock_id, (struct sockaddr *)&saddr, sizeof(saddr)) != 0)
	  oops("bind");

     /*
      *step3:允许接入调用
      */
     if (listen(sock_id, 1) != 0)
	  oops("listen");

     /*
      *等待调用，传输数据，关闭
      */
     while(1)
     {
	  sock_fd = accept(sock_id, NULL, NULL);
	  printf("Wow!got a call!\n");
	  if (sock_fd == -1)
	       oops("accept");
	  sock_fp = fdopen(sock_fd, "w");
	  if (sock_fp == NULL)
	       oops("fdopen");
	  thetime = time(NULL);
	  fprintf(sock_fp, "The time here is ..");
	  fprintf(sock_fp, "%s", ctime(&thetime));
	  fclose(sock_fp);
     }
}
