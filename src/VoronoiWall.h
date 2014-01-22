//
//  voronoiWall.h
//  StereoPlanes
//
//  Created by Johan Bichel Lindegaard on 15/01/14.
//
//

#pragma once

#include "ofMain.h"
//#include "ofxVoronoi.h"
//#include "Utils.h"
#include "ofxVoro.h"
#include "ContentScene.h"

struct BreakPoint {
    ofVec3f pos;
    float radius;
    float pressure;
};

struct Cell {
    ofMesh mesh;
    ofVec3f offset;
    ofColor color;
};

class VoronoiWall : public ContentScene {

public:
    void setup();
    void draw(int _surfaceId);
    void update();
    void genTheVoronoi();
    
    void setGui(ofxUICanvas * gui, float width);
    void guiEvent(ofxUIEventArgs &e);
    
    void receiveOsc(ofxOscMessage * m, string rest);
    
    ofVec3f wallBreakPos;
    ofVec3f wallBreakReach;
    
    float wallBreakStrength;
    float breakPointRadius;
    float wallSpeed;
    
    float nCells;
    bool autoOn = true;
    
    vector <ofVec3f> vpts;
    ofRectangle      vbounds;
    float depth;
    
    ofBoxPrimitive bounds;
    
    float wallTime = 0;
    void drawVoronoiWall3d();
    void updateCells();
    
    vector<BreakPoint> breakPoints;
    vector<Cell> cells;
        
    vector<ofMesh>  cellMeshes;
    
    ofLight light;
    ofLight dirLight;

    
    
};