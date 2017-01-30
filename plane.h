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
        float t = dot(n, center - r.origin()) / dot(n, r.direction());
        vec3 p = r.point_at_parameter(t); 
        
        if(t < 0)
            return false;

        rec.t = t;
        rec.p = p;
        rec.normal = n;
        rec.mat_ptr = mat;
        return true;
    }
};
#endif
