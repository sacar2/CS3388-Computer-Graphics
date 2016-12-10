//
//  Vec3.cpp
//  cs3388_assignment4_sacar2
//
//  Created by selin acar on 2016-12-09.
//  Copyright © 2016 selin acar. All rights reserved.
//

#include "Vec3.hpp"
#include "Math.h"


Vec3::Vec3(){
    x = y = z = 0;
}

Vec3::Vec3(double x_coordinate, double y_coordinate, double z_coordinate){
    x = x_coordinate;
    y = y_coordinate;
    z = z_coordinate;
}

// calculate magnitude of 3D vector
double Vec3::magnitude(){
    //fabs to get absolute of the floating point value
    return fabs(sqrt(x*x + y*y + z*z));
}

Vec3 Vec3::multiply(double scale){
    return Vec3(scale*x, scale*y, scale*z);
}

Vec3 Vec3::invert(){
    return multiply(-1);
}

Vec3 Vec3::add(Vec3 vector){
    return Vec3(vector.x+x, vector.y+y, vector.z+z);
}
Vec3 Vec3::subtract(Vec3 vector){
    return add(vector.invert());
}

Vec3 Vec3::normalize(){
    return multiply(1.0/magnitude());
}

//for when trying to find distance between points
double Vec3::calcDistance(Vec3 vector){
    return subtract(vector).magnitude();
}

double Vec3::dotProduct(Vec3 vector){
    return x*vector.x + y*vector.y + z*vector.z;
}

Vec3 Vec3::crossProduct(Vec3 vector){
    return Vec3(y*vector.z - z*vector.y, z*vector.x - x*vector.z, x*vector.y - y*vector.x );
}

bool Vec3::isEmpty(){
    return x==0 && y==0 && z==0;
}
