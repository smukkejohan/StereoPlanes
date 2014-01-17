//
//  lines.cpp
//  StereoPlanes
//
//  Created by Ole Kristensen on 17/01/14.
//
//

#include "Lines.h"


void Lines::setup(ofParameterGroup * params) {
    
    params->add(angle.set("lines angle", 0, 0, 90));
    params->add(width.set("lines width", 2, 0, 100));
    params->add(randomWidth.set("lines rand width", 0, 0, 90));
    params->add(randomWidth.set("lines rand width", 0, 0, 90));
    
}

void Lines::update() {
    
    
}

void Lines::draw() {
    
    glPushMatrix();
    
    glPopMatrix();
    
}