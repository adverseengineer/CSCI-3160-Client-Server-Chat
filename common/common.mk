#Nick Sells, 2022
#CSCI 3160 Final Project

#Path Vars
#==============================================================================
SRC=source
COMMONINC=include
COMMONBUILD=build

#Compilation Vars
#==============================================================================
CFLAGS=-I$(COMMONINC) -Wall -Wextra

#Object Files
#==============================================================================
COMMONDEPS=$(patsubst %,$(COMMONINC)/%,$(_COMMONDEPS))

$(COMMONBUILD)/%.o: $(SRC)/%.c $(COMMONDEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

#Output Binaries
#==============================================================================
COMMONOBJ=$(patsubst %,$(COMMONBUILD)/%,$(_COMMONOBJ))
default: $(COMMONOBJ)

#Misc Rules
#==============================================================================
clean:
	rm -f $(COMMONBUILD)/*.o

.PHONY: default clean
