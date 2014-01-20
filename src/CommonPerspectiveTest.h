//
//  CommonPerspectiveTest.h
//  StereoPlanes
//
//  Created by Johan Bichel Lindegaard on 20/01/14.
//
//

#pragma once
#include "ofMain.h"

class CommonPerspectiveTest {
    
public:
    void drawFloor();
    void drawWall();
    void setup(ofParameterGroup * params);
    void draw();
    void update();
    ofBoxPrimitive box;
    
    ofTexture tex;
    ofImage ofLogo;
    ofLight light;
    
};