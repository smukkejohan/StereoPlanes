//
//  Ribbon.h
//  StereoPlanes
//
//  Created by Johan Bichel Lindegaard on 16/01/14.
//
//

#pragma once

#include "ofMain.h"

class Ribbon {
    
public:
    
    void setup(ofParameterGroup * params);
    void update();
    void draw();
    
    vector<ofVec3f> points;
    
    ofParameter<float> speed;
    ofParameter<ofVec3f> origin;
    ofVec3f lastPoint;
    
    ofParameter<ofVec2f> autoAmp;
    ofParameter<ofVec2f> autoSpeed;
    ofParameter<bool> autoOn;
    
};