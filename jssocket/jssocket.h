#ifndef JSSOCKET_H
#define	JSSOCKET_H

#define MAX_TCP_SIZE 32768

#define jssocket_no_err 0
#define jssocket_err_init -1
//#define jssocket_err_unknown_host -2
#define jssocket_err_open_socket -3
#define jssocket_err_bind_socket -4
#define jssocket_err_listen_socket -5

#include <iostream>
#include <cstring>
#ifdef _WIN32
#include <winsock2.h>
#else
#include <netdb.h>
#include <arpa/inet.h>
//#include <errno.h>
//#include <fcntl.h>
#include <netinet/in.h>
//#include <signal.h>
//#include <sys/socket.h>
#include <unistd.h>
#endif

using namespace std;

class jssocket;
class jssocketconn;

class jssocket {
private:
    static bool started;
    int connerror;
    int queue;
    int mysocket;
    struct sockaddr_in server_addr;
    void init(int port, bool onlylocal);
    void connect_disconnected();
public:
    jssocket(int port,int queue_size, bool onlylocal);
    ~jssocket();
    
    int getconnerror();
    void reconnect();
    void disconnect();
    jssocketconn* connect_client();
};


class jssocketconn
{
friend class jssocket;
private:
    struct sockaddr_in client_addr;
    int connection;
    jssocketconn(int mysocket);
public:
    ~jssocketconn();
    void close_conn();
    int sync_send(const void* pointer, int size);
    int sync_send_string(string str);
    int sync_send_cstring(const char* pointer);
    int sync_rec(const void* pointer, int maxsize);
    string getip();
};

#endif	/* JSSOCKET_H */

