//
//  Ground.cpp
//  cs3388_assignment4_sacar2
//
//  Created by selin acar on 2016-12-09.
//  Copyright © 2016 selin acar. All rights reserved.
//

#include "Ground.hpp"
#include "Vec3.hpp"
#include "Math.h"

// Ground initializer()
// create a ground object
Ground::Ground(double red, double green, double blue, Vec3 pos){
    r = red;
    g = green;
    b = blue;
    position = pos;
    ambient = 0.2;
    diffuse = 0.8;
    specular = 0.9;
}

//get the intersection of the ground and a vector
Vec3 Ground::getIntersection(Vec3 origin, Vec3 direction){
    Vec3 normal = Vec3(0.0,1.0,0);
    double a = direction.dotProduct(normal);
    
    //if ray is parallel to the plane
    if (a ==0){
        return Vec3();
    }
    else{
        double b = normal.dotProduct(origin.add(normal.multiply(40).invert()));
        return origin.add(direction.multiply(-b/a));

    }
    
}

//get the normal!
Vec3 Ground::getNormal(Vec3 intersection){
    return intersection.subtract(position).normalize();
}

