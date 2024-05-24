BIN := bash
OBJ := compare_string.o del_newline_character.o main.o
SRC := compare_string.c del_newline_character.c main.c
CFLAGS := -Wall -Werror -Wextra

.PHONY : clean
$(BIN): compare_string.c del_newline_character.c main.c
		gcc $(SRC) $(CFLAGS) -c
		gcc $(OBJ) $(CFLAGS) -o $(BIN)
clean:
		rm -rf $(BIN) compare_string.o del_newline_character.o main.o 
		
