#ifndef JSOCKET_H
#define	JSOCKET_H

#define MAX_TCP_SIZE 32768

#define jsocket_no_err 0
#define jsocket_err_init -1
#define jsocket_err_unknown_host -2
#define jsocket_err_open_socket -3
#define jsocket_err_connect_socket -4

#include <iostream>
#include <cstring>
#ifdef _WIN32
#include <winsock2.h>
#else
#include <netdb.h>
//#include <arpa/inet.h>
//#include <errno.h>
//#include <fcntl.h>
//#include <netinet/in.h>
//#include <signal.h>
//#include <sys/socket.h>
#include <unistd.h>
#endif

using namespace std;

class jsocket {
private:
    static bool started;
    int connerror;
    int mysocket;
    struct hostent *hostinfo;
    struct sockaddr_in server_addr;
    void init(const char *host, int port);
    void connect_disconnected();
public:
    jsocket(const char *host, int port);
    jsocket(string host, int port);
    ~jsocket();
    
    int getconnerror();
    void reconnect();
    void disconnect();
    int sync_send(const void* pointer, int size);
    int sync_send_string(string str);
    int sync_send_cstring(const char* pointer);
    int sync_rec(const void* pointer, int maxsize);
    
};

#endif
