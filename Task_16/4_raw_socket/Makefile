BIN_CLI := raw_client
OBJ_CLI := raw_client.o
SRC_CLI := raw_client.c


.PHONY : all clean

all: $(BIN_CLI)

$(BIN_CLI): $(OBJ_CLI)
	gcc $(OBJ_CLI) -o $(BIN_CLI) -lncurses

$(OBJ_CLI): $(SRC_CLI)
	gcc -c $(SRC_CLI) -o $(OBJ_CLI)

clean:
	rm -rf $(BIN_CLI) $(OBJ_CLI)

