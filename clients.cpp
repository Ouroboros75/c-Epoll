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

