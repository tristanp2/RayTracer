#ifndef MATERIALH
#define MATERIALH

#include <math.h>
#include "ray.h"
#include "hitable.h"
#include "vec3.h"

class material{
public:
    virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const = 0;
};

vec3 random_in_unit_sphere(){
    vec3 p;
    do{
        p = 2.0*(vec3(drand48(),drand48(), drand48()) - vec3(1,1,1));
    }while(dot(p,p) >= 1.0);
    return p;
}

float schlick(float cosine, float ref_index){
    float r0 = (1 - ref_index) / (1 + ref_index);
    r0 = r0*r0;
    return r0 + (1 - r0)*pow(1 - cosine, 5);
}
bool refract(const vec3& v, const vec3& n, float ni_over_nt, vec3& refracted){
    vec3 uv = unit_vector(v);
    float dt = dot(uv, n);
    float discriminant = 1.0 - ni_over_nt*ni_over_nt*(1-dt*dt);
    if(discriminant > 0){
        refracted = ni_over_nt*(v - n*dt) - n*sqrt(discriminant);
        return true;
    }
    else return false;
}
vec3 reflect(const vec3& v, const vec3& n){
    return v - 2*dot(v,n)*n;
}
#endif
