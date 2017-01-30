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
#include "plane.h"
#include "hitable.h"
#include "hitable_list.h"
#include "camera.h"

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
hitable* random_scene(int max_dist){
    int list_size = drand48() * max_dist;
    cout<<list_size<<endl;
    float max_radius = 1.5;
    hitable** list = new hitable*[list_size];
    vec3 world_center(0,0,0);
    for(int i=0; i<list_size; i++){
        int choice = drand48() * 3.99;
        vec3 offset = vec3(max_dist*drand48(), max_dist*drand48(), max_dist*drand48());
        switch(choice){
            case 2: //metal
                list[i] = new sphere(offset + world_center, max_radius*drand48(), new metal(vec3(drand48(), drand48(), drand48()), drand48()));
                break;
            case 3: //dielectric
                list[i] = new sphere(offset + world_center, max_radius*drand48(), new dielectric(drand48() * 4));
                break;
            default://diffuse
                list[i] = new sphere(offset + world_center, max_radius*drand48(), new lambertian(vec3(drand48(), drand48(), drand48())));
                break;
        }
    }
    return new hitable_list(list, list_size);
}


int main(){
    int nx = 400;
    int ny = 200;
    int ns = 100; //num samples?
    srand48(time(0));

    rgba8* pixels = new rgba8[nx*ny];
    hitable *list[4];
    list[3] = new sphere(vec3(0,0,-1), 0.5, new lambertian(vec3(0.1,0.1,0.8)));
    list[1] = new plane(vec3(0,-0.5,0), vec3(1,0,0), vec3(0,1,0), 10,10, new lambertian(vec3(0.5,0.5,0.5)));
    list[2] = new sphere(vec3(1,0,-1), 0.5, new metal(vec3(0.6,0.2,0.2), 0.0));
    list[0] = new sphere(vec3(-1,0,-1), 0.5, new dielectric(1.5));
    hitable* world = new hitable_list(list, 4);
//    hitable *world = random_scene(10);

    vec3 from(-1,1,2), to(0,0,-1);
    float ap = 0;
    float focus_dist = (from - to).length();
    camera cam(from, to, vec3(0,1,0), 90, float(nx) / ny, ap, focus_dist);
    int i=0;
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
    stbi_write_png("img.png", nx, ny, 4, pixels, nx * 4);
    //system("gimp img.png");
    system("open img.png");
}
