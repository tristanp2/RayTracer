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
        refracted = ni_over_nt*(uv - n*dt) - n*sqrt(discriminant);
        return true;
    }
    else return false;
}
vec3 reflect(const vec3& v, const vec3& n){
    return v - 2*dot(v,n)*n;
}

class dielectric: public material{
public:
    float ref_index;

    dielectric(float ref_index){
        this->ref_index = ref_index;
    }
    virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const {
        vec3 outward_normal;
        vec3 reflected = reflect(r_in.direction(), rec.normal);
        float ni_over_nt, cosine, reflect_prob;
        attenuation = vec3(1.0,1.0,1.0);
        vec3 refracted;
        if(dot(r_in.direction(), rec.normal) > 0){
            outward_normal = -rec.normal;
            ni_over_nt = ref_index;
            cosine = ref_index * dot(r_in.direction(), rec.normal) / r_in.direction().length();
        }
        else{
            outward_normal = rec.normal;
            ni_over_nt = 1.0 / ref_index;
            cosine = -dot(r_in.direction(), rec.normal) / r_in.direction().length();
        }
        if(refract(r_in.direction(), outward_normal, ni_over_nt, refracted)){
            reflect_prob = schlick(cosine, ref_index);
        }
        else{
            reflect_prob = 1.0;
        }
        if(drand48() < reflect_prob){
            scattered = ray(rec.p, reflected);
        }
        else{
            scattered = ray(rec.p, refracted);
        }
        return true;
    }
};

class lambertian: public material{
public:
    vec3 albedo;

    lambertian(vec3 albedo){ 
        this->albedo = albedo;
    }
    virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const {
        vec3 target = rec.p + rec.normal + random_in_unit_sphere();
        scattered = ray(rec.p, target - rec.p);
        attenuation = albedo;
        return true;
    }
};

class metal: public material{
public:
    vec3 albedo;
    float fuzz;
    
    metal(vec3 albedo, float fuzz){
        this->albedo = albedo;
        if(fuzz < 1) this->fuzz = fuzz;
        else this->fuzz = 1;
    }
    
    virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const {
        vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
        scattered = ray(rec.p, reflected + fuzz*random_in_unit_sphere());
        attenuation = albedo;
        return dot(scattered.direction(), rec.normal) > 0;
    }
};
#endif
