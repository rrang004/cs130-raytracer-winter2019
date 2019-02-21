#include "render_world.h"
#include "flat_shader.h"
#include "object.h"
#include "light.h"
#include "ray.h"
#include <iostream>
#include <vector>
extern bool disable_hierarchy;

Render_World::Render_World()
    :background_shader(0),ambient_intensity(0),enable_shadows(true),
    recursion_depth_limit(3)
{}

Render_World::~Render_World()
{
    delete background_shader;
    for(size_t i=0;i<objects.size();i++) delete objects[i];
    for(size_t i=0;i<lights.size();i++) delete lights[i];
}

// Find and return the Hit structure for the closest intersection.  Be careful
// to ensure that hit.dist>=small_t.
Hit Render_World::Closest_Intersection(const Ray& ray)
{
    float min_t = 9999;
    Hit closestIntersect = {};
    unsigned smallestIndex = 0;
    for(unsigned i = 0; i < this->objects.size(); ++i){
        if(this->objects.at(i)->Intersection(ray, -1).dist < min_t && this->objects.at(i)->Intersection(ray, -1).dist > small_t ){
           smallestIndex = i;
           min_t = this->objects.at(smallestIndex)->Intersection(ray, -1).dist;
           closestIntersect = this->objects.at(smallestIndex)->Intersection(ray, -1);
        }
    }
 
    return closestIntersect;
}

// set up the initial view ray and call
void Render_World::Render_Pixel(const ivec2& pixel_index)
{
    // set up the initial view ray here
    Ray ray(camera.position, (this->camera.World_Position(pixel_index) - camera.position).normalized());
       
    vec3 color=Cast_Ray(ray,1);
    camera.Set_Pixel(pixel_index,Pixel_Color(color));
}

void Render_World::Render()
{
    if(!disable_hierarchy)
        Initialize_Hierarchy();

    for(int j=0;j<camera.number_pixels[1];j++)
        for(int i=0;i<camera.number_pixels[0];i++)
            Render_Pixel(ivec2(i,j));
}

// cast ray and return the color of the closest intersected surface point,
// or the background color if there is no object intersection
vec3 Render_World::Cast_Ray(const Ray& ray,int recursion_depth)
{
    Hit hit;
    vec3 color;
    
    hit = this->Closest_Intersection(ray);
    //Object is null if no closest intersection
    if(hit.object != 0 && recursion_depth <= recursion_depth_limit){
       vec3 pointIntersect = ray.Point(hit.dist);
       vec3 normal = hit.object->Normal(pointIntersect,-1);
       color = hit.object->material_shader->Shade_Surface(ray, pointIntersect, normal, recursion_depth + 1);
}   
    else {
       vec3 empty;
       color = background_shader->Shade_Surface(ray, empty, empty, recursion_depth + 1); 
    }  
    
    return color;
}

void Render_World::Initialize_Hierarchy()
{
    TODO; // Fill in hierarchy.entries; there should be one entry for
    // each part of each object.

    hierarchy.Reorder_Entries();
    hierarchy.Build_Tree();
}
