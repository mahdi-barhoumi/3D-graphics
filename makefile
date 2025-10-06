C = gcc
CPP = g++
LIB_DIR = ./lib
INCLUDE_DIR = ./include
OBJ_DIR = ./build

SRCS = $(wildcard src/*.cpp)
OBJS = $(patsubst src/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))

CXXFLAGS = -I $(INCLUDE_DIR) -std=c++17

.PHONY: all clean

all: Application

Application: $(OBJS)
	$(CPP) $(OBJS) -o Application -L$(LIB_DIR) -lglfw -lglew -lopengl32 -lgdi32

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: src/%.cpp | $(OBJ_DIR)
	$(CPP) $(CXXFLAGS) -c $< -o $@

clean:
	rm.exe -rf $(OBJ_DIR) Application