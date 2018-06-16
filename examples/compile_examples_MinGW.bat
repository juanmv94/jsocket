g++ -c -O3 -I../jsocket http_downloader\main.cpp
g++ -c -O3 -I../jsocket ..\jsocket\jsocket.cpp
g++ -o http_downloader.exe main.o jsocket.o -lws2_32
