#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include<stdio.h>
#include <unistd.h>
#include<string.h>
#include <sys/types.h> 
#include <sys/stat.h>
#include<unistd.h>
#include<fcntl.h>

#define BUF 1024

typedef struct buffed
{
	char name[25];
	char message[BUF];
	int option;
}buffer2;

int main()
{
	int sockfd,len;
	struct sockaddr_in address;
	int result;
	char name[25];
	printf("Enter ip and port:\n");
	char ip[20];
	int port;
	scanf("%s",ip);
	scanf("%d",&port);
	setbuf(stdout, NULL);
	printf("Enter chatname(no spaces): ");
	
	buffer2 message;
	scanf("%s",name);
	strcpy(message.name,name);
	printf("----%s-----",message.name);
	
	//int fd = open(name,O_CREAT|O_RDWR);
	char msg[100],atta[3],nl[2];
	strcpy(atta,": ");
	nl[0]='\n';nl[1]='\0';
	sockfd = socket(AF_INET,SOCK_STREAM,0);
	address.sin_family=AF_INET;
	address.sin_port=htons(port);
	address.sin_addr.s_addr=inet_addr(ip);
	len=sizeof(address);
	result=connect(sockfd,(struct sockaddr*)&address,len);
	if(result==-1) perror("connect: ");
	char buf[256];
	
	fd_set rfds;
	FD_ZERO( &rfds);
	FD_SET(0, &rfds);
	FD_SET(result,&rfds);
	
	int cchoice;
	int nmsg=1;
	while(1)
	{
		printf("1-send message 2-unread messages anything else exit: ");
		scanf("%d",&cchoice);
		if(cchoice==1)
		{
		strcpy(message.name,name);
		scanf("%s",message.message);
		message.option=1;
		send(sockfd,(struct buffed*)&message,sizeof(message),0);
		}
		else if(cchoice==2)
		{
			strcpy(message.name,name);
			message.option=0;
			send(sockfd,(struct buffed*)&message,sizeof(message),0);
			recv(sockfd,(struct buffed*)&message,sizeof(message),0);
			printf("%s",message.message);
			nmsg=message.option;
			for(;nmsg>0;)
			{
				recv(sockfd,(struct buffed*)&message,sizeof(message),0);
				if(strcmp(message.name,name)==0)
					strcpy(message.name,"You");
				printf("%s: %s\n",message.name,message.message);
				nmsg--;
			}
			//close(fd);
			//open(name,O_CREAT|O_RDWR);
		}
		else
			break;
		
		
		/*
		recv(sockfd,(struct buffed*)&message,sizeof(message),0);
		strcpy(msg,message.name);
		strcat(msg,atta);
		strcat(msg,message.message);
		strcat(msg,nl);
		write(fd,msg,100);*/
		//printf("%s: %s",message.name,message.message);
	}
	close(sockfd);
	return 0;
}
