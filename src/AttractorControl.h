//
//  attractorControl.h
//  StereoPlanes
//
//  Created by Bernardo Schorr on 1/17/14.
//
//

#pragma once

#include "ofMain.h"
#include "ContentScene.h"
#include "AttractorParticle.h"

class AttractorControl : public ContentScene {
public:
    void setup();
    void update();
    void draw( int _surfaceId );
    
    void setGui(ofxUICanvas * gui, float width);
    void guiEvent(ofxUIEventArgs &e);

    void receiveOsc(ofxOscMessage * m, string rest);
    
    float damping, acceleration, multiplier, bkgColor, bkgTransparency, particleColor, explosionSize;
    
    bool bCreateStructure, bHideDancer, bAngleMovement, bFreeze;
    
//    ofParameter<float> damping;
//    ofParameter<float> acceleration;
//    ofParameter<float> multiplier;
//    ofParameter<float> bkgColor;
//    ofParameter<float> bkgTransparency;
//    ofParameter<float> particleColor;
//
//    ofParameter<bool> explode;
//    ofParameter<bool> finalExplode;
//    ofParameter<bool> angleMovement;
//    ofParameter<bool> invertColors;
//    ofParameter<bool> showAttractor;
//    ofParameter<bool> freeze;
    
    vector<AttractorParticle>   particleList;
    ofVec3f attractorPos;
    
    int explosionCountdown;
    int finalExplosionCountdown;
    
    ofVec2f dancerPos;
    
    ofLight localLight;
    
    bool started = false;
    
};