CC      = gcc
CXX     = g++
LIB_DIR = ./lib
INCLUDE_DIR = ./include
SRC_DIR = ./src
BUILD_DIR = ./build
RC      = resource.rc

# Recursive wildcard to find matching files
rwildcard = $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2) $(filter $(subst *,%,$2),$d))

# Source files
C_SRCS   = $(call rwildcard,$(SRC_DIR)/,*.c)
CPP_SRCS = $(call rwildcard,$(SRC_DIR)/,*.cpp)

# Base flags
BASE_CFLAGS   = -I$(INCLUDE_DIR)
BASE_CXXFLAGS = -std=c++23 -I$(INCLUDE_DIR) -DGLEW_STATIC -DSTB_IMAGE_IMPLEMENTATION -DGLFW_EXPOSE_NATIVE_WIN32 -DGLFW_EXPOSE_NATIVE_WGL -DAL_LIBTYPE_STATIC
BASE_LDFLAGS  = -L$(LIB_DIR) -lglew -lglfw -lopengl32 -lopenal32 -lgdi32 -luser32 -lkernel32 -lshell32 -lole32 -luuid -lwinmm -lavrt

# Debug configuration
DEBUG_OBJ_DIR = $(BUILD_DIR)/debug
DEBUG_DEP_DIR = $(BUILD_DIR)/debug/deps
DEBUG_C_OBJS   = $(patsubst $(SRC_DIR)/%.c,$(DEBUG_OBJ_DIR)/%.o,$(C_SRCS))
DEBUG_CPP_OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(DEBUG_OBJ_DIR)/%.o,$(CPP_SRCS))
DEBUG_RESOURCE_OBJ = $(DEBUG_OBJ_DIR)/resource.o
DEBUG_OBJS = $(DEBUG_C_OBJS) $(DEBUG_CPP_OBJS) $(DEBUG_RESOURCE_OBJ)
DEBUG_C_DEPS   = $(patsubst $(SRC_DIR)/%.c,$(DEBUG_DEP_DIR)/%.d,$(C_SRCS))
DEBUG_CPP_DEPS = $(patsubst $(SRC_DIR)/%.cpp,$(DEBUG_DEP_DIR)/%.d,$(CPP_SRCS))
DEBUG_CFLAGS   = $(BASE_CFLAGS) -g -O0
DEBUG_CXXFLAGS = $(BASE_CXXFLAGS) -g -O0
DEBUG_LDFLAGS  = $(BASE_LDFLAGS)
DEBUG_TARGET   = ApplicationDebug

# Release configuration
RELEASE_OBJ_DIR = $(BUILD_DIR)/release
RELEASE_DEP_DIR = $(BUILD_DIR)/release/deps
RELEASE_C_OBJS   = $(patsubst $(SRC_DIR)/%.c,$(RELEASE_OBJ_DIR)/%.o,$(C_SRCS))
RELEASE_CPP_OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(RELEASE_OBJ_DIR)/%.o,$(CPP_SRCS))
RELEASE_RESOURCE_OBJ = $(RELEASE_OBJ_DIR)/resource.o
RELEASE_OBJS = $(RELEASE_C_OBJS) $(RELEASE_CPP_OBJS) $(RELEASE_RESOURCE_OBJ)
RELEASE_C_DEPS   = $(patsubst $(SRC_DIR)/%.c,$(RELEASE_DEP_DIR)/%.d,$(C_SRCS))
RELEASE_CPP_DEPS = $(patsubst $(SRC_DIR)/%.cpp,$(RELEASE_DEP_DIR)/%.d,$(CPP_SRCS))
RELEASE_CFLAGS   = $(BASE_CFLAGS) -O3 -march=native
RELEASE_CXXFLAGS = $(BASE_CXXFLAGS) -O3 -march=native
RELEASE_LDFLAGS  = $(BASE_LDFLAGS) -flto -static -static-libgcc -static-libstdc++ #-mwindows
RELEASE_TARGET   = Application

.PHONY: debug release clean

# Default target
debug: $(DEBUG_TARGET)

release: $(RELEASE_TARGET)

# Link debug target
$(DEBUG_TARGET): $(DEBUG_OBJS)
	$(CXX) $(DEBUG_OBJS) -o $@ $(DEBUG_LDFLAGS)

# Link release target
$(RELEASE_TARGET): $(RELEASE_OBJS)
	$(CXX) $(RELEASE_OBJS) -o $@ $(RELEASE_LDFLAGS)

# Debug resource compilation
$(DEBUG_RESOURCE_OBJ): $(RC)
	@mkdir -p $(dir $@)
	windres -i $< -o $@

# Release resource compilation
$(RELEASE_RESOURCE_OBJ): $(RC)
	@mkdir -p $(dir $@)
	windres -i $< -o $@

# Debug C compilation
$(DEBUG_OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@) $(dir $(DEBUG_DEP_DIR)/$*.d)
	$(CC) $(DEBUG_CFLAGS) -MMD -MP -MF $(DEBUG_DEP_DIR)/$*.d -c $< -o $@

# Debug C++ compilation
$(DEBUG_OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@) $(dir $(DEBUG_DEP_DIR)/$*.d)
	$(CXX) $(DEBUG_CXXFLAGS) -MMD -MP -MF $(DEBUG_DEP_DIR)/$*.d -c $< -o $@

# Release C compilation
$(RELEASE_OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@) $(dir $(RELEASE_DEP_DIR)/$*.d)
	$(CC) $(RELEASE_CFLAGS) -MMD -MP -MF $(RELEASE_DEP_DIR)/$*.d -c $< -o $@

# Release C++ compilation
$(RELEASE_OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@) $(dir $(RELEASE_DEP_DIR)/$*.d)
	$(CXX) $(RELEASE_CXXFLAGS) -MMD -MP -MF $(RELEASE_DEP_DIR)/$*.d -c $< -o $@

# Include dependency files
-include $(DEBUG_C_DEPS) $(DEBUG_CPP_DEPS)
-include $(RELEASE_C_DEPS) $(RELEASE_CPP_DEPS)

clean:
	rm -rf $(BUILD_DIR) $(DEBUG_TARGET) $(RELEASE_TARGET)