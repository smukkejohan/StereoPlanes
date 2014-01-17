//
//  WireMesh.h
//  StereoPlanes
//
//  Created by Maria Paula Saba on 1/17/14.
//
//

#pragma once

#include "ofMain.h"

class WireMesh {
    
public:
    void setup(ofParameterGroup * params);
    void draw( ofVec3f off);
    void update( ofVec2f dancerPos );
    void createMesh();
    void resetMesh();
    void createVertex();
    //ofParameterGroup * parameters;


    ofParameter<bool> reset;
    ofParameter<float> speed;
    ofParameter<float> numVerts;
    ofParameter<float> threshold;
    ofParameter<ofVec3f> offset;
    ofParameter<float> bgColor;
    ofParameter<float> lineWidth;
    ofParameter<bool> triangles;
    ofParameter<float> dancerZ;
    ofParameter<bool> createVert;

    

    ofMesh mesh;
    vector<ofColor> colors;

};