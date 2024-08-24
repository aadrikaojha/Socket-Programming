#include<iostream>
#include <string>
#include<cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fstream>
#include <unistd.h>

int main(){

    std::cout<<"<--------------SERVER------------->";
    std::cout<<"<--------------Step 1------------->";

    int socket_desc, client_sock, client_size;
    struct sockaddr_in server_addr, client_addr;
    std::string server_message{""}, client_message{""};
    
    // // Clean buffers:
    memset(server_message, '\0', server_message.length());
    memset(client_message, '\0', client_message.length());
    
    // Create socket:
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    
    if(socket_desc < 0){
        std::cout<<"Error while creating socket\n";
        return -1;
    }
    std::cout<<"Socket created successfully\n";
    
    // Set port and IP:
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(2000);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    // Bind to the set port and IP:
    if(bind(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr))<0){
        std::cout<<"Couldn't bind to the port\n";
        return -1;
    }
    std::cout<<"Done with binding\n";
    
    // Listen for clients:
    if(listen(socket_desc, 1) < 0){
        std::cout<<"Error while listening\n";
        return -1;
    }
    std::cout<<"\nListening for incoming connections.....\n";
    
    // Accept an incoming connection:
    client_size = sizeof(client_addr);
    client_sock = accept(socket_desc, (struct sockaddr*)&client_addr, &client_size);
    
    if (client_sock < 0){
        std::cout<<"Can't accept\n";
        return -1;
    }
    std::cout<<"Client connected at IP: %s and port: %i\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port);
    
    // Receive client's message:
    if (recv(client_sock, client_message, sizeof(client_message), 0) < 0){
        printf("Couldn't receive\n");
        return -1;
    }
    printf("Msg from client: %s\n", client_message);
    
    // Respond to client:
    strcpy(server_message, "This is the server's message.");
    
    if (send(client_sock, server_message, strlen(server_message), 0) < 0){
        printf("Can't send\n");
        return -1;
    }
    
    // Closing the socket:
    close(client_sock);
    close(socket_desc);



    return 0;
}