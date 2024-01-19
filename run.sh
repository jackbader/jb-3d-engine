#!/bin/bash

clang++ -o main core/*.cpp -I. -I$(brew --prefix)/include -L$(brew --prefix)/lib -lglfw -framework OpenGL
./main