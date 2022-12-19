

#PATHS
#==============================================================================
SRC=source
CLIENTSRC=$(SRC)/client
SERVERSRC=$(SRC)/server
COMMONSRC=$(SRC)/common

INC=include
CLIENTINC=$(INC)/client
SERVERINC=$(INC)/server
COMMONINC=$(INC)/common

BUILD=build
CLIENTBUILD=$(BUILD)/client
SERVERBUILD=$(BUILD)/server
COMMONBUILD=$(BUILD)/common

BIN=bin
CLIENTBIN=$(BIN)/client
SERVERBIN=$(BIN)/server


#COMPILER AND FLAGS
#==============================================================================
CC=gcc
LIBS=-pthread
CFLAGS=-Wall -Wextra -I$(INC)


#OBJECT NAMES
#these have to be manually edited when we add a new source file
#TODO: find a way around this restriction. makefiles suck
#TODO: remove the need for subdirectories of build
#==============================================================================
_SERVEROBJ=server.o blacklist.o 
SERVEROBJ=$(patsubst %,$(SERVERBUILD)/%,$(_SERVEROBJ))

_CLIENTOBJ=client.o 
CLIENTOBJ=$(patsubst %,$(CLIENTBUILD)/%,$(_CLIENTOBJ))

_COMMONOBJ=
COMMONOBJ=$(patsubst %,$(COMMONBUILD)/%,$(_COMMONOBJ))


#OBJECTS
#==============================================================================
#client depends on client source and headers, and common headers
$(CLIENTBUILD)/%.o: $(CLIENTSRC)/%.c $(CLIENTINC) $(COMMONINC)
	mkdir -p $(CLIENTBUILD)
	$(CC) -c -o $@ $< $(CFLAGS)

#server depends on server source and headers, and common headers
$(SERVERBUILD)/%.o: $(SERVERSRC)/%.c $(SERVERINC) $(COMMONINC)
	mkdir -p $(SERVERBUILD)
	$(CC) -c -o $@ $< $(CFLAGS)

#common only need common source and headers
$(COMMONBUILD)/%.o: $(COMMONSRC)/%.c $(COMMONINC)
	mkdir -p $(COMMONBUILD)
	$(CC) -c -o $@ $< $(CFLAGS)


#EXECUTABLES
#==============================================================================
#build both client and server
all: $(CLIENTBIN) $(SERVERBIN)

#build client using client objects and common objects
$(CLIENTBIN): $(CLIENTOBJ) $(COMMONOBJ)
	mkdir -p $(BIN)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

#build server using server objects and common objects
$(SERVERBIN): $(SERVEROBJ) $(COMMONOBJ)
	mkdir -p $(BIN)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)


#MISC
#==============================================================================
.PHONY: clean

clean:
	rm -f $(CLIENTBUILD)/*.o $(SERVERBUILD)/*.o $(COMMONBUILD)/*.o $(CLIENTBIN) $(SERVERBIN)
