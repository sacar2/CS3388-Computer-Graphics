//
//  Sphere.hpp
//  cs3388_assignment4_sacar2
//
//  Created by selin acar on 2016-12-09.
//  Copyright © 2016 selin acar. All rights reserved.
//

#ifndef Sphere_hpp
#define Sphere_hpp

#include <stdio.h>
#include "Vec3.hpp"
#include "SceneObject.hpp"

class Sphere :public SceneObject{
private:
    double radius;
    Vec3 position;
public:
    Sphere(double r, double g, double b, Vec3 pos, double radius);
    Vec3 getIntersection(Vec3 origin, Vec3 direction);
    Vec3 getNormal(Vec3);
};

#endif /* Sphere_hpp */
