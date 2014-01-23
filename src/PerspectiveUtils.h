//
//  CommonPerspectiveTest.h
//  StereoPlanes
//
//  Created by Johan Bichel Lindegaard on 20/01/14.
//
//

#pragma once
#include "ofMain.h"

class PerspectiveUtils {
    
public:
    
    void transformWall() {
        ofTranslate(0,1);
    }
    
    void transformFloor() {
        ofTranslate(0,-1);
        ofRotate(90, 1, 0, 0);
    }
    
};