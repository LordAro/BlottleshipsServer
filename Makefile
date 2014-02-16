# This file is part of BlottleshipsServer.
# BlottleshipsServer is licenced under MIT License. See LICENSE for more details.

BINDIR=bin
OBJDIR=objs
SRCDIR=src

CXX=clang++
CXXFLAGS=-c -Wall -pedantic -g -std=c++11
LDFLAGS=-lboost_system
SOURCES=command.cpp blottleships.cpp manager.cpp player.cpp

OBJECTS=$(addprefix $(OBJDIR)/,$(SOURCES:.cpp=.o))
TARGET=$(BINDIR)/blottleships

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

run: $(TARGET)
	./$(TARGET)

gdbrun: $(TARGET)
	gdb $(TARGET)

clean:
	rm -f $(OBJECTS) $(TARGET)
