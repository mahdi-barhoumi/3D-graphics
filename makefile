
C = gcc
CPP = g++
LIB_DIR = ./lib
INCLUDE_DIR = ./include

all: Application clean

Application: main.o
	$(CPP) main.o -o Application -L$(LIB_DIR) -lglfw -lglew -lopengl32 -lgdi32

main.o: main.cpp 
	$(CPP) -c main.cpp -I $(INCLUDE_DIR)

clean:
	rm.exe -f *.o