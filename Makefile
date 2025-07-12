# Compiler and base flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

# Detect the OS
UNAME_S := $(shell uname -s)

# Default values
SFML_PATH =
RPATH_FLAG =

# OS-specific settings
ifeq ($(UNAME_S),Linux)
    SFML_PATH = SFML/linux
    RPATH_FLAG = -Wl,-rpath=SFML/linux/lib
endif

ifeq ($(UNAME_S),Darwin)
    SFML_PATH = SFML/macos
    # Use @executable_path so .dylibs are found relative to the binary
    RPATH_FLAG = -Wl,-rpath,@executable_path/SFML/macos/lib
endif

# SFML include and library flags
SFML_INC = -I$(SFML_PATH)/include
SFML_LIB = -L$(SFML_PATH)/lib
SFML_LIBS = -lsfml-graphics -lsfml-window -lsfml-system

# Source and object files
SRCS = main.cpp Hourglass_Grid.cpp Utility.cpp
OBJS = $(SRCS:.cpp=.o)

# Output binary name
TARGET = hourglass

# Default target
all: $(TARGET)

# Linking the final binary
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@ $(SFML_INC) $(SFML_LIB) $(SFML_LIBS) $(RPATH_FLAG)

# Compiling source files to object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(SFML_INC) -c $< -o $@

# Clean build files
clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean