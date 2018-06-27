#include "jsocket.h"

using namespace std;

bool jsocket::started=false;

jsocket::jsocket(const char* host, int port) {
    init(host,port);
}

jsocket::jsocket(string host, int port) {
    init(host.c_str(),port);
}

jsocket::~jsocket()
{
    disconnect();
}

int jsocket::getconnerror()
{
   return connerror; 
}

void jsocket::init(const char* host, int port)
{    
    if (!started)
    {
        #ifdef WIN32
        WSADATA wsadata;
        if (WSAStartup(MAKEWORD(1,1), &wsadata) == SOCKET_ERROR)  // Initialize Winsock version 1.1
        {
            connerror=jsocket_err_init;
            return;
        }
        #endif
        started=true;
    }

    hostinfo = gethostbyname (host);
    if (hostinfo == NULL) 
    {
        connerror=jsocket_err_unknown_host;
        return;
    }
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons (port);
    server_addr.sin_addr = *(struct in_addr *) hostinfo->h_addr;
    
    connect_disconnected();
}

void jsocket::connect_disconnected()
{
    if ((mysocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        connerror= jsocket_err_open_socket;
    }
    else if (connect(mysocket, (struct sockaddr *) &server_addr, sizeof (server_addr)) < 0)
    {
        disconnect();
        connerror=jsocket_err_connect_socket;
    }
    else
    {
        connerror=jsocket_no_err;
    }
}

void jsocket::reconnect()
{
    disconnect();
    connect_disconnected();
}

void jsocket::disconnect()
{
    #ifdef WIN32
    shutdown(mysocket, SD_BOTH);        //we are not capturing errors
    closesocket(mysocket);
    #else
    close(mysocket);
    #endif
}

int jsocket::sync_send(const void* pointer, int size)
{
    #ifdef WIN32
    return send(mysocket, (char*)pointer, size, 0);
    #else
    return write(mysocket, (char*)pointer, size);
    #endif
}

int jsocket::sync_send_string(string str)
{
    return sync_send(str.c_str(),str.length());
}

int jsocket::sync_send_cstring(const char* pointer)
{
    return sync_send(pointer,strlen(pointer));
}

int jsocket::sync_rec(const void *pointer, int maxsize)
{
    #ifdef WIN32
    return recv(mysocket, (char*)pointer, maxsize, 0);
    #else
    return read(mysocket, (char*)pointer, maxsize);
    #endif
}
