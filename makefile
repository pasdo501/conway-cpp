OUT = bin
BUILD = build
CC = g++
FLAGS = -Wall -O2
SFML_FLAGS = -lsfml-graphics -lsfml-window -lsfml-system

all: $(OUT) $(BUILD) $(OUT)/main $(OUT)/conway $(OUT)/conway_nowrap

$(OUT):
	[ -d $(OUT) ] || mkdir -p $(OUT)

$(BUILD):
	[ -d $(BUILD) ] || mkdir -p $(BUILD)

$(OUT)/main: $(BUILD)/conway_class.o $(BUILD)/main.o
	$(CC) $(FLAGS) -o $(OUT)/main $(BUILD)/conway_class.o $(BUILD)/main.o $(SFML_FLAGS) -fopenmp

$(BUILD)/conway_class.o: src/conway_class.cpp
	$(CC) $(FLAGS) -c -o $(BUILD)/conway_class.o src/conway_class.cpp -fopenmp

$(BUILD)/main.o: src/main.cpp
	$(CC) $(FLAGS) -c -o $(BUILD)/main.o src/main.cpp $(SFML_FLAGS)

$(OUT)/conway: src/conway.cpp
	$(CC) $(FLAGS) -o $(OUT)/conway src/conway.cpp

$(OUT)/conway_nowrap: src/conway_nowrap.cpp
	$(CC) $(FLAGS) -o $(OUT)/conway_nowrap src/conway_nowrap.cpp

$(OUT)/conway_class: src/conway_class.cpp
	$(CC) $(FLAGS) -o $(OUT)/conway_class src/conway_class.cpp -fopenmp

clean:
	rm -r $(OUT) $(BUILD)