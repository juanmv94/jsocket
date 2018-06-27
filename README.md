# JSocket
Simple and easy crossplatform C++ classes/libraries for using TCP sockets in Windows (MinGW, VC) and Linux (G++) for both client and server side.

## Getting Started

This library allows you to easily develop portable C++ applications (compilable on both Windows and Linux) that uses TCP sockets using the C++ classes **jsocket** for client side applications, and **jssocket**, **jssocketconn** for server side applications. Both of them have a very easy interface.

## How to compile
* For client side applications with **jsocket** class, you need to add **jsocket.cpp** and **jsocket.h** to your projects.

* For server side applications with **jssocket**, **jssocketconn** classes, you need to add **jssocket.cpp** and **jssocket.h** to your projects.

You can include both libraries if you need both server and client socket functionality in your application without any problem.

Windows users need to add Winsock2 lib to the linker. With MinGW add **-lws2_32** at the final of the link command.

## The jsocket class (clients)

Just create a jsocket object with an URL and a port, and connection will be automatically created!

Example: `jsocket js("example.com", 80);`

Then you could verify connection with **int getconnerror()**.

Here is a list of the methods for a jsocket object:

* **int getconnerror()**: gets an integer with a defined connection error after object creation or reconnect() call. It returns *jsocket\_no\_err*(==0) if connection was sucessful. You can check the full error define list in jsocket.h.

* **void reconnect()**: connects again to the same host and port. Useful if the remote host closed the connection, or just want to disconnect (It does it automatically) and connect again.

* **void disconnect()**: closes the socket connection.

* **int sync\_send(const void\* pointer, int size)**: sends **size** bytes of binary data pointed by **pointer** and returns the number of bytes sent(==-1 if fails, ex: disconnected).

* **int sync\_send\_string(string str)**: sends a std::string and and returns the number of bytes sent(==-1 if fails, ex: disconnected).

* **int sync\_send\_cstring(const char\* pointer)**: sends a char * string finished by 0x00 and and returns the number of bytes sent(==-1 if fails, ex: disconnected).

* **int sync\_rec(const void\* pointer, int maxsize)**: receives a max of **maxsize** bytes from the socket into **pointer**. Returns the number of bytes received(==-1 if fails, ex: disconnected).

## JServerSocket classes (servers)
### jssocket
Just create a jssocket object with a port, and a client queue size, and JServerSocket will bind and start to listen at the port!

Example: `jssocket(80, 0);`

* **int getconnerror()**: gets an integer with a defined connection error after object creation or reconnect() call. It returns *jssocket\_no\_err*(==0) if connection was sucessful. You can check the full error define list in jssocket.h.
* **void reconnect()**: unbinds and binds again to the same port. Useful if there was a bind error creating the *jssocket* object and you dont want to create a new one. 
* **void disconnect()**: unbinds the port
* **jssocketconn\* connect_client()**: this is the only way to get a *jssocketconn*. After jssocket object creation (port bind) without errors (*jssocket\_no\_err*(==0)) You should call this function to get the next client connection.

###jssocketconn
* **void close_conn()**: closes the client connection.

The following are equivalent to the jsocket functions:
 
* **int sync\_send(const void\* pointer, int size)**: sends **size** bytes of binary data pointed by **pointer** and returns the number of bytes sent(==-1 if fails, ex: disconnected).

* **int sync\_send\_string(string str)**: sends a std::string and and returns the number of bytes sent(==-1 if fails, ex: disconnected).

* **int sync\_send\_cstring(const char\* pointer)**: sends a char * string finished by 0x00 and and returns the number of bytes sent(==-1 if fails, ex: disconnected).

* **int sync\_rec(const void\* pointer, int maxsize)**: receives a max of **maxsize** bytes from the socket into **pointer**. Returns the number of bytes received(==-1 if fails, ex: disconnected).

## Some facts

* Destructors **~jsocket()**, **~jssocket()**, and **~jssocketconn()** automatically disconnects the sockets/connections, so you don't have to worry about it.
* Error codes **jsocket\_err\_init** and **jssocket\_err\_init** means an error in Winsock2 initiallization, and you'll never get it in Linux.

## Examples
### Jsocket
You can find an HTTP file downloader in the examples 
folder.

### JServerSocket
You can find an echo server in the examples folder


### Scripts
There are scripts for both compilation on MinGW (bat) and G++ Linux (sh).

## License

I have no idea what license to use. Maybe Creative Commons??

As I don't think this library will become popular (at least by now) try it for yourself and tell me if you are going to use it in a important thing and I would find a license for it.

## Author
@Juanmv94

http://tragicomedy-hellin.blogspot.com