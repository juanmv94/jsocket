#include "jssocket.h"
using namespace std;

int main(int argc, char** argv) {

    char buffer[MAX_TCP_SIZE];
    jssocket jss(1234,0);				 //Puerto 1234, sin cola de clientes esperando ser atendidos (tamaño 0)
	switch (jss.getconnerror())          //comprobamos si ha habido algun error al conectar
    {
        case jssocket_no_err:
            cout<<"Conexion correcta\n\n";
            break;
        case jssocket_err_init:
            cout<<"Error al inicializar WinSock\n";
            return 0;
        case jssocket_err_open_socket:
            cout<<"Error al abrir socket\n";
            return 0;
        case jssocket_err_bind_socket:
            cout<<"Error al realizar el bind al puerto. Tal vez se este usando?\n";
            return 0;
		case jssocket_err_listen_socket:
            cout<<"Error al intentar escuchar el socket creado\n";
            return 0;
        default:
            cout<<"error desconocido\n";
            return 0;
    }
    while(1)
    {
        jssocketconn* c=jss.connect_client();
        cout<<"conectado\n";
        int r;
        while((r=c->sync_rec(buffer,MAX_TCP_SIZE))>0)
            c->sync_send(buffer,r);
        delete c;
        cout<<"desconectado\n";
    }
    return 0;
}

