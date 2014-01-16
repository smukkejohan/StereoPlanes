//
//  voronoiWall.h
//  StereoPlanes
//
//  Created by Johan Bichel Lindegaard on 15/01/14.
//
//

#pragma once

#include "ofMain.h"
#include "ofxVoronoi.h"
#include "Utils.h"

class VoronoiWall {

public:
    void setup(ofParameterGroup * params);
    void draw();
    void update();
    void genTheVoronoi();
    
    //ofParameterGroup * parameters;
    
    ofParameter<float> shivering;
    ofParameter<int> subdivisions;
    ofParameter<float> wallSpeed;
    
    ofParameter<ofVec3f> wallBreakPos;
    ofParameter<ofVec3f> wallBreakReach;
    ofParameter<float> wallBreakStrength;
    
    
    vector <ofVec3f> vpts;
    ofRectangle      vbounds;
    ofxVoronoi       voronoi;
    
    float wallTime = 0;
    void drawVoronoiWall3d();
    
    
    
    
};