CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

# Detect OS
UNAME_S := $(shell uname -s)

SFML_PATH =
RPATH_FLAG =

# OS-specific settings
ifeq ($(UNAME_S),Linux)
    SFML_PATH = SFML/linux
    RPATH_FLAG = -Wl,-rpath=SFML/linux/lib
endif
ifeq ($(UNAME_S),Darwin)
    SFML_PATH = SFML/macos
    RPATH_FLAG = -Wl,-rpath,@executable_path/SFML/macos/lib
endif

SFML_INC = -I$(SFML_PATH)/include
SFML_LIB = -L$(SFML_PATH)/lib
SFML_LIBS = -lsfml-graphics -lsfml-window -lsfml-system

SRCS = main.cpp Hourglass_Grid.cpp Utility.cpp
OBJS = $(SRCS:.cpp=.o)

TARGET = hourglass

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@ $(SFML_INC) $(SFML_LIB) $(SFML_LIBS) $(RPATH_FLAG)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(SFML_INC) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean