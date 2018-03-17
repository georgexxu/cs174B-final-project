
all: main.exe

main.exe: main.o bpt.o
	g++ -o main.exe main.o bpt.o

main.o: main.cpp
	g++ -c main.cpp

bpt.o: bpt.cc bpt.h
	g++ -c bpt.cc

clean:
	rm -rf *.o
	rm -rf *.exe
