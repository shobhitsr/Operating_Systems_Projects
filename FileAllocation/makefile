CC = g++
CCFLAGS = -Wall -std=c++11

FAT: FAT.o
	$(CC) $(CCFLAGS) -o FAT FAT.o

FAT.o: FAT.cpp
	$(CC) $(CCFLAGS) -c FAT.cpp

clean:
	-rm *.o FAT
