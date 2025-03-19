#include "socket.h"

class clients{
    public:
        clients(){};
        void insert(int fd);
        void rename(int fd);
        string get_name(int fd);
        char* get_buffer(int fd);

    private:
        struct name_msg{
            string name;
            char message_buffer[1024];
        };
        map<int, name_msg> clientelle;
};
