# https://seisman.github.io/how-to-write-makefile/introduction.html
# -Wall display all warining

cpprendererapp: ./cpprenderer/TGAImage.cpp ./cpprenderer/TGAImage.h ./cpprenderer/main.cpp
	g++ -o cpprendererapp ./cpprenderer/TGAImage.cpp ./cpprenderer/main.cpp

# main.o: ./cpprenderer/main.cpp #./cpprenderer/TGAImage.h
# 	g++ -c ./cpprenderer/main.cpp

# TGAImage.o: ./cpprenderer/TGAImage.cpp
# 	g++ -c ./cpprenderer/TGAImage.cpp

# cpprendererapp: cpprenderer.o TGAImage.o
# 	g++ -Wall -o ./cpprendererapp -lm

clean:
	-rm cpprendererapp

.PHONY: clean
