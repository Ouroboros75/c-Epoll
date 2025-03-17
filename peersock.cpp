#include "socket.h"
#include <unistd.h>

#define MAX_INPUT_LEN 30

void safeLineParsing(std::string* user_input_p, int max_len){
    std::getline(std::cin, *user_input_p);
    //if(user_input_p.length() > max_len) user_input_p.substr(0, max_len);
}

int main(int argc, char* argv[]){

    int connecting_sock_fd = create_connecting_socket();
    std::string user_input;
    //ssize_t sent_bytes;
    //unsigned char send_buffer[MAX_INPUT_LEN];
    while(1){
        safeLineParsing(&user_input, MAX_INPUT_LEN);
        //cout<<"-> ";
        //print(user_input);
        send(connecting_sock_fd, user_input.data(), user_input.length(), 0);
        user_input.clear();
    }

    return 0;
}
