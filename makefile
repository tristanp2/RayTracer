CC=g++
CFLAGS=-Wall -g
MP=-fopenmp
all: linux	

linux: sphere.h hitable.h hitable_list.h camera.h camera.h material.h ray.h vec3.h ray_trace.cpp
	$(CC) $(CFLAGS) -o ray_trace ray_trace.cpp 
mp: linux
	$(MP)
