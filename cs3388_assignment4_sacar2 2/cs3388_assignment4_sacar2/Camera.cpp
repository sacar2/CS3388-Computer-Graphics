//
//  Camera.cpp
//  cs3388_assignment4_sacar2
//
//  Created by selin acar on 2016-12-09.
//  Copyright © 2016 selin acar. All rights reserved.
//

#include "Camera.hpp"
#include "Constants.h"
#include <GLUT/glut.h>
#include "Math.h"


/*  Draw using rya casting algorithm:
    This method draws the scene objects that are projected into the viewing rectangle
    It goes through each pizel on the screen and check if the viewer wil be able to see the object
    if the viewer can see the object, you check if */
void Camera::draw(std::vector<std::shared_ptr<SceneObject> > scene, Vec3 light){
    for (int x = 0 ; x<Constants::WINDOW_WIDTH ; x++){
        for (int y = 0 ; y<Constants::WINDOW_HEIGHT ; y++){
            //find position of pixel, also scale the camera 40 units away from origin
            Vec3 pixelPos = direction.invert().multiply(40).add(right.multiply((x - Constants::WINDOW_WIDTH/2)/10.0)).add(down.multiply((y- Constants::WINDOW_HEIGHT/2)/10.0));
            std::shared_ptr<SceneObject> closestObject;
            Vec3 closest = Vec3();
            for (int i = 0; i<scene.size(); i++) {
                //check if pixel direction intersects with a scene object!
                //find the object that intersects, which is closest
                //position of intersection of pixel and object
                Vec3 intersection = scene[i]->getIntersection(pixelPos, direction);
                if (intersection.isEmpty()){
                    continue;
                }
                if (closest.isEmpty() || closest.calcDistance(pixelPos)>intersection.calcDistance(pixelPos)){
                    closest = intersection;
                    closestObject = scene[i];
                }
            }
            if( closest.isEmpty()){
                //draw the current pixel in black
                glColor3f(0, 0, 0);
                glVertex2d(x, y);
                continue;
            }
            //if there is a closest object.. an object that is actually drawn, then find the colours and draw it at the pixel position!
            //get normal of object at the point of the intersection
            Vec3 normal = closestObject->getNormal(closest);
            Vec3 lightNormal = light.subtract(normal).normalize();
            
            //Diffuse: calculate light factors - shadows first - will calculate specular later
            double lightingFactor = (closestObject->ambient + (-normal.dotProduct(lightNormal))*closestObject->diffuse);
            
            //figure out shadows
            bool hasShadow = false;
            //direction we're looking at - the light pos
            Vec3 lightVector = closest.subtract(light).normalize();
            for (int i = 0; i<scene.size(); i++) {
                if( scene[i] == closestObject){
                    continue;
                }
                //now look for intersection
                Vec3 intersection = scene[i]->getIntersection(light, lightVector);
                if (!intersection.isEmpty() && (intersection.calcDistance(light) < closest.calcDistance(light))){
                    //if it enters this statement, there is an intersection and there should be a shadow
                    hasShadow = true;
                    break;
                }
            }
            if(hasShadow){
                glColor3f(closestObject->r*closestObject->ambient,closestObject->g*closestObject->ambient,closestObject->b*closestObject->ambient);
                glVertex2d(x, y);
                continue;
            }
            Vec3 halfway = lightVector.add(closest.subtract(pixelPos).normalize()).normalize();
            double highLightFactor = halfway.dotProduct(normal);
            highLightFactor = pow(highLightFactor, 18) * closestObject->specular;
            glColor3f(closestObject->r*lightingFactor + highLightFactor,closestObject->g*lightingFactor + highLightFactor,closestObject->b*lightingFactor + highLightFactor);
            glVertex2d(x, y);
        }
    }
}

/* Camera Constructor */
Camera::Camera(Vec3 dir, Vec3 up_vec){
    //want to make sure that direction is normalized
    direction = dir.normalize();
    
    up = up_vec;
    //define camera right and down
    right = up.crossProduct(direction).normalize();
    down = right.crossProduct(direction).normalize();
}

/*another camera constructor, with default up_vector */
Camera::Camera(Vec3 dir){
    Camera(dir, Vec3(0,1.0,0));
}
