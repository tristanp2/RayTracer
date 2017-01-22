#ifndef SPHEREH
#define SPHEREH

#include "vec3.h"
#include "ray.h"
#include "hitable.h"
#include "material.h"
#include <math.h>

class sphere: public hitable{
public:
    sphere(){}
    sphere(vec3 cen, float r, material* m) : center(cen), radius(r), mat(m) {};
    virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;
    
    vec3 center;
    float radius;
    material* mat;

};
bool sphere::hit(const ray& r, float t_min, float t_max, hit_record& rec) const{
    vec3 oc = r.origin() - center;
    float a = dot(r.direction(), r.direction());
    float b = dot(oc, r.direction());
    float c = dot(oc,oc) - radius*radius;
    float discriminant = b*b - a*c;
    
    if(discriminant > 0){
        float temp = (-b - sqrt(discriminant)) / a;
        rec.mat_ptr = mat;
        if(temp < t_max and temp > t_min){
            rec.t = temp;
            rec.p = r.point_at_parameter(rec.t);
            rec.normal = (rec.p - center) / radius;
            return true;
        }
        temp = (-b + sqrt(discriminant)) / a;
        if(temp < t_max and temp > t_min){  //hmmm...
            rec.t = temp;
            rec.p = r.point_at_parameter(rec.t);
            rec.normal = (rec.p - center) / radius;
            return true;
        }
    }
    return false;
}
#endif 
