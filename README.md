# Jsocket
A simple and easy crossplatform C++ class/library for using TCP sockets in Windows (MinGW, VC) and Linux (G++)

## Getting Started

This library allows you to easily develop portable C++ applications (compilable on both Windows and Linux) that uses TCP sockets using the C++ class **jsocket** with a very simple interface.

## How to compile
You need to add **jsocket.cpp** and **jsocket.h** to your projects.

Windows users need to add Winsock2 lib. With MinGW add **-lws2_32** at the final of the link command.

## The jsocket class

Just create a jsocket object with an URL and a port, and connection will be automatically created!
Example: jsocket js("example.com", 80);
Then you could verify connection with **int getconnerror()**.

Here is a list of the methods for a jsocket object:

* **int getconnerror()**: gets an integer with a defined connection error after object creation or reconnect() call. It returns **jsocket_no_err**(==0) if connection was sucessful. You can check the full error define list in jsocket.h.

* **void reconnect()**: connects again to the same host and port. Useful if the remote host closed the connection, or just want to disconnect (It does it automatically) and connect again.

* **void disconnect()**: closes the socket connection.

* **int sync\_send(const void\* pointer, int size)**: sends **size** bytes of binary data pointed by **pointer** and returns the number of bytes sent(==-1 if fails, ex: disconnected).

* **int sync\_send\_string(string str)**: sends a std::string and and returns the number of bytes sent(==-1 if fails, ex: disconnected).

* **int sync\_send\_cstring(const char\* pointer)**: sends a char * string finished by 0x00 and and returns the number of bytes sent(==-1 if fails, ex: disconnected).

* **int sync\_rec(const void\* pointer, int maxsize)**: receives a max of **maxsize** bytes from the socket into **pointer**. Returns the number of bytes received(==-1 if fails, ex: disconnected).

## Some facts

* Destructor **~jsocket()** automatically disconnects the socket, so you don't have to worry about it.
* Error code **jsocket\_err\_init** means an error in Winsock2 initiallization, and you'll never get it in Linux.

## Examples

You can find an HTTP file downloader in the examples folder.
There's also a BAT file for compilation on MinGW.

## Future things(?)

Jsocket implements TCP socket clients. Maybe a Jserversocket class for TCP socket servers would be nice.

## License

I have no idea what license to use. Maybe Creative Commons??

As I don't think this library will become popular (at least by now) try it for yourself and tell me if you are going to use it in a important thing and I would find a license for it.

## Author
@Juanmv94

http://tragicomedy-hellin.blogspot.com