#Nick Sells, 2022
#CSCI 3160 Final Project

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

#Object Files
#==============================================================================
COMMONDEPS=$(patsubst %,$(COMMONINC)/%,$(_COMMONDEPS))
CLIENTDEPS=$(patsubst %,$(CLIENTINC)/%,$(_CLIENTDEPS))

$(CLIENTBUILD)/%.o: $(SRC)/%.c $(CLIENTDEPS) $(COMMONDEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

#Output Binaries
#==============================================================================
COMMONOBJ=$(patsubst %,$(COMMONBUILD)/%,$(_COMMONOBJ))
CLIENTOBJ=$(patsubst %,$(CLIENTBUILD)/%,$(_CLIENTOBJ))

$(BIN): $(CLIENTOBJ) $(COMMONOBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

#Misc Rules
#==============================================================================
clean:
	rm -f $(CLIENTBUILD)/*.o $(BIN)

.PHONY: clean
