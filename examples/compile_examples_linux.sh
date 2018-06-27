g++ -c -O3 -I../jsocket http_downloader/main.cpp
g++ -c -O3 -I../jsocket ../jsocket/jsocket.cpp
g++ -o http_downloader.out main.o jsocket.o

g++ -c -O3 -I../jssocket echo_server/main.cpp
g++ -c -O3 -I../jssocket ../jssocket/jssocket.cpp
g++ -o echo_server.out main.o jssocket.o

rm *.o
