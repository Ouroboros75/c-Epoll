#include "socket.h"

class clients{
    public:
        clients(){};
        void insert(int fd);
        void rename(int fd);
        void parse_messages(int fd);
        string get_name(int fd);
        char* get_buffer(int fd);

    private:
        struct name_msg{
            string name;
            char message_buffer[1024];
            char remain_buffer[1024]; //holds non-whole packets
            unsigned short short1byte = 0;
        };
        map<int, name_msg> clientelle;
};
