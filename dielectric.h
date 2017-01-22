#ifndef DIELECTRICH
#define DIELECTRICH

#include "vec3.h"
#include "hitable.h"
#include "material.h"
#include "ray.h"

class dielectric: public material{
public:
    float ref_index;

    dielectric(float ref_index){
        this->ref_index = ref_index;
    }
    virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const {
        vec3 outward_normal;
        vec3 reflected = reflect(r_in.direction(), rec.normal);
        float ni_over_nt;
        attenuation = vec3(1.0,1.0,0.0);
        vec3 refracted;
        if(dot(r_in.direction(), rec.normal)){
            outward_normal = -rec.normal;
            ni_over_nt = ref_index;
        }
        else{
            outward_normal = rec.normal;
            ni_over_nt = 1.0 / ref_index;
        }
        if(refract(r_in.direction(), outward_normal, ni_over_nt, refracted)){
            scattered = ray(rec.p, refracted);
            return true;
        }
        else{
            scattered = ray(rec.p, reflected);
            return false;
        }
    }
};
#endif
