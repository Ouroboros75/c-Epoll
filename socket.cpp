#include "socket.h"
#include <cerrno>
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

int accept_handler(int socket_fd){
    struct sockaddr_in incoming_addr_t;
    int is_listening = 0;
    socklen_t len_incoming_addr_t = sizeof(incoming_addr_t);
    socklen_t len = sizeof(is_listening);

    if (getsockopt(socket_fd, SOL_SOCKET, SO_ACCEPTCONN, &is_listening, &len) == -1) {
        cout<<"ERR: getsockopt: ";
        print(strerror(errno));
        return -1;
    }
    cout<<"DBG: start accept blocking"<<endl;
    return(accept(socket_fd, (struct sockaddr*) &incoming_addr_t, &len_incoming_addr_t));
}


int create_listening_socket(){
    struct sockaddr_in addr_t, incoming_addr_t;
    const int enable = 1;
    int sock_fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0); 
    memset(&addr_t, 0, sizeof(addr_t));
    memset(&incoming_addr_t, 0, sizeof(incoming_addr_t));
    addr_t.sin_family = AF_INET;
    addr_t.sin_port   = htons(8080);

    inet_pton(AF_INET, "127.0.0.1", &(addr_t.sin_addr));

    if(setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0){
        cout<<"ERR: setsockopt: ";
        print(strerror(errno));
        return -1;
    } 
    if(bind(sock_fd, (struct sockaddr*) &addr_t, sizeof(addr_t)) == -1){
        cout<<"ERR: sock bind: ";
        print(strerror(errno));
        return -1;
    }
    listen(sock_fd, 1);
    return sock_fd;

    /*
    //int flags = fcntl(accepting_fd, F_GETFL, 0);
    //fcntl(accepting_fd, F_SETFL, flags | O_NONBLOCK);
    */
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
