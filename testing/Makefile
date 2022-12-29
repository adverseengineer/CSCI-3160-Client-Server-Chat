BIN=a.out

$(BIN): ui.c
	gcc $< -lncurses -lpanel -O0 -g -o$(BIN)

clean:
	rm $(BIN)

.PHONY: clean
