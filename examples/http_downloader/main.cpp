#include <cstdlib>
#include <iostream>
#include <cstring>
#include <fstream>
#include "jsocket.h"
using namespace std;

int main(int argc, char** argv) {

    char buffer[MAX_TCP_SIZE];
    int bytes_obtenidos;
    
    /////////////////////////
    //Comprobamos argumentos pasados al programa
    ///////////////////////
    if (argc<3) 
    {
        cout<<"uso: "<<argv[0]<<" url archivo\nEj:\n"<<argv[0]<<" example.com ex.html\nSintaxis URL:\nexample.com\nexample.com/index.htm\nhttp://example.com/index.htm\n";
        return 0;
    }
    
    /////////////////////////
    //separamos elementos de la url:
    ///////////////////////
    string urltotal=string(argv[1]);
    string urlsinprotocolo;
    string urldominio;
    string urlruta;
    int posfind;
    if ((posfind=urltotal.find("://"))==string::npos)
        urlsinprotocolo=urltotal;
    else
        urlsinprotocolo=urltotal.substr(posfind+3,string::npos);

    
    if ((posfind=urlsinprotocolo.find("/"))==string::npos)
    {
        urldominio=urlsinprotocolo;
        urlruta="/";
    }
    else
    {
        urldominio=urlsinprotocolo.substr(0,posfind);
        urlruta=urlsinprotocolo.substr(posfind,string::npos);
    }
    
    cout<<"Procesando solicitud HTTP para host: "<<urldominio<<" ruta: "<<urlruta<<"\n\n";
    
    /////////////////////////////////
    //Preparamos comando HTTP GET
    ///////////////////////////////
    string getrequest="GET ";
    getrequest.append(urlruta).append(" HTTP/1.1\r\nHost: ").append(urldominio).append("\r\n\r\n");

    ///////////////////////////////
    //Creamos un jsocket con el servidor
    /////////////////////////////
    jsocket js(urldominio,80);          //conecta al sitio
    switch (js.getconnerror())          //comprobamos si ha habido algun error al conectar
    {
        case jsocket_no_err:
            cout<<"Conexion correcta\n\n";
            break;
        case jsocket_err_init:
            cout<<"Error al inicializar WinSock\n";
            return 0;
        case jsocket_err_unknown_host:
            cout<<"Host desconocido\n";
            return 0;
        case jsocket_err_open_socket:
            cout<<"Error al abrir socket\n";
            return 0;
        case jsocket_err_connect_socket:
            cout<<"Error al conectar con el host\n";
            return 0;
        default:
            cout<<"error desconocido\n";
            return 0;
    }
    
    ///////////////////////////////////
    //Mandamos comando HTTP GET y procesamos respuesta
    /////////////////////////////////
    js.sync_send_string(getrequest);                    //envia comando HTTP
    bytes_obtenidos=js.sync_rec(buffer,MAX_TCP_SIZE);   //recibimos primer mensaje TCP con las cabeceras
    if (bytes_obtenidos==0)
    {
        cout<<"Respuesta del servidor vacia! abortando...\n";
        return 0;
    }
    else if (bytes_obtenidos<0)        //error en la conexion
    {
        cout<<"Error de conexion.\n";
        return 0;
    }
    
    //Obtenemos strings con las cabeceras
    string cabeceras,cabeceras_restantes,cabecera_actual;
    bool encontrado_fin_cabecera=false;
    int content_length=-1;
    cabeceras.assign(buffer,bytes_obtenidos);
    cabeceras_restantes=cabeceras;
    while ((posfind=cabeceras_restantes.find("\r\n"))!=string::npos)
    {
        if (posfind==0)         //dos saltos de linea consecutivos: fin cabecera
        {
            encontrado_fin_cabecera=true;
            break;
        }
        cabecera_actual=cabeceras_restantes.substr(0,posfind);
        cabeceras_restantes=cabeceras_restantes.substr(posfind+strlen("\r\n"),string::npos);

        ////A partir de aqui procesamos cada una de las cabeceras
        cout<<"Cabecera obtenida: "<<cabecera_actual<<"\n";
        //Buscamos cabecera "Content-Length: " 
        if (content_length<0)   //Si aun no hemos encontrado un valor para Content-Length
            if (cabecera_actual.substr(0,strlen("Content-Length: "))=="Content-Length: ")
                content_length=atoi(cabecera_actual.substr(strlen("Content-Length: "),string::npos).c_str());
    }
    cout<<"\nContent lenght obtenido: "<<content_length<<"\n\n";
    if (!encontrado_fin_cabecera)
    {
        cout<<"Error: No se ha encontrado final de la cabecera\n";
        return 0;
    }
    
    ///////////////////////////////////
    //Empezamos a escribir el contenido en el archivo
    /////////////////////////////////
    ofstream archivo_salida(argv[2],ios::binary | ios::trunc);
    if (!archivo_salida.is_open())
    {
        cout<<"Error al abrir archivo\n";
        return 0;
    }
    
    //Grabamos en el archivo el contenido en el buffer tras las cabeceras
    int punterobuffer=cabeceras.find("\r\n\r\n")+strlen("\r\n\r\n");
    int bytesrecibidos=bytes_obtenidos-punterobuffer;
    archivo_salida.write(buffer+punterobuffer,bytesrecibidos);
    
    //Descargamos el resto del archivo
    while ((content_length<0) || (bytesrecibidos<content_length))
    {
        bytes_obtenidos=js.sync_rec(buffer,MAX_TCP_SIZE);
        if (bytes_obtenidos<=0)
        {
            cout<<"Transmision cortada\n";
            break;
        }
        archivo_salida.write(buffer,bytes_obtenidos);
        bytesrecibidos+=bytes_obtenidos;
        cout<<"\nbytes recibidos: "<<bytesrecibidos;
        if (content_length>=0)
            cout<<"/"<<content_length<<" "<<(bytesrecibidos*100/content_length)<<"%";
    }
    js.disconnect();
    archivo_salida.close();
    cout<<"\nDescarga finalizada\n";    
    return 0;
}
