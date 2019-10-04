SRC = src
BIN = ./bin
BUILD = build
PROG = conway conway_old conway_nowrap
OBJ = $(addprefix $(BIN)/, $(PROG))
CC = g++
FLAGS = -Wall -O2
SFML_FLAGS = -lsfml-graphics -lsfml-window -lsfml-system
vpath %.cpp $(SRC)
vpath %.hpp $(SRC)
vpath %.o $(BUILD)

all: $(BIN) $(BUILD) $(OBJ)

$(BIN):
	[ -d $(BIN) ] || mkdir -p $(BIN)

$(BUILD):
	[ -d $(BUILD) ] || mkdir -p $(BUILD)

$(BIN)/conway: conway.o main.o
	$(CC) $(FLAGS) -o $(BIN)/conway $(BUILD)/conway.o $(BUILD)/main.o $(SFML_FLAGS) -fopenmp

conway.o: conway.cpp
	$(CC) $(FLAGS) -c -o $(BUILD)/conway.o $(SRC)/conway.cpp -fopenmp

main.o: main.cpp
	$(CC) $(FLAGS) -c -o $(BUILD)/main.o $(SRC)/main.cpp $(SFML_FLAGS)

$(BIN)/conway_old: conway_old.cpp
	$(CC) $(FLAGS) -o $(BIN)/conway_old $(SRC)/conway_old.cpp

$(BIN)/conway_nowrap: conway_nowrap.cpp
	$(CC) $(FLAGS) -o $(BIN)/conway_nowrap $(SRC)/conway_nowrap.cpp

clean:
	rm -r $(BIN) $(BUILD)