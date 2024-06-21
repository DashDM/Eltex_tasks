BIN_SRV := server
OBJ_SRV := server.o
SRC_SRV := server.c

BIN_CLI := client
OBJ_CLI := client.o
SRC_CLI := client.c

.PHONY : all clean

all: $(BIN_SRV) $(BIN_CLI)

$(BIN_SRV): $(OBJ_SRV)
	gcc $(OBJ_SRV) -o $(BIN_SRV)

$(BIN_CLI): $(OBJ_CLI)
	gcc $(OBJ_CLI) -o $(BIN_CLI) -lncurses

$(OBJ_SRV): $(SRC_SRV)
	gcc -c $(SRC_SRV) -o $(OBJ_SRV)

$(OBJ_CLI): $(SRC_CLI)
	gcc -c $(SRC_CLI) -o $(OBJ_CLI)

clean:
	rm -rf $(BIN_SRV) $(BIN_CLI) $(OBJ_SRV) $(OBJ_CLI)

