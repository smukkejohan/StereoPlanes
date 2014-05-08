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
    
    ofxTLCurves * tldifalpha;
    ofxTLCurves * tlspecalpha;
    //ofxTLCurves * tlshininess;
    ofxTimeline * tl;
    
    void setup(ofxTimeline * _tl) {
        tl = _tl;
        
        x = tl->addCurves("X");
        x->setValueRangeMax(3);
        x->setValueRangeMin(-3);
        
        y = tl->addCurves("Y");
        y->setValueRangeMax(3);
        y->setValueRangeMin(-3);
        
        tlstrength = tl->addCurves("Strength");
        tlstrength->setValueRangeMax(6);
        tlstrength->setValueRangeMin(-1);
        
        tlnoise = tl->addCurves("Noise");
        tlnoise->setValueRangeMax(2);
        tlnoise->setValueRangeMin(0);
        
        tlspeed = tl->addCurves("Speed");
        tlspeed->setValueRangeMax(4);
        tlspeed->setValueRangeMin(0);
        
        tlradius = tl->addCurves("Radius");
        tlradius->setValueRangeMax(4);
        tlradius->setValueRangeMin(0);
        
        /*tlpush = tl->addCurves("Push");*/
        /*tlrotate = tl->addCurves("Rotate");*/
        
        tldifalpha = tl->addCurves("Specular Alpha");
        tlspecalpha = tl->addCurves("Diffuse Alpha");
        
        //tlshininess = tl->addCurves("Shine");
    }
    
    void update() {
        
        pos.x = x->getValue();
        pos.y = y->getValue();
        
        strength = tlstrength->getValue();
        radius = tlradius->getValue();
        noise = tlnoise->getValue();
        speed = tlspeed->getValue();
        
        time += 0.01 * speed;
    };
    
    void debugDraw() {
        
        
    };
    
};

class VoronoiPlane {
    
public:
    
    ofxOlaShaderLight::Material mat;
    
    ofRectangle bounds;
    float depth = 0.005;
    vector<ofVboMesh>  cellMeshes;
    int nCells = 20;
    vector<Cell> cells;
    float fade = 1;
    
    ofxTimeline * tl;
    ofxTLCurves * tlcells;
    ofxTLCurves * tlrotationy;
    ofxTLCurves * tlrotationfixy;
    
    ofxTLCurves * tldifalpha;
    ofxTLCurves * tlspecalpha;
    ofxTLCurves * tlshine;
    
    vector<ofPoint> controlPoints;
    vector<BreakZone *> breakZones;
    string name;
    
    void draw();
    
    void setup(ofRectangle _bounds, ofxTimeline * _tl, string _name) {
        tl = _tl;
        bounds = _bounds;
        name = _name;
        
        tlcells = tl->addCurves("Cells");
        tlcells->setValueRangeMax(600);
        tlcells->setValueRangeMin(1);
        
        tlrotationy = tl->addCurves("Rotate");
        tlrotationy->setValueRangeMax(-180);
        tlrotationy->setValueRangeMin(0);
        
        tldifalpha = tl->addCurves("Diffuse alpha");
        tlspecalpha = tl->addCurves("Specular alpha");
        tlshine = tl->addCurves("Shine");
        tlshine->setValueRangeMax(8);
        
        //tldifalpha->setValueRangeMax(-180);
        //tldifalpha->setValueRangeMin(0);
        
        
        tlrotationfixy = tl->addCurves("Rotate around Y");
        tlrotationfixy->setValueRangeMax(2);
        tlrotationfixy->setValueRangeMin(-2);
        

        
        for(int i=0; i <3; i++) {
            BreakZone * br = new BreakZone;
            tl->addPage(name + ofToString(i));
            br->setup(tl);
            breakZones.push_back(br);
        }
        
        generate();
    }
    
    void update() {
        
        
        mat.diffuseColor = ofVec4f(1.0, 1.0, 1.0, tldifalpha->getValue());
        mat.specularColor = ofVec4f(1.0, 1.0, 1.0, tlspecalpha->getValue());
        mat.specularShininess = tlshine->getValue();
        
        for(int b = 0; b<breakZones.size(); b++) {
            breakZones[b]->update();
        }
        
        nCells = round(tlcells->getValue());
        
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
            cells.pop_back();
        }
        
        while(controlPoints.size() < nCells) {
            controlPoints.push_back(ofPoint(ofRandom(-bounds.width,bounds.width),
                                            ofRandom(-bounds.height,bounds.height),
                                            ofRandom(-depth,depth)));
        }
        
        addCellsSeeds(con, controlPoints);
        
        cellMeshes = getCellsFromContainer(con, 0);
        
        //cells.clear();
        
        for (int i=cells.size(); cells.size() < cellMeshes.size(); i++) {
            Cell cell;
            cell.mesh.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
            
            cell.offset = ofVec3f(0,0,0);
            int r = ofRandom(0,255);
            cell.r = ofRandom(-1.0,1.0);
            
            cell.mat.diffuseColor = ofVec4f(1.0, 1.0, 1.0, 1.0);
            cell.mat.specularColor = ofVec4f(1.0, 1.0, 1.0, 1.0);
            cell.mat.specularShininess = 4.8;
            
            cells.push_back(cell);
        }
        
        for (int i=0; i < cellMeshes.size(); i++) {
            cells[i].mesh = cellMeshes[i];
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
    
    //ofLight pointlight;
    
};