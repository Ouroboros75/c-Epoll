#include "socket.h"
#include <cerrno>
#include <cstring>
#include <netinet/in.h>

int create_listening_socket(){
    struct sockaddr_in addr_t, incoming_addr_t;
    memset(&addr_t, 0, sizeof(addr_t));
    memset(&incoming_addr_t, 0, sizeof(incoming_addr_t));
    addr_t.sin_family = AF_INET;
    addr_t.sin_port   = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &(addr_t.sin_addr));

    socklen_t len_addr_t, len_incoming_addr_t; 

    int sock_fd = socket(AF_INET, SOCK_STREAM, 0); 
    if(bind(sock_fd, (struct sockaddr*) &addr_t, sizeof(addr_t)) == -1){
        cout<<"ERR";
        print(strerror(errno));
        return -1;
    }
    listen(sock_fd, 1);
    len_incoming_addr_t = sizeof(incoming_addr_t);

    print("DBG: socket is listening");
    int accepting_fd = accept(sock_fd, (struct sockaddr*) &incoming_addr_t, &len_incoming_addr_t);
    if(accepting_fd == -1){
        print("ERR: accept == EINVAL, check if socket is properly bound and in listening state");
        print(EINVAL);
        return -1;
    }

    //int flags = fcntl(accepting_fd, F_GETFL, 0);
    //fcntl(accepting_fd, F_SETFL, flags | O_NONBLOCK);
    print("DBG: succesfully bound to localhost and accepted an incoming connection");
    return accepting_fd;
}


int create_connecting_socket(){
    struct sockaddr_in addr_t;
    memset(&addr_t, 0, sizeof(addr_t));
    addr_t.sin_family = AF_INET;
    addr_t.sin_port   = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &(addr_t.sin_addr));
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0); 
    if(connect(sock_fd, (struct sockaddr*) &addr_t, sizeof(addr_t)) == -1){
        print("ERR: sock_connect");
        return -1;
    }

    print("DBG: succesfully connected to localhost socket");
    return sock_fd;
}
