#ifndef PLANEH
#define PLANEH

class plane: public hitable{
public:
    float w, h;
    vec3 pa, pb, pc, pd; //corner points
    vec3 n;              //normal vector
    vec3 center;
    material* mat;

    plane(vec3 center, vec3 horizontal, vec3 normal, float w, float h, material* m){
        this->center = center;
        this->w = w;
        this->h = h;
        mat = m;
        float half_width = w/2, half_height = h/2;
        n = unit_vector(normal);
        horizontal = unit_vector(horizontal);
        vec3 vertical = unit_vector(cross(horizontal, n));
        pa = half_width*horizontal + half_height*vertical + center;
        pb = -half_width*horizontal + half_height*vertical + center;
        pc = -half_width*horizontal - half_height*vertical + center;
        pd = half_width*horizontal - half_height*vertical + center;
    }
    virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const{
        float f1, f2, f3, f4;
        float t = dot(n, center - r.origin()) / dot(n, r.direction());
        vec3 p = r.point_at_parameter(t); 
        
        if(t < 0 or (p - center).squared_length() > (w*w + h*h))
            return false;

        f1 = dot(n, cross(pb - pa, p - pa));
        f2 = dot(n, cross(pc - pb, p - pb));
        f3 = dot(n, cross(pd - pc, p - pc));
        f4 = dot(n, cross(pa - pd, p - pd));
        if(f1 < 0 or f2 < 0 or f3 < 0 or f4 < 0)
            if(f1 > 0 or f2 > 0 or f3 > 0 or f4 > 0)
                return false;

        rec.t = t;
        rec.p = p;
        rec.normal = n;
        rec.mat_ptr = mat;
        return true;
    }
};
#endif
