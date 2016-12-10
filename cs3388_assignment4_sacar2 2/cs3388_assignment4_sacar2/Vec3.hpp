//
//  Vec3.hpp
//  cs3388_assignment4_sacar2
//
//  Created by selin acar on 2016-12-09.
//  Copyright © 2016 selin acar. All rights reserved.
//

#ifndef Vec3_hpp
#define Vec3_hpp

#include <stdio.h>
class Vec3{
public:
    double x;
    double y;
    double z;
    Vec3(double x, double y, double z);
    Vec3();
    double magnitude();
    Vec3 multiply(double);
    Vec3 invert();
    Vec3 add(Vec3);
    Vec3 subtract(Vec3);
    Vec3 normalize();
    
    //for when trying to find distance between points
    double calcDistance(Vec3);
    
    double dotProduct(Vec3);
    Vec3 crossProduct(Vec3);
    
    bool isEmpty();
};
#endif /* Vec3_hpp */
