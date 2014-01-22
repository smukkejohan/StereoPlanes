//
//  BoxWall.h
//  StereoPlanes
//
//  Created by Bernardo Schorr on 1/16/14.
//
//

#pragma once
#include "ContentScene.h"
#include "ofMain.h"

class BoxFloor : public ContentScene {
    
public:
    void setup();
    void update();
    void draw( int _surfaceId );
    void setGui(ofxUICanvas * gui, float width);
    
    //ofParameterGroup * parameters;
    
//    ofParameter<float> surfaceHeight;
//    ofParameter<float> waveHeight;
//    ofParameter<float> speed;
//    ofParameter<float> plateauHeight;
//    ofParameter<float> lightHeight;
//    ofParameter<float> distance;
//    ofParameter<bool> bRotation;
    
    float surfaceHeight, waveHeight, speed, plateauHeight, lightHeight, platformRadius;
    bool bRotation;
    
    ofLight boxLight;
    
    vector <float> rotation;
    
    ofVec2f dancerPos;

    
};