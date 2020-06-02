#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>

#define DEFAULT_BUFLEN 256

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    int sockfd, newsockfd, portno;
    socklen_t clilen;
    char buffer[DEFAULT_BUFLEN], res[DEFAULT_BUFLEN];
    struct sockaddr_in serv_addr, cli_addr;
    int n;
    if (argc < 2) {
        fprintf(stderr,"ERROR, no port provided\n");
    exit(1);
    }
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR on binding");
    while(true)
    {
        listen(sockfd,5);
        clilen = sizeof(cli_addr);
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd < 0)
            error("ERROR on accept");
        bzero(buffer,DEFAULT_BUFLEN);
        n=recv(newsockfd,buffer,DEFAULT_BUFLEN, 0);
        if (n < 0) 
            error("ERROR reading from socket");
        printf("Here is the initial text: %s\n",buffer);
        //---------------------------Caesar-------------------------------
        char cipher[DEFAULT_BUFLEN];
        bzero(cipher,DEFAULT_BUFLEN);
        int length  = strlen(buffer);
        for (int i = 0; i < length; ++i)
        {
            cipher[i] = buffer[i] + 3;
            if ( (cipher[i] > 'Z') && (cipher[i] < 'a') )
                cipher[i] = (cipher[i] + 'A') % ('Z'+1);
            else if ((buffer[i] + 3) > 'z')
                cipher[i] = (cipher[i] + 'a') % ('z'+1);
            else
                cipher[i]  = cipher[i] % ('z'+1);
        }
        printf("Here is the cipher: %s\n", cipher); 
        //----------------------------------------------------------------
        n=send(newsockfd, (char *) cipher, strlen(cipher)+1, 0);
        if (n < 0) 
            error("ERROR writing to socket");
        close(newsockfd);
    }
    close(sockfd);
    return 0;
}
