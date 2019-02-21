#include "light.h"
#include "phong_shader.h"
#include "ray.h"
#include "render_world.h"
#include "object.h"

vec3 Phong_Shader::
Shade_Surface(const Ray& ray,const vec3& intersection_point,
    const vec3& normal,int recursion_depth) const
{
    vec3 color;
    //determine the color
    color += world.ambient_color*world.ambient_intensity*color_ambient;
    
    Ray lightRay;
    vec3 lColor; //light color

    for(unsigned i = 0; i < this->world.lights.size(); i++){
      lightRay.direction = world.lights.at(i)->position - intersection_point;
      double raySqr = lightRay.direction.magnitude_squared();
      lightRay.direction = lightRay.direction.normalized();
      lightRay.endpoint = intersection_point; // world.lights.at(i)->position;
      if(world.enable_shadows){
            Hit hit;
            Ray ray2;
            ray2.endpoint = intersection_point;
            ray2.direction = lightRay.direction;
            ray2.direction = ray2.direction.normalized();
            hit = world.Closest_Intersection(lightRay);//ray2);
           if(hit.object != NULL && hit.dist < (world.lights.at(i)->position - intersection_point).magnitude()){// if(hit.dist > small_t){
              continue;	
//	if(hit.dist < sqrt(raySqr)){ continue;}
            }
      }
      lColor = world.lights.at(i)->Emitted_Light(lightRay.direction);
      lColor /= raySqr;
      double x = std::max(dot(lightRay.direction, normal.normalized()), 0.0);
      color += (lColor*color_diffuse*x);

      vec3 reflectDir = lightRay.direction - (2*dot(lightRay.direction, normal)*normal);
      reflectDir = reflectDir.normalized();
      vec3 oppositeDir = (intersection_point - ray.endpoint).normalized();//*-1;
      double spec = std::max(dot(reflectDir, oppositeDir),0.0);
      spec = pow(spec, specular_power);
     
     
      color += (lColor*color_specular*spec);
    }
    return color; 																																																													   
}
