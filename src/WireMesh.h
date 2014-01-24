//
//  WireMesh.h
//  StereoPlanes
//
//  Created by Maria Paula Saba on 1/17/14.
//
//

#pragma once

#include "ofMain.h"
#include "ContentScene.h"

class WireMesh : public ContentScene {
    
public:
    void setup();
    void draw( int _surfaceId );
    void update();
    void createMesh();
    void resetMesh();
    void createVertex();
    void setGui(ofxUICanvas * gui, float width);
    void receiveOsc(ofxOscMessage * m, string rest);
    
    bool reset, createVert;
    float sliderspeed, speed, numVerts, threshold, bgColor, shadowColor, shadowX, shadowY, lineWidth, whiteMesh, blackMesh;
    
    ofVec3f offset;
    ofMesh mesh;
    vector<ofColor> colors;
    
    int mySurface;
    bool changeSurface;
    
    ofVec2f dancerPos;
    
    
    
    bool shadInc, shadDec, wCloser, wFurther, bCloser, bFurther, speedInc, speedDec, lightBg, darkBg;
    
    
    void shadowIncrease();
    void shadowDecrease();
    void whiteCloser();
    void whiteFurther();
    void blackCloser();
    void blackFurther();
    void lightenBackground();
    void darkenBackground();
    void speedDecrease();
    void speedIncrease();
    
};