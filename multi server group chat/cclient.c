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
#include<stdlib.h>

# define BUF 2048

typedef struct buffe
{
	int option;
	char name[25];
}buffer;

typedef struct buffed
{
	char name[25];
	char message[BUF];
	int option;
}buffer2;

int main()
{
	setbuf(stdout, NULL);
	struct sockaddr_in server;
	char hostip[15];
	//struct hostent *host;//stores host names aliases addrtype etc
	int len_data,sock_desc,port;
	printf("Enter host ip: ");
	scanf("%s",hostip);
	printf("Enter port: ");
	scanf("%d",&port);
	sock_desc=socket(AF_INET,SOCK_DGRAM,0);
	if(sock_desc==-1)
	{
		perror("socket: ");
		return 0;
	}
	memset((char *) &server, 0, sizeof(struct sockaddr_in));//similar to bzero
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = inet_addr(hostip);
	//server.sin_addr.s_addr = inet_addr("127.0.0.1");
	buffer buf;
	int charsize,groupport;
	int choice=0;
	char cboxname[30];
	int length = sizeof(server);
	int exit=0;
	while(1)
	{
		printf("1. New chat\n2.Join group\n3.Exit\n");
		scanf("%d",&choice);
		if(choice==3)
			break;
		printf("group name: ");
		scanf("%s",cboxname);
		buf.option = choice;
		strcpy(buf.name,cboxname);
		switch(choice)
		{
			case 1://start a new session option 1 and name is group name
				
				if(sendto(sock_desc,(struct buffe*)&buf,sizeof(buf),0,(struct sockaddr*)&server,sizeof(server))==-1)
				{
					perror("sendto: ");
					printf("Error connection\nchoice: %d\nname: %s",buf.option,buf.name);
					return 0;
				}
				charsize = recvfrom(sock_desc,(int *)&groupport,sizeof(int),0,(struct sockaddr*)&server,&length);
				printf("-----%d-----\n",groupport);
				if(groupport < 3000)
				printf("couldn't create group\n");
				else
				printf("new group created port:%d\n",groupport);
				break;
				//groupport has the port of the group with which the user is chatting
				//groupchat(port) //function which chats witht the chat box
			
			case 2://join a group option 2 name is search name
				
				if(sendto(sock_desc,(struct buffe*)&buf,sizeof(buf),0,(struct sockaddr*)&server,sizeof(server))==-1)
				{
					printf("Error connection\n");
					return 0;
				}
				charsize = recvfrom(sock_desc,(int *)&groupport,sizeof(int),0,(struct sockaddr*)&server,&length);
				printf("---%d---\n",groupport);
				if(groupport < 3000)
				printf("group not found\n");
				else
				printf("group joined\n");
				//groupport has the port of the joined group
				//groupchat(port) //function which chats with the chatbox
				break;
			default:
			exit=1;
			break;//exit
		}
		if(exit) break;
		
		
		
		
		//join server and begin chatting
		int sockfd,len;
		struct sockaddr_in address;
		int result;
		char name[25];
	
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
		address.sin_port=htons(groupport);
		//address.sin_addr.s_addr=inet_addr("127.0.0.1");
		address.sin_addr.s_addr=inet_addr(hostip);
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
				message.option=120;
				printf("---%s %d %s---\n",message.message,message.option,message.name);
				send(sockfd,(struct buffed*)&message,sizeof(buffer2),0);
				printf("---%s %d %s---\n",message.message,message.option,message.name);
			}
			else if(cchoice==2)
			{
				strcpy(message.name,name);
				message.option=0;
				strcpy(message.message,"(}");
				send(sockfd,(struct buffed*)&message,sizeof(buffer2),0);
				recv(sockfd,(struct buffed*)&message,sizeof(buffer2),0);
				printf("%s",message.message);
				nmsg=atoi(message.name);
				for(;nmsg>0;)
				{
					recv(sockfd,(struct buffed*)&message,sizeof(buffer2),0);
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
	
	//chat ends here so client can exit
	}
	close(sock_desc);
	return 0;
}
