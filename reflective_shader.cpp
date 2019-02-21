#include "reflective_shader.h"
#include "ray.h"
#include "render_world.h"

vec3 Reflective_Shader::
Shade_Surface(const Ray& ray,const vec3& intersection_point,
    const vec3& normal,int recursion_depth) const
{
    vec3 color;
   // TODO; // determine the color
    Ray refRay;
    refRay.endpoint = intersection_point;
    
    vec3 r = ray.direction - (2*dot(ray.direction, normal)*normal);
    refRay.direction = r;
    vec3 refColor = world.Cast_Ray(refRay, recursion_depth);
    vec3 locColor = shader->Shade_Surface(ray, intersection_point, normal, recursion_depth);
    color = reflectivity*refColor + (1 - reflectivity)*locColor;
    return color;
}
