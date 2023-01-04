# https://seisman.github.io/how-to-write-makefile/introduction.html
# -Wall display all warining

ALL_FILE = ./cpprenderer/TGAImage.cpp ./cpprenderer/TGAImage.h ./cpprenderer/main.cpp ./cpprenderer/ObjectModel.cpp ./cpprenderer/ObjectModel.h

OBJECTS = ./cpprenderer/TGAImage.cpp ./cpprenderer/main.cpp ./cpprenderer/ObjectModel.cpp

all : cpprendererapp

cpprendererapp : $(ALL_FILE)
	g++ -o cpprendererapp $(OBJECTS)

debugcompile : $(ALL_FILE)
	g++ -ggdb -g -pg -o cpprendererapp $(OBJECTS)

# main.o: ./cpprenderer/main.cpp #./cpprenderer/TGAImage.h
# 	g++ -c ./cpprenderer/main.cpp

# TGAImage.o: ./cpprenderer/TGAImage.cpp
# 	g++ -c ./cpprenderer/TGAImage.cpp

# cpprendererapp: cpprenderer.o TGAImage.o
# 	g++ -Wall -o ./cpprendererapp -lm

clean:
	-rm cpprendererapp

.PHONY: clean debugcompile
