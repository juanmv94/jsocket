#include "jssocket.h"

////////////////
//JServerSocket
///////////////

bool jssocket::started=false;

jssocket::jssocket(int port, int queue_size) : queue(queue_size) {
    init(port);
}

jssocket::~jssocket()
{
    disconnect();
}

int jssocket::getconnerror()
{
   return connerror; 
}

void jssocket::init(int port)
{    
    if (!started)
    {
        #ifdef WIN32
        WSADATA wsadata;
        if (WSAStartup(MAKEWORD(1,1), &wsadata) == SOCKET_ERROR)  // Initialize Winsock version 1.1
        {
            connerror=jssocket_err_init;
            return;
        }
        #endif
        started=true;
    }
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons (port);
    //server_addr.sin_addr = htonl(INADDR_ANY);
    
    connect_disconnected();
}

void jssocket::connect_disconnected()
{
    if ((mysocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        connerror= jssocket_err_open_socket;
    }
    else if (bind(mysocket, (struct sockaddr*)&server_addr,sizeof(server_addr)) < 0)
    {
        disconnect();
        connerror=jssocket_err_bind_socket;
    }
    else if ((listen(mysocket, queue)) < 0)
    {
        disconnect();
        connerror=jssocket_err_listen_socket;
    }
    else
    {
        connerror=jssocket_no_err;
    }
}

void jssocket::reconnect()
{
    disconnect();
    connect_disconnected();
}

void jssocket::disconnect()
{
    #ifdef WIN32
    shutdown(mysocket, SD_BOTH);        //we are not capturing errors
    closesocket(mysocket);
    #else
    close(mysocket);
    #endif
}

jssocketconn* jssocket::connect_client()
{
    return new jssocketconn(mysocket);
}

////////////////////////////
//JServerSocket connection
///////////////////////////

jssocketconn::jssocketconn(int mysocket)
{
    #ifdef WIN32
    int client_addr_length = sizeof(client_addr);
    #else
    unsigned int client_addr_length = sizeof(client_addr);
    #endif
    connection=accept(mysocket, (struct sockaddr *)&client_addr, &client_addr_length);
}

jssocketconn::~jssocketconn()
{
    close_conn();
}

void jssocketconn::close_conn()
{
    #ifdef WIN32
    shutdown(connection, SD_BOTH);        //we are not capturing errors
    closesocket(connection);
    #else
    close(connection);
    #endif
}

int jssocketconn::sync_send(const void* pointer, int size)
{
    #ifdef WIN32
    return send(connection, (char*)pointer, size, 0);
    #else
    return write(connection, (char*)pointer, size);
    #endif
}

int jssocketconn::sync_send_string(string str)
{
    return sync_send(str.c_str(),str.length());
}

int jssocketconn::sync_send_cstring(const char* pointer)
{
    return sync_send(pointer,strlen(pointer));
}

int jssocketconn::sync_rec(const void *pointer, int maxsize)
{
    #ifdef WIN32
    return recv(connection, (char*)pointer, maxsize, 0);
    #else
    return read(connection, (char*)pointer, maxsize);
    #endif
}
