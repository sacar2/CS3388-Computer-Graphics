//
//  Ground.hpp
//  cs3388_assignment4_sacar2
//
//  Created by selin acar on 2016-12-09.
//  Copyright © 2016 selin acar. All rights reserved.
//

#ifndef Ground_hpp
#define Ground_hpp

#include <stdio.h>
#include "Vec3.hpp"
#include "SceneObject.hpp"

class Ground :public SceneObject{
private:
    double width;
    double height;
    Vec3 position;
public:
    Ground(double r, double g, double b, Vec3 pos);
    Vec3 getIntersection(Vec3 origin, Vec3 direction);
    Vec3 getNormal(Vec3);
};

#endif /* Ground_hpp */
