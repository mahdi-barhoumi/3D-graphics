C = gcc
CPP = g++
LIB_DIR = ./lib
INCLUDE_DIR = ./include

SRCS = $(wildcard src/*.cpp)
OBJS = $(SRCS:.cpp=.o)

CXXFLAGS = -I $(INCLUDE_DIR) -std=c++17

all: Application

Application: $(OBJS)
	$(CPP) $(OBJS) -o Application -L$(LIB_DIR) -lglfw -lglew -lopengl32 -lgdi32

%.o: %.cpp
	$(CPP) $(CXXFLAGS) -c $< -o $@

clean:
	rm.exe -f $(OBJS)