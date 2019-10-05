SRC = ./src
BIN = ./bin
BUILD = ./build
PROG = conway
OBJ = $(addprefix $(BIN)/, $(PROG))
CC = g++
FLAGS = -Wall -O3
SFML_FLAGS = -lsfml-graphics -lsfml-window -lsfml-system
vpath %.cpp $(SRC)
vpath %.hpp $(SRC)
vpath %.o $(BUILD)

all: $(BIN) $(BUILD) $(OBJ)

$(BIN):
	[ -d $(BIN) ] || mkdir -p $(BIN)

$(BUILD):
	[ -d $(BUILD) ] || mkdir -p $(BUILD)

$(BIN)/conway: conway.o main.o cli_args.o
	$(CC) $(FLAGS) -o $(BIN)/conway $(BUILD)/conway.o $(BUILD)/main.o $(BUILD)/cli_args.o $(SFML_FLAGS) -fopenmp

conway.o: conway.cpp
	$(CC) $(FLAGS) -c -o $(BUILD)/conway.o $(SRC)/conway.cpp -fopenmp

main.o: main.cpp
	$(CC) $(FLAGS) -c -o $(BUILD)/main.o $(SRC)/main.cpp $(SFML_FLAGS)

cli_args.o: cli_args.cpp
	$(CC) $(FLAGS) -c -o $(BUILD)/cli_args.o $(SRC)/cli_args.cpp

clean:
	rm -r $(BIN) $(BUILD)