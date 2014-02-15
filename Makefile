# This file is part of BlottleshipsServer.
# BlottleshipsServer is licenced under MIT License. See LICENSE for more details.

CXX=clang++
CXXFLAGS=-c -Wall -pedantic -g -std=c++11
LDFLAGS=-lboost_system
SOURCES=blottleships.cpp manager.cpp player.cpp

OBJDIR=objs
OBJECTS=$(addprefix $(OBJDIR)/,$(SOURCES:.cpp=.o))
TARGET=blottleships

all: $(SOURCES) $(TARGET)

$(TARGET): $(OBJECTS)
	echo $(OBJECTS)
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $@

$(OBJDIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJECTS) $(TARGET)
