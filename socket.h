#include <unistd.h>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <cerrno>
#include <cstring>

using std::cout;
using std::endl;

#ifndef print
#define print(x) cout<<x<<endl;
#endif

int create_listening_socket();
int create_connecting_socket();
