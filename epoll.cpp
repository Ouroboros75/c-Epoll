#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <iostream>
#include <fcntl.h>
#include "socket.h"
#include <vector>

#define MAX_EPOLL_BUFF_LEN  10
#define MAX_MSG_LEN         30
#define MAX_SOCKS           10

#ifndef print
#define print(x) cout<<x<<endl
#endif

int main(int argc, char* argv[]){

    vector<epoll_event> epoll_event_monitor_buff, epoll_event_report_buff;
    unsigned int connected_socket_fds[MAX_SOCKS], readies, listening_socket, is_listening = 0;
    socklen_t len = sizeof(is_listening);
    int epoll_instance = epoll_create1(0);
    std::string rx_buffer;

    listening_socket = create_listening_socket();
    epoll_ctl(epoll_instance, 
            EPOLL_CTL_ADD, 
            listening_socket, 
            &(epoll_event_monitor_buff.emplace_back(
                    [listening_socket](){epoll_event x; x.data.fd=listening_socket, x.events=EPOLLIN; return x;}()))
            );
    epoll_event_report_buff.emplace_back();

    print("DBG: ready for SEGGs");
    while(1){
                                             //SEGGS
        readies = epoll_wait(epoll_instance, epoll_event_report_buff.data(), MAX_EPOLL_BUFF_LEN, -1);
        if(readies<0){
            print("ERR: epoll_wait: ");
            print(strerror(errno));
            return -1;
        }
        sleep(1);
        for(int i=0; i<readies; i++){
            if((epoll_event_report_buff.data() + i)->data.fd == listening_socket){
                unsigned int accepted_sock = accept_handler(listening_socket);
                epoll_ctl(epoll_instance, 
                        EPOLL_CTL_ADD, 
                        accepted_sock, 
                        &(epoll_event_monitor_buff.emplace_back(
                                [accepted_sock](){epoll_event x; x.data.fd=accepted_sock, x.events=EPOLLIN; return x;}()))
                        );
                epoll_event_report_buff.emplace_back();
            }
            else{
                print("DBG: accepted socket is getting in on this");
            }
        }
    }

    return 0;
}
