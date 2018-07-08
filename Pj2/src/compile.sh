#!/bin/bash
cd /mnt/f/Code/PhysicalAnimation/Pj2/src
g++ task1.cpp object.cpp -std=c++11 -lGLEW -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -o task1
