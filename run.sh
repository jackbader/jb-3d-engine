#!/bin/bash

clang++ -std=c++11 -o main core/*.cpp -I. -I$(brew --prefix)/include -L$(brew --prefix)/lib -lglfw -lassimp -framework OpenGL
./main