#Nick Sells, 2022
#CSCI 3160 Final Project

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
LIBS=-pthread
CFLAGS=-Wall -Wextra -I$(SERVERINC) -I$(COMMONINC)

#Object Files
#==============================================================================
COMMONDEPS=$(patsubst %,$(COMMONINC)/%,$(_COMMONDEPS))
SERVERDEPS=$(patsubst %,$(SERVERINC)/%,$(_SERVERDEPS))

$(SERVERBUILD)/%.o: $(SRC)/%.c $(SERVERDEPS) $(COMMONDEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
	
#Output Binaries
#==============================================================================
SERVEROBJ=$(patsubst %,$(SERVERBUILD)/%,$(_SERVEROBJ))
COMMONOBJ=$(patsubst %,$(COMMONBUILD)/%,$(_COMMONOBJ))

$(BIN): $(SERVEROBJ) $(COMMONOBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

#Misc Rules
#==============================================================================
clean:
	rm -f $(SERVERBUILD)/*.o $(BIN)

.PHONY: clean
