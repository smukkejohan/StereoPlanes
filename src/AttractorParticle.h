//
//  attractorParticle.h
//  StereoPlanes
//
//  Created by Bernardo Schorr on 1/17/14.
//
//

#pragma once

#include "ofMain.h"

class AttractorParticle {
public:
    
    void setup( ofVec3f _pos, ofVec3f _vel );
    void update(float _damping, float _accelMult, ofVec3f attractorPos);
    void draw();
    
    ofVec3f     pos, prevPos;
    ofVec3f     velocity, accel;
    
    float damping;
    float accelMult;
    
    vector <ofVec3f> points;
    
};