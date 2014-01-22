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
    
    //ofParameterGroup * parameters;
    
    /*ofParameter<float> shivering;
    ofParameter<int> subdivisions;
    ofParameter<float> wallSpeed;
    
    ofParameter<ofVec3f> wallBreakPos;
    ofParameter<ofVec3f> wallBreakReach;
    ofParameter<float> wallBreakStrength;
    
    ofParameter<float> breakPointRadius;
    ofParameter<bool> autoOn;
    */
    
    ofVec3f wallBreakPos;
    ofVec3f wallBreakReach;
    
    float wallBreakStrength;
    float breakPointRadius;
    float wallSpeed;
    
    int nCells;
    bool autoOn = true;
    
    
    vector <ofVec3f> vpts;
    ofRectangle      vbounds;
    float depth;
    
    ofBoxPrimitive bounds;
    //ofxVoronoi       voronoi;
    
    float wallTime = 0;
    void drawVoronoiWall3d();
    void updateCells();
    
    vector<BreakPoint> breakPoints;
    vector<Cell> cells;
    
    bool active = true;
    
    vector<ofMesh>  cellMeshes;
    
    ofLight light;
    ofLight dirLight;

    
    
};