CC = gcc
CXX = g++

AMX = ../third-party/amx/source/amx
AMX_LINUX = ../third-party/amx/source/linux
RAKNET = ../third-party/RakNet
RAKNET_COMPAT = .raknet-include
RAKNET_CASE_ALIASES = RakPeer.h RakPeerInterface.h RakServerInterface.h SystemAddressList.h
SQLITE = ../third-party/sqlite

INCLUDES = -I$(AMX) -I$(AMX_LINUX) -I$(RAKNET)/Include -I$(RAKNET_COMPAT) -I$(SQLITE)
DEFINES = -DLINUX -DSAMPSRV -DAMX_NODYNALOAD

COMMON_FLAGS = -m32 -O2 -w $(INCLUDES) $(DEFINES)
CFLAGS = $(COMMON_FLAGS) -fpack-struct=1
CXXFLAGS = $(COMMON_FLAGS) -std=c++98
LIBS = -ldl -lpthread -lm

AMX_SOURCES = \
	$(AMX)/amx.c \
	$(AMX)/amxcons.c \
	$(AMX)/amxcore.c \
	$(AMX)/amxdgram.c \
	$(AMX)/amxstring.c \
	$(AMX)/amxtime.c \
	$(AMX)/float.c \
	$(AMX_LINUX)/getch.c

all:
	mkdir -p $(RAKNET_COMPAT)
	for header in $(RAKNET_CASE_ALIASES); do \
		lower=$$(echo "$$header" | tr A-Z a-z); \
		ln -sf "../$(RAKNET)/Include/$$lower" "$(RAKNET_COMPAT)/$$header" || exit 1; \
	done
	$(CC) $(CFLAGS) -c $(AMX_SOURCES)
	$(CC) $(CFLAGS) '-D_T(x)=x' -c $(AMX)/amxfile.c
	$(CC) $(CFLAGS) -c $(SQLITE)/sqlite3.c
	$(CXX) $(CXXFLAGS) -c *.cpp
	$(CXX) $(CXXFLAGS) -D__int64='long long' -c $(RAKNET)/Source/*.cpp
	$(CXX) -m32 -s -o samp03svr *.o $(LIBS)

clean:
	rm -f *.o samp03svr
	rm -f $(addprefix $(RAKNET_COMPAT)/,$(RAKNET_CASE_ALIASES))
	if test -d $(RAKNET_COMPAT); then rmdir $(RAKNET_COMPAT); fi

.PHONY: all clean
