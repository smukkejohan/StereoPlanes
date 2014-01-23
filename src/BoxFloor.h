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
    
    float surfaceHeight, waveHeight, speed, plateauHeight, lightHeight, platformRadius;
    bool bRotation;
    
    ofLight boxLight;
    
    vector <float> rotation;
    
    ofVec2f dancerPos;

    
};