BIN := bash
OBJ :=  execute_and_start.o str_operations.o main.o my_bash.o
SRC :=  execute_and_start.c str_operations.c main.c my_bash.c
CFLAGS := -Wall -Werror -Wextra

.PHONY : clean
$(BIN):  execute_and_start.c str_operations.c my_bash.c
		gcc $(SRC) $(CFLAGS) -c
		gcc $(OBJ) $(CFLAGS) -o $(BIN)
clean:
		rm -rf $(BIN)  execute_and_start.o str_operations.o main.o my_bash.o
		
