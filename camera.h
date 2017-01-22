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
    ray get_ray(float u, float v){
        return ray(origin, lower_left + u*horizontal + v*vertical - origin);
    }
};
#endif

