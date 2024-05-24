BIN := test
OBJ := main.o
SRC := main.c

.PHONY : clean
$(BIN): main.c
		gcc $(SRC) -c
		gcc $(OBJ) -o $(BIN)
clean:
		rm -rf $(BIN) main.o
		
