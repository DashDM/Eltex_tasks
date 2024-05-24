BIN := life_simulation
OBJ := main.o threads_c_l.o fill.o
SRC := main.c threads_c_l.c fill.c

.PHONY : clean
$(BIN): main.c threads_c_l.c fill.c
		gcc $(SRC) -c
		gcc $(OBJ) -o $(BIN)
clean:
		rm -rf $(BIN) main.o threads_c_l.o fill.o
		
