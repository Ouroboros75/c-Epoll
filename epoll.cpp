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
#define MAX_SOCKS           10

#ifndef print
#define print(x) cout<<x<<endl
#endif

int main(int argc, char* argv[]){

    struct epoll_event epoll_event_monitor_buff[MAX_SOCKS], epoll_event_report_buff[MAX_EPOLL_BUFF_LEN];
    unsigned int connected_socket_fds[MAX_SOCKS], readies, listening_socket, is_listening = 0;
    int epoll_instance = epoll_create1(0);
    socklen_t len = sizeof(is_listening);
    std::string rx_buffer;

    listening_socket = create_listening_socket();
    listen(listening_socket, 1);

    for(int i=0; i<MAX_SOCKS; i++){
        connected_socket_fds[i] = accept_handler(epoll_event_monitor_buff[i].data.fd);
        if(connected_socket_fds[i] == -1) return -1;
        print(connected_socket_fds[i]);

        epoll_event_monitor_buff[i].data.fd = connected_socket_fds[i];
        epoll_event_monitor_buff[i].events  = EPOLLIN;
        epoll_ctl(epoll_instance, EPOLL_CTL_ADD, connected_socket_fds[i], &epoll_event_report_buff[i]);
    }

    print("DBG: ready for looping");
    while(1){
        readies = epoll_wait(epoll_instance, epoll_event_report_buff, MAX_EPOLL_BUFF_LEN, 1000);
        if(readies<0){
            print("ERR: epoll_wait: ");
            print(strerror(errno));
            return -1;
        }
        sleep(1);
        for(int i=0; i<readies; i++){
            //print(epoll_event_report_buff[i].data.fd);
            rx_buffer.clear();
            recv(epoll_event_report_buff[i].data.fd, rx_buffer.data(), MAX_MSG_LEN, 0);
            print(rx_buffer.data());
        }
    }

    
    
    return 0;
}
