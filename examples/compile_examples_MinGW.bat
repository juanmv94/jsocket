g++ -c -O3 -I../jsocket http_downloader\main.cpp
g++ -c -O3 -I../jsocket ..\jsocket\jsocket.cpp
g++ -o http_downloader.exe main.o jsocket.o -lws2_32

g++ -c -O3 -I../jssocket echo_server\main.cpp
g++ -c -O3 -I../jssocket ..\jssocket\jssocket.cpp
g++ -o echo_server.exe main.o jssocket.o -lws2_32

del *.o
