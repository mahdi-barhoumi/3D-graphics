CC      = gcc
CXX     = g++
LIB_DIR = ./lib
INCLUDE_DIR = ./include
OBJ_DIR = ./build

# Recursive wildcard to find matching files
rwildcard = $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2) $(filter $(subst *,%,$2),$d))

# Source files (both C and C++)
C_SRCS   = $(call rwildcard,src/,*.c)
CPP_SRCS = $(call rwildcard,src/,*.cpp)

# Object files
C_OBJS   = $(patsubst src/%.c,$(OBJ_DIR)/%.o,$(C_SRCS))
CPP_OBJS = $(patsubst src/%.cpp,$(OBJ_DIR)/%.o,$(CPP_SRCS))

OBJS = $(C_OBJS) $(CPP_OBJS)

# Base Flags
CXXFLAGS = -std=c++23 -I$(INCLUDE_DIR) -DGLEW_STATIC -DSTB_IMAGE_IMPLEMENTATION -DGLFW_EXPOSE_NATIVE_WIN32 -DGLFW_EXPOSE_NATIVE_WGL
CFLAGS = -I$(INCLUDE_DIR)
LDFLAGS = -L$(LIB_DIR) -lglew -lglfw -lopengl32 -lgdi32 -luser32 -lkernel32

.PHONY: all clean debug

# Default build
all: Application

# Debug build
debug: CFLAGS += -g -O0
debug: CXXFLAGS += -g -O0
debug: Application

Application: $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

# Compile C files
$(OBJ_DIR)/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile C++ files
$(OBJ_DIR)/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) Application