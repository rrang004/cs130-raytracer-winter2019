#include "sphere.h"
#include "ray.h"

// Determine if the ray intersects with the sphere
Hit Sphere::Intersection(const Ray& ray, int part) const
{
    vec3 p = ray.endpoint - center;
    double b = dot(ray.direction*2, p);
    double c = dot(p,p) - (radius*radius);
    double discriminant = (b*b) - (4*c);

    if(discriminant > 0){
       double root1 = (-1*b) - sqrt(discriminant);
       root1 /=2;
       double root2 = (-1*b) + sqrt(discriminant);
       root2 /= 2;
       
       Hit hit1, hit2;
       if(root2 > 0){
          if(root1 > 0){
            hit1.object = this;
            hit1.dist = root1;
            return hit1;
          }
          hit2.object = this;
          hit2.dist = root2;
          return hit2;
       }

    }
    return {0,0,0};
}

vec3 Sphere::Normal(const vec3& point, int part) const
{
    vec3 normal;
    // compute the normal direction
    normal = point - center;
    return normal.normalized();
}

Box Sphere::Bounding_Box(int part) const
{
    Box box;
    TODO; // calculate bounding box
    return box;
}
