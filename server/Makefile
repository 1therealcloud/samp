CC = gcc
CXX = g++

AMX = ../third-party/amx/source/amx
AMX_LINUX = ../third-party/amx/source/linux
RAKNET = ../third-party/RakNet
SQLITE = ../third-party/sqlite

INCLUDES = -I$(AMX) -I$(AMX_LINUX) -I$(RAKNET)/Include -I$(SQLITE)
DEFINES = -DLINUX -DSAMPSRV -DAMX_NODYNALOAD

CFLAGS = -m32 -O2 -fpack-struct=1 -w $(INCLUDES) $(DEFINES)
CXXFLAGS = $(CFLAGS) -std=c++98
LIBS = -ldl -lpthread -lm

AMX_SOURCES = \
	$(AMX)/amx.c \
	$(AMX)/amxcons.c \
	$(AMX)/amxcore.c \
	$(AMX)/amxdgram.c \
	$(AMX)/amxfile.c \
	$(AMX)/amxstring.c \
	$(AMX)/amxtime.c \
	$(AMX)/float.c \
	$(AMX_LINUX)/getch.c

all:
	$(CC) $(CFLAGS) -c $(AMX_SOURCES)
	$(CC) $(CFLAGS) -c $(SQLITE)/sqlite3.c
	$(CXX) $(CXXFLAGS) -c *.cpp
	$(CXX) $(CXXFLAGS) -c $(RAKNET)/Source/*.cpp
	$(CXX) -m32 -s -o samp03svr *.o $(LIBS)

clean:
	rm -f *.o samp03svr

.PHONY: all clean
