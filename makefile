#
##
#
G= g++
CFLAGS = -I/usr/local/lib/Oracle/instantclient_11_2/sdk/include
LFLAGS = -L/usr/local/lib/Oracle/instantclient_11_2 -locci -lociei
all: main

main.o: main.cpp
	$(G)-4.9 -c $(CFLAGS) main.cpp

main: main.o
	$(G)-4.9 $(LFLAGS) -o main main.o

clean:
	rm -f main.o main
