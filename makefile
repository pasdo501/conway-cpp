OUT = bin
BUILD = build
PROG = conway conway_old conway_nowrap
OBJ = $(addprefix $(OUT)/, $(PROG))
CC = g++
FLAGS = -Wall -O2
SFML_FLAGS = -lsfml-graphics -lsfml-window -lsfml-system
vpath %.cpp src
vpath %.hpp src
vpath %.o $(BUILD)

all: $(OUT) $(BUILD) $(OBJ)

$(OUT):
	[ -d $(OUT) ] || mkdir -p $(OUT)

$(BUILD):
	[ -d $(BUILD) ] || mkdir -p $(BUILD)

$(OUT)/conway: $(BUILD)/conway.o $(BUILD)/main.o
	$(CC) $(FLAGS) -o $(OUT)/conway $(BUILD)/conway.o $(BUILD)/main.o $(SFML_FLAGS) -fopenmp

$(BUILD)/conway.o: src/conway.cpp
	$(CC) $(FLAGS) -c -o $(BUILD)/conway.o src/conway.cpp -fopenmp

$(BUILD)/main.o: src/main.cpp
	$(CC) $(FLAGS) -c -o $(BUILD)/main.o src/main.cpp $(SFML_FLAGS)

$(OUT)/conway_old: src/conway_old.cpp
	$(CC) $(FLAGS) -o $(OUT)/conway_old src/conway_old.cpp

$(OUT)/conway_nowrap: src/conway_nowrap.cpp
	$(CC) $(FLAGS) -o $(OUT)/conway_nowrap src/conway_nowrap.cpp

clean:
	rm -r $(OUT) $(BUILD)