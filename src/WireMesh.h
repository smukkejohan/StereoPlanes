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
    
    bool reset, createVert;
    float speed, numVerts, threshold, bgColor, shadowSize, lineWidth,
    whiteMesh, blackMesh;
    
    ofVec3f offset;
    ofMesh mesh;
    vector<ofColor> colors;
    
    int mySurface;
    bool changeSurface;
    
    ofVec2f dancerPos;
};