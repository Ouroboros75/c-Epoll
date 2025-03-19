#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <iostream>
#include <fcntl.h>
#include <vector>
#include "socket.h"
#include "clients.h"

#define MAX_EPOLL_BUFF_LEN  10
#define MAX_SOCKS           10
#define MAX_MESSAGE_RW      64

int main(int argc, char* argv[]){

    vector<epoll_event> epoll_event_monitor_buff, epoll_event_report_buff;
    unsigned int readies, listening_socket;
    //socklen_t len = sizeof(is_listening); unsigned int connecting_sock_fd[MAX_SOCKS], is_listening = 0;
    int epoll_instance = epoll_create1(0), fd_holder;
    string rx_buffer;
    clients clients_instance;   //currently empty constructor

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
        readies = epoll_wait(epoll_instance, epoll_event_report_buff.data(), MAX_EPOLL_BUFF_LEN, -1);
        if(readies<0){
            print("ERR: epoll_wait: ");
            print(strerror(errno));
            return -1;
        }
        for(int i=0; i<readies; i++){
            fd_holder = (epoll_event_report_buff.data() + i)->data.fd;
            if(fd_holder == listening_socket){
                unsigned int accepted_sock = accept_handler(listening_socket);
                epoll_ctl(epoll_instance, 
                        EPOLL_CTL_ADD, 
                        accepted_sock, 
                        &(epoll_event_monitor_buff.emplace_back(
                                [accepted_sock](){epoll_event x; x.data.fd=accepted_sock, x.events=EPOLLIN; return x;}()))
                        );
                epoll_event_report_buff.emplace_back();     //sole purpose is add another empty element to the vector for reporting
                clients_instance.insert(accepted_sock);
            }
            else{
                while(read(fd_holder, clients_instance.get_buffer(fd_holder), MAX_MESSAGE_RW) != 0);
                //use a switch on read()'s result
                clients_instance.rename(fd_holder);
                cout<<"DBG: connected fd "<<fd_holder<<" for client "<<clients_instance.get_name(fd_holder)<<endl;
            }
        }
    }

    return 0;
}
