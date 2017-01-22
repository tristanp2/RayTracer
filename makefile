CC=g++
CFLAGS=-Wall -g

all: linux	

linux: sphere.h hitable.h hitable_list.h camera.h dielectric.h camera.h material.h lambertian.h ray.h vec3.h ray_trace.cpp
	$(CC) $(CFLAGS) -o ray_trace ray_trace.cpp 
