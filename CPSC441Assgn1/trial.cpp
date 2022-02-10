#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <iostream>
#include <string.h> // memset
#include <unistd.h> // close
#include <cstdio>


#include <boost/algorithm/string.hpp>
#include <vector>


using namespace std;
#define IPADDRESSSERVER "136.159.5.27"
#define ERROR_URL "pages.cpsc.ucalgary.ca/~carey/CPSC441/ass1/error.html HTTP/1.1\r\nHost: pages.cpsc.ucalgary.ca\r\nConnection: close\r\n\r\n"
#define PORT "1247"
#define PORTCLIENT "80"

//make socket only for keywords
//server socket < waiting for webrowser
//telnet !!
//telnet(IP,Port)
//then type: BLOCK spongebob
//changes reflected in webbrowser


int main(int argc, char  *argv[])
{

    //telnet connection
    
    addrinfo ip_tel, *lin; // server side
    memset(&ip_tel, 0 , sizeof(addrinfo));
    ip_tel.ai_family = AF_INET;
    ip_tel.ai_socktype = SOCK_STREAM;
    ip_tel.ai_flags = AI_PASSIVE;
            

    getaddrinfo( NULL, PORT, &ip_tel, &lin);


    int telnet = socket(lin->ai_family, lin->ai_socktype, lin->ai_protocol);

    connect(telnet, lin->ai_addr, lin->ai_addrlen);

    char message[80] = {0};
     string msg;

    

    //client side
    addrinfo client , *greet;
    memset(&client, 0 , sizeof(addrinfo));
    client.ai_family = AF_INET;
    client.ai_socktype = SOCK_STREAM;

    int opt = 1;


    int s, Error , sec_socket, client_socket ;
    addrinfo ipgetter, *res; // server side
    memset(&ipgetter, 0 , sizeof(addrinfo));
    char buffer[1024] = {0};

    ipgetter.ai_family = AF_INET;
    ipgetter.ai_socktype = SOCK_STREAM;
    ipgetter.ai_flags = AI_PASSIVE;
            

    getaddrinfo( NULL, PORT, &ipgetter, &res);

    s = socket(res->ai_family, res->ai_socktype, res->ai_protocol);


    if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}

    if(s < 0){
        perror("Failed to make server socket\n");
        exit(EXIT_FAILURE);
    }


    if(Error = bind(s , res->ai_addr, res->ai_addrlen) ==  -1){
         perror("Failed to bind server socket\n");
         exit(EXIT_FAILURE);
     }

     if( listen(s , 5) < 0){
         perror("Failure in listen()\n");
         exit(EXIT_FAILURE);
     }
     
    //vector <string> blo;
    while(1){
        
  
        sec_socket = accept(s , res->ai_addr, &res->ai_addrlen);
        if(sec_socket < 0){
            perror("Failure to accept\n");
            exit(EXIT_FAILURE);
        }


        if(recv(sec_socket ,&buffer, sizeof(buffer), 0 ) != -1){

            char host[sizeof(buffer)], url[sizeof(buffer)];
            char read[sizeof(buffer)];
            memcpy(read,buffer,sizeof(read));
            char *url_in = strtok(read, "\r\n");
            char *host_in = strtok(NULL, "\r\n");

            sscanf(url_in, "GET http://%s", url);
            sscanf(host_in, "Host: %s", host);

            string h = url;
            
            
             int adrinfo= getaddrinfo( host, PORTCLIENT, &client ,&greet);
            

             client_socket = socket( greet->ai_family , greet->ai_socktype , greet->ai_protocol);
             
             
             if(client_socket < 0){
                 perror("Failed to create client socket\n");
                 exit(EXIT_FAILURE);

             }
             
             int temp = connect(client_socket, greet->ai_addr, greet->ai_addrlen);

             if( temp != 0){
                 perror("Connection error\n");
                 exit(EXIT_FAILURE);
             }
           
             
             
            bool b;

            for(int i = 1; i < argc; i++){

                if(boost::algorithm::contains(h, argv[i])){
                    b = true;
                    break;
                } 
               
                b = false;
            }
            
            
            if(b){

                char buff[] = "GET http://pages.cpsc.ucalgary.ca/~carey/CPSC441/ass1/error.html \r\nHTTP/1.1Host: pages.cpsc.ucalgary.ca\r\nConnection: keep-alive\r\n\r\n";

                send( client_socket ,buff , sizeof(buff), 0);
                char buff2[1024] = {0};
                
                recv(client_socket , &buff2, sizeof(buff2) , 0);
               
               
                send(sec_socket , buff2, sizeof(buff2) , 0);
                
                close(sec_socket);


            }else{
                send(client_socket , buffer, sizeof(buffer), 0 );
            
            
                char buffer2[1024];


                recv(client_socket,&buffer2,1024 , 0);


                send( sec_socket , buffer2, 1024,0);
                close(sec_socket);
            }

            cout  << buffer << endl;
           
             
        }

    }

    close(s);
    close(client_socket);
  

	return 0;
}

