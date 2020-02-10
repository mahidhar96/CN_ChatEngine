#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <error.h>

#define BUF 1024
#define MAX 1000 //maximum no of groups

typedef struct buffe
{
	int option;
	char name[25];
}buffer;

int main()
{
	int port,n,i;
	char crname[MAX][25];//names only 25 characters
	int crport[MAX],numport=0,pres=3000;//number of ports=0//crport stores port numbers 3000 series
	char ip[20];
	printf("Enter IP address: ");
	scanf("%s",ip);
	strcpy(ip,"127.0.0.1");//for testing
	printf("Enter port number to start server(above 4000): ");
	scanf("%d",&port);
	int s = socket(AF_INET,SOCK_DGRAM,0);//IPPROTO_UDP=0
	if(s==-1)
	{
		perror("socket: ");
		return 1;
	}
	struct sockaddr_in server,client;
	server.sin_family=AF_INET;
	server.sin_port=htons(port);
	server.sin_addr.s_addr=inet_addr(ip);//INADDR_ANY receives from all interfaces icluding local
	int len = sizeof(struct sockaddr_in);
	int result=bind(s,(struct sockaddr*)&server,sizeof(server));
	if(result==-1)
	{
		perror("bind: ");
		return 0;
	}
	setbuf(stdout, NULL);//buffer is set to zero because for some reason printf doesnt work
	printf("server started port: %d\n",port);
	buffer buf;
	int flag=1;
	n=recvfrom(s,(struct buffe*)&buf,sizeof(buffer),0,(struct sockaddr*)&client,&len);
	char txtport[20];
	int grport;
	while(n!=-1 && flag)
	{
		printf("%d %s\n",buf.option,buf.name);
		switch(buf.option)
		{
			case 1:
			
				//start a chatbox,name in buf
				if(pres>=4000 || flag==1)
				{
					for(i=0;i<1000;i++)
					{
						if(crport[i]==-1)
						{
							pres=i;
							flag=1;
							break;
						}
					}
				}
				
			sprintf(txtport,"%d",pres);
			crport[numport]=pres;
			strcpy(crname[numport],buf.name);
			pres++;
			numport++;
			pid_t pid = fork();
        	if (pid == -1)
        	{
            // When fork() returns -1, an error happened.
            perror("Fork failed");
	        }
    	    else if (pid == 0)
    	    {
            	// When fork() returns 0, we are in the child process.
	            execl("croom","croom",txtport,ip,NULL);
	            exit(1);
        	}
	        else
	        {
            	// When fork() returns a positive number, we are in the parent process
            	// and the return value is the PID of the newly created child process.
            	grport=pres-1;
            	n = sendto(s,(int*)&grport,sizeof(int),0,(struct sockaddr*)&client,len);
            	printf("-----%d-----\n",grport);
            	if (n == -1)
            	{
                	perror("Could not send to client");
            	}
        	}
        	
        	case 2:
        	//search for chatboxes
        	grport=0;
        	for(i=0;i<numport;i++)
        	{
        		if(strcmp(crname[i],buf.name)==0)
        		{
        			grport = crport[i];
        			printf("group %d\n",grport);
        			break;
        		}
        		
        	}
        	sendto(s,(int*)&grport,sizeof(int),0,(struct sockaddr*)&client,len);
			break;
		
			default :
				flag=0;
				break;
				//exit
		}
		bzero(&buf,sizeof(buffer));// setting buffer to zero
		n=recvfrom(s,(struct buffe*)&buf,sizeof(buffer),0,(struct sockaddr*)&client,&len);
	}
	close(s);
	return 0;
}
