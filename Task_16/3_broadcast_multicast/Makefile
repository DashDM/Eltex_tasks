BIN_SRV_BC := broadcast_server
OBJ_SRV_BC := broadcast_server.o
SRC_SRV_BC := broadcast_server.c

BIN_CLI_BC := broadcast_client
OBJ_CLI_BC := broadcast_client.o
SRC_CLI_BC := broadcast_client.c

BIN_SRV_MC := multicast_server
OBJ_SRV_MC := multicast_server.o
SRC_SRV_MC := multicast_server.c

BIN_CLI_MC := multicast_client
OBJ_CLI_MC := multicast_client.o
SRC_CLI_MC := multicast_client.c

.PHONY : all clean

all: $(BIN_SRV_BC) $(BIN_CLI_BC) $(BIN_SRV_MC) $(BIN_CLI_MC)

$(BIN_SRV_BC): $(OBJ_SRV_BC)
	gcc $(OBJ_SRV_BC) -o $(BIN_SRV_BC)

$(BIN_CLI_BC): $(OBJ_CLI_BC)
	gcc $(OBJ_CLI_BC) -o $(BIN_CLI_BC)
	
$(OBJ_SRV_BC): $(SRC_SRV_BC)
	gcc -c $(SRC_SRV_BC) -o $(OBJ_SRV_BC)

$(OBJ_CLI_BC): $(SRC_CLI_BC)
	gcc -c $(SRC_CLI_BC) -o $(OBJ_CLI_BC)
	
$(BIN_SRV_MC): $(OBJ_SRV_MC)
	gcc $(OBJ_SRV_MC) -o $(BIN_SRV_MC)

$(BIN_CLI_MC): $(OBJ_CLI_MC)
	gcc $(OBJ_CLI_MC) -o $(BIN_CLI_MC)
	
$(OBJ_SRV_MC): $(SRC_SRV_MC)
	gcc -c $(SRC_SRV_MC) -o $(OBJ_SRV_MC)

$(OBJ_CLI_MC): $(SRC_CLI_MC)
	gcc -c $(SRC_CLI_MC) -o $(OBJ_CLI_MC)

clean:
	rm -rf $(BIN_SRV_BC) $(BIN_CLI_BC) $(OBJ_SRV_BC) $(OBJ_CLI_BC) $(BIN_SRV_MC) $(BIN_CLI_MC) $(OBJ_SRV_MC) $(OBJ_CLI_MC)

