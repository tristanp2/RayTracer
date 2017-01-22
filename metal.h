#ifndef METALH
#define METALH

#include "material.h"
#include "vec3.h"
#include "ray.h"
#include "hitable.h"

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
