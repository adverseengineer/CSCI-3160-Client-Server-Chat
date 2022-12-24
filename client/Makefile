#Makefile for client directory

#Path Vars
#==============================================================================
SRC=source
CLIENTINC=include
CLIENTBUILD=build
COMMONDIR=../common
COMMONINC=$(COMMONDIR)/include
COMMONBUILD=$(COMMONDIR)/build
BIN=client

#Compilation Vars
#==============================================================================
LIBS=-lcurses
CFLAGS=-Wall -Wextra -I$(CLIENTINC) -I$(COMMONINC)

#Header Dependencies
#TODO: find a way to auto-detect them and not explicitly name them here
#==============================================================================
_CLIENTDEPS=
CLIENTDEPS=$(patsubst %,$(CLIENTINC)/%,$(_CLIENTDEPS))

_COMMONDEPS=common.h config.h
COMMONDEPS=$(patsubst %,$(COMMONINC)/%,$(_COMMONDEPS))

#Object Files
#only build the object files for client code, common objects are made elsewhere
#==============================================================================
_CLIENTOBJ=client.o
CLIENTOBJ=$(patsubst %,$(CLIENTBUILD)/%,$(_CLIENTOBJ))

_COMMONOBJ=common.o
COMMONOBJ=$(patsubst %,$(COMMONBUILD)/%,$(_COMMONOBJ))

$(CLIENTBUILD)/%.o: $(SRC)/%.c $(CLIENTDEPS) $(COMMONDEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
	
#Output Binaries
#==============================================================================
$(BIN): $(CLIENTOBJ) $(COMMONOBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

#Misc Rules
#==============================================================================
clean:
	rm -f $(CLIENTBUILD)/*.o $(BIN)

.PHONY: clean
