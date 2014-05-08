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

class GhostLights : public ContentScene {
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
    
    
    ofxTLCurves * tlexplode;
    float explode;
    
    VoroCube * wallCube;
    VoroCube * floorCube;
    VoroCube * extraCube;
    
    ofxOlaShaderLight::Material mat;
    
    
    
    
};