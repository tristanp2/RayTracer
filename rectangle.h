#ifndef RECTANGLEH
#define RECTANGLEH

//defines an infinite xzrect
class xzrect: public hitable{
public:
    vec3 n;              //normal vector
    vec3 center;
    material* mat;
    float w, h;

    xzrect(vec3 center, float w, float h, material* m){
        this->center = center;
        this->w = w;
        this->h = h;
        mat = m;
        n = vec3(0,1,0);
    }
    virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const{
        float t = (center.y() - r.origin().y()) / r.direction().y();

        if(t < t_min or t > t_max)
            return false;

        float half_width = w/2;
        float half_height = h/2;
        float x = r.origin().x() + t*r.direction().x();
        float z = r.origin().z() + t*r.direction().z();
        float x_min = center.x() - half_width;
        float x_max = center.x() + half_width;
        float z_min = center.z() - half_height;
        float z_max = center.z() + half_height;
        
        if( x < x_min or x > x_max or z < z_min or z > z_max)
           return false; 

        rec.t = t;
        rec.p = r.point_at_parameter(t);
        rec.normal = n;
        rec.mat_ptr = mat;
        return true;
    }
};
#endif
