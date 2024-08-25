# Makefile for custom shell project
#
# This Makefile compiles the custom shell project, including
# main.cpp, shell.cpp, and history.cpp into an executable named 'shell'.
#
# Author: Deep padmani
# Date: August 25, 2024

CXX = g++
CXXFLAGS = -std=c++11
TARGET = Matic
SOURCES = MaticMain.cpp MaticBash.cpp
OBJECTS = $(SOURCES:.cpp=.o)


$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f $(TARGET) *.o
