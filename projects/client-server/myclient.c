#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>

void error(const char *errortxt)
{
  perror(errortxt);
  exit(1);
}

int main()
{
  int socketfd;                   
  int portno;
  struct sockaddr_in address;
  struct hostent *server;
  char hostname[16];
  int temp;

  char text[1000];

  printf("\n please input the port number \n");           //Get user input for the port number 
  scanf("%d", &portno);
  getchar();
  printf("\n");
  
  if((socketfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)    //initialize the socket
    error("SOCKET ERROR");

  printf("\n Please input the IPv4 address of the host \n"); //get user input for the address of the host the message will be sent to, then put it in the hostent struct
  scanf("%s", hostname);
  getchar();
  printf("\n");
  server = gethostbyname(hostname);
  
  if(server==NULL)                                         
    error("NO SUCH HOST");

  bzero((char *) &address, sizeof(address));                //set all required values, then connect to the host
  address.sin_family = AF_INET;
  address.sin_port = htons(portno);
  bcopy((char *)server->h_addr, (char *)&address.sin_addr.s_addr, server->h_length);
  if(connect(socketfd, (struct sockaddr *)&address, sizeof(address)) < 0)
    error("CONNECTION ERROR");

  struct ifreq ipaddressget;                               //get the IP address of the current machine
  ipaddressget.ifr_addr.sa_family = AF_INET;
  strncpy(ipaddressget.ifr_name, "eth0", IFNAMSIZ-1);
  ioctl(socketfd, SIOCGIFADDR, &ipaddressget);

  printf(" \n Host %s connected to %s \n ", inet_ntoa(( (struct sockaddr_in *)&ipaddressget.ifr_addr)->sin_addr),hostname); // print the current IP address, along with the address of the host


  char loopbreak;
  while(-1)
    {
      printf(" \n enter the message you want to send, ending with a ## \n");
      bzero(text, 1000);
      fgets(text, 1000, stdin);                           //prompt the user to input the message, and then write it to the host
      if((temp = write(socketfd,text, strlen(text))) < 0)
	error("ERROR, COULD NOT WRITE TO SOCKET");
      bzero(text, 1000);
      if((temp = read(socketfd, text, 1000)) < 0)         //read the hosts response, and then print it
	error("ERROR, COULD NOT READ FROM SOCKET");
      printf("\n The amount of non-whitespace characters in that last message was: %s\n", text);


      printf(" \n input y to send another message. Press anything else to quit \n");  //if the host does not enter y, send the termination string to the host, break out of the loop, close the socket, and exit.     
      scanf("%c", &loopbreak);
      getchar();
      if(loopbreak != 'y')
	{
	  const char *breaktext = "thesystemhasclosedexityourloop1923";
	  if((temp = write(socketfd, breaktext, strlen(breaktext))) < 0)
	    error("COULD NOT WRITE TO SOCKET TO TERMINATE");
	  break;
	}

    }

  close(socketfd);
 
  return 0;

}
