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

#include "ofxOlaShaderLight.h"

class VoroGhost {
public:
    
    float width;
    float height;
    float depth;
    
    vector<ofVboMesh>  cellMeshes;
    int nCells;
    vector<Cell> cells;
    
    void setup(float _w = 0.2, float _h = 0.2, float _d = 0.2, int _c = 40) {
        width  = _w;
        height = _h;
        depth  = _d;
        nCells = _c;
        
        generate();
    }
    
    void update() {
        if(nCells != cells.size()) {
            generate();
        }
    }
    
    void generate() {
        
        voro::container con(-width,width,
                            -height,height,
                            -depth,depth,
                            1,1,1,
                            true,true,true, // set true to flow beyond box
                            8);
        
        for(int i = 0; i < nCells;i++){
            ofPoint newCell = ofPoint(ofRandom(-width,width),
                                      ofRandom(-height,height),
                                      ofRandom(-depth,depth));
            
            addCellSeed(con, newCell, i, true);
        }
        
        cellMeshes = getCellsFromContainer(con, 0);
        
        
        cells.clear();
        for (int i=0; i < cellMeshes.size(); i++) {
            
            Cell cell;
            cell.mesh.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
            
            cell.offset = ofVec3f(0,0,0);
            cell.r = ofRandom(-1.0,1.0);
            cell.mesh = cellMeshes[i];
            
            cells.push_back(cell);
        }
    }
    
};


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
    
    VoroGhost * wallCube;
    VoroGhost * floorCube;
    VoroGhost * extraCube;
    
    ofxOlaShaderLight::Material mat;
    
    
    
    
};