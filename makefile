CC=g++
CFLAGS=-Wall -g

all: linux	

linux: ray.h vec3.h ray_trace.cpp
	$(CC) $(CFLAGS) -o ray_trace ray_trace.cpp 
