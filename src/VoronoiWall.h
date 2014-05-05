//
//  voronoiWall.h
//  StereoPlanes
//
//  Created by Johan Bichel Lindegaard on 15/01/14.
//
//

#pragma once

#include "ofMain.h"
#include "ContentScene.h"
#include "VoroUtils.h"
#include "ofxOlaShaderLight.h"


class BreakZone {
    
public:
    BreakZone(){};
    
    ofVec3f pos;
    float radius;
    float strength;
    float noise;
    
    float time = 0;
    float speed;
    
    ofxTLCurves * x;
    ofxTLCurves * y;
    
    ofxTLCurves * tlstrength;
    ofxTLCurves * tlnoise;
    ofxTLCurves * tlradius;
    ofxTLCurves * tlpush;
    ofxTLCurves * tlrotate;
    
    ofxTLCurves * tlspeed;
    
    ofxTLCurves * tlalpha;
    //ofxTLCurves * tlshininess;
    ofxTimeline * tl;
    
    void setup(ofxTimeline * _tl) {
        tl = _tl;
        
        x = tl->addCurves("X");
        x->setValueRangeMax(2);
        x->setValueRangeMin(-2);
        
        y = tl->addCurves("Y");
        y->setValueRangeMax(2);
        y->setValueRangeMin(-2);
        
        tlstrength = tl->addCurves("Strength");
        tlstrength->setValueRangeMax(6);
        tlstrength->setValueRangeMin(-2);
        
        tlnoise = tl->addCurves("Noise");
        tlnoise->setValueRangeMax(2);
        tlnoise->setValueRangeMin(0);
        
        tlspeed = tl->addCurves("Speed");
        tlspeed->setValueRangeMax(2);
        tlspeed->setValueRangeMin(0);
        
        tlradius = tl->addCurves("Radius");
        tlradius->setValueRangeMax(4);
        tlradius->setValueRangeMin(0);
        
        tlpush = tl->addCurves("Push");
        tlrotate = tl->addCurves("Rotate");
        tlalpha = tl->addCurves("Alpha");
        //tlshininess = tl->addCurves("Shine");
    }
    
    void update() {
        
        //if(tl->getIsPlaying()) {
            time += 0.01 * speed;
        //}
        
        pos.x = x->getValue();
        pos.y = y->getValue();
        
        strength = tlstrength->getValue();
        radius = tlradius->getValue();
        noise = tlnoise->getValue();
        speed = tlspeed->getValue();
        
    };
    
    void debugDraw() {
        
        
    };
    
};

class VoronoiPlane {
    
public:
    
    ofRectangle bounds;
    float depth = 0.005;
    vector<ofVboMesh>  cellMeshes;
    int nCells = 20;
    vector<Cell> cells;
    float fade = 1;
    
    ofxTimeline * tl;
    
    vector<ofPoint> controlPoints;
    vector<BreakZone *> breakZones;
    string name;
    
    void draw();
    
    void setup(ofRectangle _bounds, ofxTimeline * _tl, string _name) {
        tl = _tl;
        bounds = _bounds;
        name = _name;
        
        for(int i=0; i <3; i++) {
            BreakZone * br = new BreakZone;
            tl->addPage(name + ofToString(i));
            br->setup(tl);
            breakZones.push_back(br);
        }
        
        generate();
    }
    
    void update() {
        
        for(int b = 0; b<breakZones.size(); b++) {
            
            breakZones[b]->update();
        }
        
        if(nCells != cells.size()) {
            generate();
        }
        
    }
    
    void addPoint(ofPoint) {
    }
    
    void generate() {
        
        voro::container con(-bounds.width,bounds.width,
                            -bounds.height,bounds.height,
                            -depth,depth,
                            1,1,1,
                            false,false,false, // set true to flow beyond box
                            8);
        
        while(controlPoints.size() > nCells) {
            controlPoints.pop_back();
        }
        
        while(controlPoints.size() < nCells) {
            controlPoints.push_back(ofPoint(ofRandom(-bounds.width,bounds.width),
                                            ofRandom(-bounds.height,bounds.height),
                                            ofRandom(-depth,depth)));
        }
        
        addCellsSeeds(con, controlPoints);
        
        cellMeshes = getCellsFromContainer(con, 0);
        
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
    
    ofRectangle  vbounds;
    float depth;
    
    ofBoxPrimitive bounds;
    
    float wallTime = 0;
    void drawVoronoiWall3d();
    
    vector<BreakZone> breakZones;
    vector<Cell> cells;
    
    ofxOlaShaderLight::Material mat;
    
    vector<ofMesh>  cellMeshes;
    
    float darksolid;
    float rotation;
    float floor;
    
    VoronoiPlane * voroWall;
    VoronoiPlane * voroWallRight;
    VoronoiPlane * voroFloor;
    
    
    //ofLight pointlight;
    
};