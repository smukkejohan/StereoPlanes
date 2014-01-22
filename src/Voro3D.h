//
//  Voro3D.h
//  StereoPlanes
//
//  Created by Johan Bichel Lindegaard on 21/01/14.
//
//

#pragma once
#include "ofMain.h"
#include "ofxVoro.h"

class Voro3D {
public:
    
    void setup();
    void update();
    void draw();
    
    void makeTissue(int _nCells, float _width, float _height, float _deep);
    
    vector<ofPoint> cellCentroids;
    vector<float>   cellRadius;
    vector<ofMesh>  cellMeshes;
    
};