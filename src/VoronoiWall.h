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

struct Cell {
    ofMesh mesh;
    ofVec3f offset;
    ofColor color;
    float r = 0;
};

class VoronoiPlane {
    
public:
    
    ofRectangle bounds;
    float depth = 0.005;
    vector<ofMesh>  cellMeshes;
    int nCells = 20;
    vector<Cell> cells;
    
    float fade = 1;
    
    void setup(ofRectangle _bounds) {
        bounds = _bounds;
        generate();
    }
    
    void update() {

        if(nCells != cells.size()) {
                generate();
        }
    }
    
    void generate() {
        
        voro::container con(-bounds.width,bounds.width,
                            -bounds.height,bounds.height,
                            -depth,depth,
                            1,1,1,
                            false,false,false, // set true to flow beyond box
                            8);
        
        for(int i = 0; i < nCells;i++){
            ofPoint newCell = ofPoint(ofRandom(-bounds.width,bounds.width),
                                      ofRandom(-bounds.height,bounds.height),
                                      ofRandom(-depth,depth));
            
            addCellSeed(con, newCell, i, true);
        }
        
        cellMeshes = getCellsFromContainer(con);
        
        cells.clear();
        
        for (int i=0; i < cellMeshes.size(); i++) {
            
            Cell cell;
            cell.mesh.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
            
            cell.offset = ofVec3f(0,0,0);
            int r = ofRandom(0,255);
            cell.r = ofRandom(-1.0,1.0);
            cell.mesh = cellMeshes[i];
            
            cells.push_back(cell);
        }
    }
    
};


struct BreakPoint {
    ofVec3f pos;
    float radius;
    float pressure;
};



class VoronoiWall : public ContentScene {

public:
    void setup();
    void draw(int _surfaceId);
    void update();
    
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
    
    ofRectangle      vbounds;
    float depth;
    
    ofBoxPrimitive bounds;
    
    float wallTime = 0;
    void drawVoronoiWall3d();
    
    vector<BreakPoint> breakPoints;
    vector<Cell> cells;
    
    ofLight light;
    ofLight dirLight;
    vector<ofMesh>  cellMeshes;
    
    float darksolid;
    float rotation;
    float floor;
    
    VoronoiPlane * voroWall;
    VoronoiPlane * voroFloor;
    
};