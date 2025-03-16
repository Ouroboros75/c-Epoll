#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include "socket.h"

using std::cout;
using std::endl;

#define MAX_EPOLL_BUFF_LEN  10
#define MAX_MSG_LEN         30

#ifndef print
#define print(x) cout<<x<<endl
#endif

int main(int argc, char* argv[]){

    struct epoll_event epoll_event_instance, epoll_event_buff[MAX_EPOLL_BUFF_LEN];
    //int fd = open("./myfile.txt", O_RDONLY);
    int fd = create_listening_socket();     //blocks until getting a connection
    if(fd == -1) return -1;

    epoll_event_instance.data.fd = fd;
    epoll_event_instance.events  = EPOLLIN;

    int epoll_instance = epoll_create1(0);
    int epoll_ctl_res  = epoll_ctl(epoll_instance, EPOLL_CTL_ADD, fd, &epoll_event_instance);

    //unsigned char rx_buffer[MAX_MSG_LEN];
    std::string rx_buffer;

    while(1){
        int readies = epoll_wait(epoll_instance, epoll_event_buff, MAX_EPOLL_BUFF_LEN, 1000);
        if(readies<0) print("ERR: epoll_wait");
        for(int i=0; i<readies; i++){
           print(epoll_event_buff[i].data.fd);
           rx_buffer.clear();
           recv(epoll_event_buff[i].data.fd, rx_buffer.data(), MAX_MSG_LEN, 0);
           print(rx_buffer.data());
           rx_buffer.clear();
        }
    }

    
    
    return 0;
}
