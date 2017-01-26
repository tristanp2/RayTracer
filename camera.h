#ifndef CAMERAH
#define CAMERAH

#include <iostream>

class camera{
public:
    vec3 origin, lower_left, vertical, horizontal;
    camera(){
        origin = vec3(0.0, 0.0, 0.0);
        lower_left = vec3(-2.0, -1.0, -1.0);
        vertical = vec3(0.0, 2.0, 0.0);
        horizontal = vec3(4.0, 0.0, 0.0);
    }
    camera(vec3 look_from, vec3 look_at, vec3 v_up, float fov, float aspect){
        vec3 u, v, w;
        float theta = fov * M_PI / 180;
        float half_height = tan(theta/2);
        float half_width = aspect * half_height;
        origin = look_from;
        w = unit_vector(look_from - look_at);
        u = unit_vector(cross(v_up, w));
        v = cross(w,u);
        lower_left = origin - half_width*u - half_height*v - w;
        horizontal = 2*half_width*u;
        vertical = 2*half_height*v;
    }
    ray get_ray(float u, float v){
        return ray(origin, lower_left + u*horizontal + v*vertical - origin);
    }
};
#endif

