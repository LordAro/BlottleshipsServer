CXX=clang++
CXXFLAGS=-c -Wall -pedantic -g -std=c++11
LDFLAGS=-lboost_system
SOURCES=blottleships.cpp manager.cpp player.cpp
OBJECTS=$(SOURCES:.cpp=.o)
TARGET=blottleships

all: $(SOURCES) $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CXX) $(CXXFLAGS) $< -o $@

run: $(TARGET)
	./$(TARGET)
