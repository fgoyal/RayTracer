# Makefile for 419 ray tracer project
# Ian Rudnick

# Name of executable file
TARGET   := main

# C++ compiler and linker to use
CXX      := g++
LD		 := g++

# Compiler flags and linker flags
CXXFLAGS += -Ofast -std=c++11
CXXFLAGS += -pedantic -Wall -Werror -Wfatal-errors -Wextra -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -std=c++11
LDFLAGS	 +=


# Directories we need:
SRC_DIR	 	 := src
INC_DIR		 := include
BUILD_DIR    := build

# Add a prefix to the include directory so compiler can find it
INC_FLAGS := $(addprefix -I,$(INC_DIR))
CXXFLAGS += $(INC_FLAGS) -MMD -MP

# Set up commands depending on the operating system
ifeq ($(OS),Windows_NT)
	CXXFLAGS += -D WIN32
	FIX_PATH = $(subst /,\,$1)
	RM_CMD = del /q
	MKDIR_CMD = mkdir
else
	ifeq ($(shell uname), Linux)
		RM_CMD = rm -f
		MKDIR_CMD = mkdir -p
		FIX_PATH = $1
	endif
endif


# Find all the C and C++ files to compile.
SRCS := $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/*/*.cpp)
INCS := $(wildcard $(INC_DIR)/*.h)
OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# Make the list of dependencies from the list of objects.
# Using string substitution (suffix version without %)
DEPENDENCIES := $(OBJECTS:.o=.d)


# Default. We want to make sure the directories are there, then follow the
# instructions to build the executable.
all: $(TARGET)

# Rule to make the main executable
# Prerequisites: must have the objects ready
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@ $(LDFLAGS)

# Rule for C++ source
$(OBJS): $(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Include the .d Makefiles. The - suppresses the errors of missing Makefiles.
include $(DEPENDENCIES)

.PHONY: all clean info

# Clear the build directory and the compiled executable
clean:
	rm -f $(TARGET) $(BUILD_DIR)/*.o $(BUILD_DIR)/*.d $(BUILD_DIR)/*/*.o $(BUILD_DIR)/*/*.d

info:
	@echo "[*] Application dir: ${BIN_DIR}     "
	@echo "[*] Object dir:      ${BUILD_DIR}     "
	@echo "[*] Sources:         ${SRCS}        "
	@echo "[*] Objects:         ${OBJS}        "
	@echo "[*] Dependencies:    ${DEPENDENCIES}"
