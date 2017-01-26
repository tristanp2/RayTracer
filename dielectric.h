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
#endif
