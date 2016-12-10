//
//  Sphere.cpp
//  cs3388_assignment4_sacar2
//
//  Created by selin acar on 2016-12-09.
//  Copyright © 2016 selin acar. All rights reserved.
//

#include "Sphere.hpp"
#include "Vec3.hpp"
#include "Math.h"

Sphere::Sphere(double red, double green, double blue, Vec3 pos, double theta){
    r = red;
    g = green;
    b = blue;
    position = pos;
    ambient = 0.5;
    diffuse = 0.8;
    specular = 0.9;
    radius = theta;
    
}

// Calculate intersection of circle and direction array
//calculate using quadratic formula
Vec3 Sphere::getIntersection(Vec3 origin, Vec3 direction){

    double x_dir_sqr = direction.x*direction.x;
    double y_dir_sqr = direction.y*direction.y;
    double z_dir_sqr = direction.z*direction.z;
    
    double x_pos_sqr = position.x*position.x;
    double y_pos_sqr = position.y*position.y;
    double z_pos_sqr = position.z*position.z;
    
    double x_ori_sqr = origin.x*origin.x;
    double y_ori_sqr = origin.y*origin.y;
    double z_ori_sqr = origin.z*origin.z;
    
    double a = x_dir_sqr + y_dir_sqr + z_dir_sqr;
    double b = 2*(direction.x*(origin.x-position.x) + direction.y*(origin.y-position.y) + direction.z*(origin.z-position.z));
    double c = x_pos_sqr + y_pos_sqr + z_pos_sqr + x_ori_sqr + y_ori_sqr + z_ori_sqr - 2*(position.x*origin.x + position.y*origin.y + position.z*origin.z) - radius*radius;
    
    double discriminant = (b*b) - (4*a*c);
    
    if (discriminant<0){
        return Vec3();
    }
    double t = (-b) - sqrt(discriminant);
    
    return origin.add(direction.multiply(t));
}

//get the normal!
Vec3 Sphere::getNormal(Vec3 intersection){
    return intersection.subtract(position).normalize();
}

