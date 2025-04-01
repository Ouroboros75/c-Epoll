#include <unistd.h>
#include <iostream>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <cerrno>
#include <cstring>
#include <vector>
#include <map>
//
#include <chrono>


//move these and other common libs to an utility unit
using std::cout;
using std::endl;
using std::vector;
using std::map;
using std::string;
using namespace std::chrono;

#ifndef print
#define print(x) cout<<x<<endl;
#endif

int create_listening_socket();
int create_connecting_socket();
int accept_handler(int socket_fd);
int create_sending_udp_socket();
int create_receiving_udp_socket();

/*
struct custom_epoll_event{
    struct epoll_event epoll_event_object;
    custom_epoll_event(int fd, int event){
        epoll_event_object.data.fd = fd;
        epoll_event_object.events  = event;
    }
};
*/
