//
//  SceneObject.hpp
//  cs3388_assignment4_sacar2
//
//  Created by selin acar on 2016-12-09.
//  Copyright © 2016 selin acar. All rights reserved.
//

#ifndef SceneObject_hpp
#define SceneObject_hpp

#include <stdio.h>
#include "Vec3.hpp"

//this class is being used for its virtual methods which will be implemented for  scene objects
class SceneObject{
public:
    //for colour
    double r;
    double g;
    double b;
    
    //for material pptys
    double ambient; //dark
    double diffuse; //colour
    double specular;//shiny

    virtual Vec3 getIntersection(Vec3 origin, Vec3 direction) = 0;
    virtual Vec3 getNormal(Vec3) = 0;
};

#endif /* SceneObject_hpp */
