#include "socket.h"
#include "clients.h"
#include <string>

void clients::insert(int fd){
    clientelle.insert({fd, {"", {}}});
}

void clients::rename(int fd){
    clientelle.at(fd).name = clientelle.at(fd).message_buffer;
}

string clients::get_name(int fd){
    return clientelle.at(fd).name;
}

char* clients::get_buffer(int fd){
    return clientelle.at(fd).message_buffer;
}

void clients::parse_messages(int fd){
                                             //TEMP for test
    unsigned short length = 0, position = 0, messageNumber = 0;
    char msg_string[1025];   //TEMP for test, with endl
    char* short_pointer = clientelle.at(fd).message_buffer;
    while(position != 1024){
        if(1024 - position < 2)
        length += (short_pointer[position]<<8) + short_pointer[position+1];
        if(position + 2 + length <= 1024){
            memcpy(msg_string, &(short_pointer[position+2]), length);
            msg_string[length] = 10; //endl
            cout << "RX message number " << messageNumber++ << ": " << msg_string << endl;
            position += 2 + length;
        }
        else{
            memcpy(clientelle.at(fd).remain_buffer, short_pointer[]
        }
    }
}

