//
//  lines.h
//  StereoPlanes
//
//  Created by Ole Kristensen on 17/01/14.
//
//

#pragma once

#include "ofMain.h"

class Lines {
    
public:
    
    void setup(ofParameterGroup * params);
    void update();
    void draw();
    
    ofParameter<float> angle;
    ofParameter<float> width;
    ofParameter<float> distance;
    ofParameter<float> randomWidth;
    ofParameter<ofVec3f> origin;
    ofParameter<float> randomDepth;
};