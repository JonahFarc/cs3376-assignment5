SHELL = /bin/sh
FLAGS = -g -Wall
CC = g++

all:    TwoPipesTwoChildren TwoPipesThreeChildren DynPipe  

TwoPipesTwoChildren:     TwoPipesTwoChildren.cpp
	$(CC) $(FLAGS) -o $@ TwoPipesTwoChildren.cpp

TwoPipesThreeChildren:     TwoPipesThreeChildren.cpp
	$(CC) $(FLAGS) -o $@ TwoPipesThreeChildren.cpp

DynPipe:    DynPipe.cpp
	$(CC) $(FLAGS) -o $@ DynPipe.cpp

clean:
	rm TwoPipesTwoChildren TwoPipesThreeChildren DynPipe
