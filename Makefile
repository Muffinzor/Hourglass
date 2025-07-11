CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

SFML_LIBS = -lsfml-graphics -lsfml-window -lsfml-system

SRCS = main.cpp Hourglass_Grid.cpp Utility.cpp
OBJS = $(SRCS:.cpp=.o)

TARGET = hourglass

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(SFML_LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean