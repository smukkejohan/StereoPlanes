//
//  CeilingPlane.h
//  StereoPlanes
//
//  Created by Johan Bichel Lindegaard on 16/01/14.
//
//

#pragma once

#include "ofMain.h"

class CeilingPlane {
    
public:
    void setup(ofParameterGroup * params);
    void update();
    void draw(ofFbo * fbo);
    
    // plane on top shift height
    
    ofParameter<ofVec3f> position;
    ofParameter<ofVec3f> dimensions;
    ofParameter<ofVec3f> rotation;
};