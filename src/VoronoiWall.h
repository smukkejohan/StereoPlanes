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
    
    float multiplier;
    
    ofxTLCurves * x;
    ofxTLCurves * y;
    
    ofxTLCurves * tlmultiplier;
    ofxTLCurves * tlstrength;
    ofxTLCurves * tlnoise;
    ofxTLCurves * tlradius;
    ofxTLCurves * tlpush;
    ofxTLCurves * tlrotate;
    
    ofxTLCurves * tlspeed;
    
    ofxTLCurves * tltransformx;
    ofxTLCurves * tltransformy;
    
    
    ofxTLCurves * tldifalpha;
    ofxTLCurves * tlspecalpha;
    
    ofxTLCurves * tlreset;
    
    //ofxTLCurves * tlshininess;
    ofxTimeline * tl;
    
    void setup(ofxTimeline * _tl) {
        tl = _tl;
        
        x = tl->addCurves("X");
        x->setValueRangeMax(5);
        x->setValueRangeMin(-5);
        
        y = tl->addCurves("Y");
        y->setValueRangeMax(5);
        y->setValueRangeMin(-5);
        
        tlstrength = tl->addCurves("Strength");
        tlstrength->setValueRangeMax(6);
        tlstrength->setValueRangeMin(-1);
        
        tlmultiplier = tl->addCurves("Multiplier");
        tlmultiplier->setValueRangeMax(10);
        tlmultiplier->setValueRangeMin(0);
        
        tlnoise = tl->addCurves("Noise");
        tlnoise->setValueRangeMax(10);
        tlnoise->setValueRangeMin(0);
        
        tlspeed = tl->addCurves("Speed");
        tlspeed->setValueRangeMax(4);
        tlspeed->setValueRangeMin(0);
        
        tlradius = tl->addCurves("Radius");
        tlradius->setValueRangeMax(4);
        tlradius->setValueRangeMin(0);
        
        tlreset = tl->addCurves("Reset");
        
        /*tlpush = tl->addCurves("Push");*/
        /*tlrotate = tl->addCurves("Rotate");*/
        
        tldifalpha = tl->addCurves("Diffuse alpha");
        //tldifalpha->setValueRangeMin(-0.1);
        tlspecalpha = tl->addCurves("Specular alpha");
        //tlspecalpha->setValueRangeMin(-0.1);
        
        //tlshininess = tl->addCurves("Shine");
    }
    
    void update() {
        
        pos.x = x->getValue();
        pos.y = y->getValue();
        
        strength = tlstrength->getValue();
        radius = tlradius->getValue();
        noise = tlnoise->getValue();
        speed = tlspeed->getValue();
        multiplier = tlmultiplier->getValue();
        
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
    float gap = 0.0;
    vector<ofVboMesh>  cellMeshes;
    vector<ofVboMesh>  cellMeshesNoNormals;
    int nCells = 20;
    vector<Cell> cells;
    float fade = 1;
    
    string scenename;
    
    ofxTimeline * tl;
    ofxTLCurves * tlcells;
    ofxTLCurves * tlrotationy;
    
    ofxTLCurves * tlrotationx;
    
    ofxTLCurves * tlrotationfixy;
    
    ofxTLCurves * tlrotationfixx;
    
    ofxTLCurves * tldifalpha;
    ofxTLCurves * tlspecalpha;
    ofxTLCurves * tlshine;
    
    ofxTLCurves * tldepth;
    ofxTLCurves * tlnormals;
    
    ofxTLCurves * tltransformx;
    ofxTLCurves * tltransformy;
    
    int fixedPointsLength = 11;
    
    ofPoint fixedPoints [11] = {
        ofPoint(0.8,-0.12),
        ofPoint(-0.7,0.4),
        ofPoint(-0.4,0.4),
        ofPoint(0.85,-0.4),
        ofPoint(-0.5,-0.2),
        ofPoint(0.9,-0.8),
        ofPoint(1.0,0.6),
        ofPoint(-1.5,0.5),
        ofPoint(0.5,0.8),
        ofPoint(1.0,-1.0),
        ofPoint(0.9,-0.2) };

    
    ofxTLCurves * tlbackalphamax;
    
    vector<ofPoint> controlPoints;
    vector<BreakZone *> breakZones;
    string name;
    
    void draw();
    
    void setup(ofRectangle _bounds, ofxTimeline * _tl, string _name) {
        tl = _tl;
        bounds = _bounds;
        name = _name;
        
        tlcells = tl->addCurves("Cells");
        
        if(scenename == "voroFloor") {
            tlcells->setValueRangeMax(800);
        } else {
            tlcells->setValueRangeMax(400);
        }
        
        tlcells->setValueRangeMin(1);
        
        tldifalpha = tl->addCurves("Diffuse alpha");
        tlspecalpha = tl->addCurves("Specular alpha");
        tlshine = tl->addCurves("Shine");
        tlshine->setValueRangeMax(20);
        
        //tldifalpha->setValueRangeMax(-180);
        //tldifalpha->setValueRangeMin(0);
        
        tlrotationy = tl->addCurves("Rotate y");
        tlrotationy->setValueRangeMax(110);
        tlrotationy->setValueRangeMin(-110);
        
        tlrotationfixy = tl->addCurves("Rotate around Y");
        tlrotationfixy->setValueRangeMax(1);
        tlrotationfixy->setValueRangeMin(-1);
        
        tlrotationx = tl->addCurves("Rotate x");
        tlrotationx->setValueRangeMax(110);
        tlrotationx->setValueRangeMin(-110);
        
        tlrotationfixx = tl->addCurves("Rotate around x");
        tlrotationfixx->setValueRangeMax(1);
        tlrotationfixx->setValueRangeMin(-1);
        
        tlbackalphamax = tl->addCurves("Fade out in distance");
        tlbackalphamax->setValueRangeMax(8);
        
        tldepth = tl->addCurves("Depth");
        tldepth->setValueRangeMax(10);
        tldepth->setValueRangeMin(1);
        
        tlnormals = tl->addCurves("Normals");
        
        tltransformx = tl->addCurves("Transform x");
        tltransformx->setValueRangeMax(5);
        tltransformx->setValueRangeMin(-5);
        
        tltransformy = tl->addCurves("Transform y");
        tltransformy->setValueRangeMax(5);
        tltransformy->setValueRangeMin(-5);
        
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
        
        if(controlPoints.size() == 0) {
            controlPoints.assign(fixedPoints,fixedPoints+fixedPointsLength);
        }
        
        while(cells.size() > nCells) {
            cells.pop_back();
        }
        
        while(controlPoints.size() < nCells) {
            
            controlPoints.push_back(ofPoint(ofRandom(-bounds.width,bounds.width),
                                            ofRandom(-bounds.height,bounds.height),
                                            ofRandom(-depth,depth)));
        }
        
        vector<ofPoint> ourControlPoints;
        
        //ourControlPoints.assign(controlPoints.begin(), controlPoints.begin()+nCells);
        ourControlPoints = vector<ofPoint>(controlPoints.begin(), controlPoints.begin()+nCells);
        
        addCellsSeeds(con, ourControlPoints);
        
        cellMeshes = getCellsFromContainer(con, gap);
        
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
            
            if(tlnormals->getValue() == 0) {
                cells[i].mesh.clearNormals();
            }
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