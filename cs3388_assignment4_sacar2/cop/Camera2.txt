//
//  Camera.hpp
//  cs3388_assignment4_sacar2
//
//  Created by selin acar on 2016-12-09.
//  Copyright © 2016 selin acar. All rights reserved.
//

#ifndef Camera_hpp
#define Camera_hpp

#include <stdio.h>
#include "vec3.hpp"
#include <vector>
#include "SceneObject.hpp"

class Camera{
public:
    //Camera is always looking at origin = 0,0,0
    Vec3 direction;
    Vec3 up;
    Vec3 right;
    Vec3 down;
    
    //camera constructors
    Camera(Vec3 direction, Vec3 up);
    Camera(Vec3 direction);
    
    void draw(std::vector<std::shared_ptr<SceneObject>>, Vec3 light);
    
};

#endif /* Camera_hpp */
