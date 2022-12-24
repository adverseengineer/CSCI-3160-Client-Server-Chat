#Makefile for server directory

#Path Vars
#==============================================================================
SRC=source
SERVERINC=include
SERVERBUILD=build
COMMONDIR=../common
COMMONINC=$(COMMONDIR)/include
COMMONBUILD=$(COMMONDIR)/build
BIN=server

#Compilation Vars
#==============================================================================
LIBS=
CFLAGS=-Wall -Wextra -I$(SERVERINC) -I$(COMMONINC)

#Header Dependencies
#TODO: find a way to auto-detect them and not explicitly name them here
#==============================================================================
_SERVERDEPS=blacklist.h
SERVERDEPS=$(patsubst %,$(SERVERINC)/%,$(_SERVERDEPS))

_COMMONDEPS=common.h config.h
COMMONDEPS=$(patsubst %,$(COMMONINC)/%,$(_COMMONDEPS))

#Object Files
#only build the object files for server code, common objects are made elsewhere
#==============================================================================
_SERVEROBJ=server.o blacklist.o
SERVEROBJ=$(patsubst %,$(SERVERBUILD)/%,$(_SERVEROBJ))

_COMMONOBJ=common.o
COMMONOBJ=$(patsubst %,$(COMMONBUILD)/%,$(_COMMONOBJ))

$(SERVERBUILD)/%.o: $(SRC)/%.c $(SERVERDEPS) $(COMMONDEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
	
#Output Binaries
#==============================================================================
$(BIN): $(SERVEROBJ) $(COMMONOBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

#Misc Rules
#==============================================================================
clean:
	rm -f $(SERVERBUILD)/*.o $(BIN)

.PHONY: clean
