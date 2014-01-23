//
//  Voro3D.h
//  StereoPlanes
//
//  Created by Johan Bichel Lindegaard on 21/01/14.
//
//

#pragma once
#include "ofMain.h"
#include "ContentScene.h"
#include "VoroUtils.h"

class Voro3D : public ContentScene {
public:
    
    void setup();
    void update();
    void draw(int _surfaceId);
    
    void setGui(ofxUICanvas * gui, float width);
    void guiEvent(ofxUIEventArgs &e);
    void receiveOsc(ofxOscMessage * m, string rest);
    
    
    vector<ofPoint> cellCentroids;
    vector<float>   cellRadius;
    vector<ofMesh>  cellMeshes;
    
    ofLight light;
    ofLight dirLight;
    
    
    VoroCube * wallCube;
    VoroCube * floorCube;
    VoroCube * extraCube;
    
    
};