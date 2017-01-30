#ifndef PLANEH
#define PLANEH

//defines an infinite plane
class plane: public hitable{
public:
    vec3 n;              //normal vector
    vec3 center;
    material* mat;

    plane(vec3 center, vec3 normal, material* m){
        this->center = center;
        mat = m;
        n = unit_vector(normal);
    }
    virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const{
        float denom = dot(r.direction(), n);
        
        if(denom < 0)
            return false;

        float t = dot(center - r.origin(), n) / denom;
        vec3 p = r.point_at_parameter(t); 

        rec.t = t;
        rec.p = p;
        rec.normal = n;
        rec.mat_ptr = mat;
        return true;
    }
};
#endif
