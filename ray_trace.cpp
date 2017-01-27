#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <iostream>
#include <unistd.h>
#include <cstdio>
#include <stdlib.h>
#include "stb_image_write.h"
#include "material.h"
#include "vec3.h"
#include "ray.h"
#include "sphere.h"
#include "hitable.h"
#include "hitable_list.h"
#include "camera.h"
//#include "lambertian.h"
//#include "metal.h"
//#include "dielectric.h"

#define MAX_FLOAT 200.0

using namespace std;

struct rgba8 {
    unsigned char r, g, b, a;
    rgba8() { }
    rgba8(unsigned char rr, unsigned char gg, unsigned char bb, unsigned char aa)
    : r(rr),g(gg),b(bb),a(aa) { }
    rgba8(vec3 v){
        *this = rgba8((unsigned char)(255.99 * v[0]), (unsigned char)(255.99 * v[1]), (unsigned char)(255.99 * v[2]), 255);
    }
};

vec3 colour(const ray& r, hitable* world, int depth){
    hit_record rec;
    if(world->hit(r, 0.001, MAX_FLOAT, rec)){
        vec3 attenuation;
        ray scattered;
        if(depth < 50 and rec.mat_ptr->scatter(r, rec, attenuation, scattered)){
            return attenuation * colour(scattered, world, depth+1);
        }
        else{
            return vec3(0,0,0);
        }
    }
    else{
        vec3 unit_direction = unit_vector(r.direction());
        float t = 0.5*(unit_direction.y() + 1.0);
        return (1.0-t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.1, 0.2, 1.0);
    }
}
int main(){
    int nx = 400;
    int ny = 200;
    int ns = 100; //num samples?

    rgba8* pixels = new rgba8[nx*ny];
    hitable *list[4];

    list[3] = new sphere(vec3(0,0,-1), 0.5, new lambertian(vec3(0.1,0.1,0.8)));
    list[1] = new sphere(vec3(0, -100.5, -1), 100, new lambertian(vec3(0.1,0.8,0.1)));
    list[2] = new sphere(vec3(1,0,-1), 0.5, new metal(vec3(0.6,0.2,0.2), 0.0));
    list[0] = new sphere(vec3(-1,0,-1), 0.5, new dielectric(1.5));
    hitable *world = new hitable_list(list, 4);

    camera cam(vec3(-1,0.5,2), vec3(0,0,-1), vec3(0,1,0), 90, float(nx) / ny);
    int i=0;
//    #pragma omp parallel for
    for(int j = ny-1; j >= 0; j--){
        for(i=0; i < nx; i++){
            vec3 col(0,0,0);
            for(int s=0; s < ns; s++){
                float u = float(i + drand48()) / float(nx); //drand48 returns rand float in range [0.0, 1.0]
                float v = float(j + drand48()) / float(ny);
               
                ray r = cam.get_ray(u,v);
                col += colour(r, world, 0);
            }
            col /= float(ns);
            col = vec3(sqrt(col.e[0]),sqrt(col.e[1]),sqrt(col.e[2]));
            pixels[(ny - 1 - j) * nx + i] = rgba8(col);
        }
    }
    std::cout<<std::endl;
    stbi_write_png("img.png", nx, ny, 4, pixels, nx * 4);
    system("gimp img.png");
}
