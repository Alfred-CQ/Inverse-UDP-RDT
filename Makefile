CC          = g++
LD          = g++ 
CFLAG       = -Wall -std=c++17

PROG_CLIENT	= client
PROG_SERVER = server

SRC_DIR		= ./src
BUILD_DIR	= ./build
BIN_DIR     = ./bin

SRC_LIST_CLIENT = $(wildcard $(SRC_DIR)/client.cpp)
OBJ_LIST_CLIENT = $(BUILD_DIR)/$(notdir $(SRC_LIST_CLIENT:.cpp=.o))

SRC_LIST_SERVER = $(wildcard $(SRC_DIR)/server.cpp)
OBJ_LIST_SERVER = $(BUILD_DIR)/$(notdir $(SRC_LIST_SERVER:.cpp=.o))

.PHONY: all clean $(PROG_CLIENT) $(PROG_SERVER) compile_client compile_server

all: $(PROG_CLIENT) $(PROG_SERVER)

compile_client: 
	$(CC) -c $(CFLAG) $(SRC_LIST_CLIENT) -o $(OBJ_LIST_CLIENT)

compile_server: 
	$(CC) -c $(CFLAG) $(SRC_LIST_SERVER) -o $(OBJ_LIST_SERVER)

$(PROG_CLIENT): compile_client
	$(LD) $(OBJ_LIST_CLIENT) -o $(BIN_DIR)/$@

$(PROG_SERVER): compile_server
	$(LD) $(OBJ_LIST_SERVER) -o $(BIN_DIR)/$@

clean:
	rm -f $(BIN_DIR)/$(PROG_CLIENT) $(BIN_DIR)/$(PROG_SERVER) $(BUILD_DIR)/*.o