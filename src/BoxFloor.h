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
    void guiEvent(ofxUIEventArgs &e);
    
    void receiveOsc(ofxOscMessage * m, string rest);

    float surfaceHeight, waveHeight, speed, plateauHeight, lightHeight, platformRadius;
    bool bRotation;
    
    ofLight boxLight;
    
    vector <float> rotation;
    
    ofVec2f dancerPos;

    
};