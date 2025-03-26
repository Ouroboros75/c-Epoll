#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include "socket.h"

int main(int argc, char* argv[]){

    if(argc > 1){
        unsigned char tempBuff[15];
        cout << "is server" << endl;
        int receiving_socket = create_receiving_udp_socket();
        size_t received = recv(receiving_socket, tempBuff, 15, 0);
        //listen(receiving_socket, 1);
        cout << "got " << received << " bytes" << endl;
        cout << tempBuff << endl;

    }
    else{
        struct sockaddr_in addr_t;
        memset(&addr_t, 0, sizeof(addr_t));
        addr_t.sin_family = AF_INET;
        addr_t.sin_port   = htons(8080);
        inet_pton(AF_INET, "127.0.0.1", &(addr_t.sin_addr));

        unsigned char sendBuff[30] = "i saw the writing on the wall";
        cout << "is sender" << endl;
        int sending_socket = create_sending_udp_socket();
        sendto(sending_socket, sendBuff, 30, 0, (sockaddr*) &addr_t, sizeof(addr_t));
    }

    return 0;
}
