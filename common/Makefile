#Makefile for common directory

#Path Vars
#==============================================================================
SRC=source
INC=include
BUILD=build

#Compilation Vars
#==============================================================================
CFLAGS=-I$(INC) -Wall -Wextra

#Header Dependencies
#TODO: find a way to auto-detect them and not explicitly name them here
#==============================================================================
_DEPS=common.h config.h
DEPS=$(patsubst %,$(INC)/%,$(_DEPS))

#Object Files
#==============================================================================
_OBJ=common.o
OBJ=$(patsubst %,$(BUILD)/%,$(_OBJ))

default: $(OBJ)

$(BUILD)/%.o: $(SRC)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

#Misc Rules
#==============================================================================
clean:
	rm -f $(BUILD)/*.o

.PHONY: default clean
