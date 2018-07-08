#!/bin/bash
cd /mnt/f/Code/PhysicalAnimation/Pj1/src
g++ task2.cpp particle.cpp -std=c++11 -lGLEW -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -o task2
