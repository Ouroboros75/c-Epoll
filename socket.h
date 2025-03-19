#include <unistd.h>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <cerrno>
#include <cstring>
#include <vector>
#include <map>

//move these and other common libs to an utility unit
using std::cout;
using std::endl;
using std::vector;
using std::map;
using std::string;

#ifndef print
#define print(x) cout<<x<<endl;
#endif

int create_listening_socket();
int create_connecting_socket();
int accept_handler(int socket_fd);

/*
struct custom_epoll_event{
    struct epoll_event epoll_event_object;
    custom_epoll_event(int fd, int event){
        epoll_event_object.data.fd = fd;
        epoll_event_object.events  = event;
    }
};
*/
