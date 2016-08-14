#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>

void error(const char *errortxt)
{
  perror(errortxt);
  exit(1);
}

int main()
{
  int socketfd;
  char Recieved_Text[1000];
  int client;

  int hashcount = 0;
  int lettercount = 0;
  int x = 0;

  char msg[1000];

  int portnumber;
  printf("\n please input a port number you want to use \n");
  scanf("%d", &portnumber);                                       //ask user for port number

  if( (socketfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)           //initialize the socker
    error("SOCKET CREATION ERROR");

  struct sockaddr_in address;                                    //bind the socket
  bzero(&address, sizeof(struct sockaddr_in));
  address.sin_port = htons(portnumber);
  address.sin_family = PF_INET;
  address.sin_addr.s_addr = htonl(INADDR_ANY);
  if(bind(socketfd, (struct sockaddr*) &address, sizeof(address)) < 0)
    error("BINDING ERROR");

  if(listen(socketfd, 5) < 0)                                    //start listening
    error("LISTENING ERROR");

  if((client = accept(socketfd, 0, 0)) < 0)                      //accept clients
    error( "CLIENT ACCEPTING ERROR" ); 

  

  while(-1)
    {

      bzero(Recieved_Text, 1000);
      if(read(client, Recieved_Text, 1000) < 0)                //recieve the signal
	error("READ ERROR");

      if(strcmp(Recieved_Text,"thesystemhasclosedexityourloop1923") == 0)   //if the recieved text is the termination text, close the sockets and exit the program. 
	{
	  close(socketfd);
	  close(client);
	  return 0;
	}

      printf(" \n %s \n", Recieved_Text);                     //Print the text
  
      hashcount = 0;                                          //count all of the non-whitespace letters in the recieved text
      lettercount = 0;
      x = 0;

      while(hashcount != 2)
	{
	  if(Recieved_Text[x] == ' ')
	    {
	      hashcount = 0;
	    }
	  else if(Recieved_Text[x] == '#')
	    {
	      hashcount++;
	    }
	  else
	    {
	      hashcount = 0;
	      lettercount++;
	    }

	  x++;
	}
      sprintf(msg, "%d", lettercount);
      write(client, msg, sizeof(msg));                       //write the resulting value back to the client
    }



}
