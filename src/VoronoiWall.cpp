//
//  VoronoiWall.cpp
//  StereoPlanes
//
//  Created by Johan Bichel Lindegaard on 15/01/14.
//
//

// TODO:
// Knock wall over
// set to floor
// Set solid space
// break at points
// Create dark space
// Increase decrease number of cells


#include "VoronoiWall.h"

void VoronoiWall::setup() {
    
    name = "Voronoi Wall";
    oscAddress = "/voronoiwall";
    // Voronoi wall
    
    vbounds.set(-1, 0, 1, 1);
    depth = 0.005;
    
    nCells = 40;
    
    light.setPosition(2, 2, -2);
    light.setSpotlight();
    light.lookAt(ofVec3f(0,0,0));
    light.setDiffuseColor(ofColor(245,245,170));
    light.setAmbientColor(ofColor(200,204,200));
    light.setSpecularColor(ofColor::white);
    
    dirLight.setSpotlight();
    dirLight.setPosition(-1, -1, 1);
    dirLight.lookAt(ofVec3f(0,0,0));
    dirLight.setDiffuseColor(ofColor(191,191,191));
    
    genTheVoronoi();
    
    for(int i=0; i<5; i++) {
        BreakPoint br;
        br.pos = ofVec3f(0,0,0);
        br.radius = 0.25;
        br.pressure = 0;
        breakPoints.push_back(br);
    }

}

void VoronoiWall::setGui(ofxUICanvas * gui, float width){
    ContentScene::setGui(gui, width);
    
    gui->addToggle(indexStr+"Animate", &autoOn);
    gui->addSlider(indexStr+"Break strength", 0, 1.8, &wallBreakStrength);
    gui->addSlider(indexStr+"Speed", 0, 2, &wallSpeed);
    
    gui->addSlider(indexStr+"Cells", 0, 200, &nCells);
    
    gui->addSlider(indexStr+"Solid pos x", -2, 2, &wallBreakPos.x);
    gui->addSlider(indexStr+"Solid pos y", -2, 2, &wallBreakPos.y);
    
    gui->addSlider(indexStr+"Solid width", 0, 4, &wallBreakReach.x);
    gui->addSlider(indexStr+"Solid height", 0, 4, &wallBreakReach.y);
    
    gui->addSlider(indexStr+"Dark solid", 0, 1, &darksolid);
}


void VoronoiWall::draw(int _surfaceId) {
    
    if(_surfaceId == 1) {
        
        light.enable();
        dirLight.enable();
        
        ofRectangle bounds = ofRectangle(wallBreakPos.x-wallBreakReach.x/2, wallBreakPos.y-wallBreakReach.y/2, wallBreakReach.x, wallBreakReach.y);
        
        
        for(int i=0; i < cells.size(); i++) {
            
            bool insideSolid = false;
            if(!bounds.inside(cells[i].mesh.getCentroid())) {
                
                if(autoOn) {
                    cells[i].offset.z = ofSignedNoise(wallTime + i) * wallBreakStrength;
                }
                
                bool inBreakpoint = false;
                for(int b=0; b<breakPoints.size(); b++) {
                    
                    //todo: break more with distance
                    
                    if(breakPoints[b].pos.distance(cells[i].mesh.getCentroid()) < breakPoints[b].radius) {
                        cells[i].offset.z = ofMap(breakPoints[b].pos.distance(cells[i].mesh.getCentroid()), 0, breakPoints[b].radius, breakPoints[b].pressure, 0);
                        inBreakpoint = true;
                    }
                }
                
                if(!inBreakpoint) {
                    cells[i].offset.z * 0.6978;
                }
                
            } else {
                cells[i].offset.z = 0;
                insideSolid = true;
            }
            
            ofPushMatrix();
            ofTranslate(cells[i].offset);
            
            ofColor col = ofColor(ofMap(cells[i].offset.z, -0.4, 0.4, 255,100));
            col.a = 225;
            
            if(insideSolid) {
                col.a = ofMap(darksolid, 0, 1, 255,0);
            }
            ofSetColor(col);
            
            cells[i].mesh.draw();
            ofPopMatrix();
        }
        
        light.disable();
        dirLight.disable();

    }
    
    /*for(int b=0; b<breakPoints.size(); b++) {
        ofPushMatrix();
        ofTranslate(0, 0, -0.1);
        ofSetColor(0,0,0,20);
        //ofCircle(breakPoints[b].pos, breakPoints[b].radius);
        ofPopMatrix();
    }*/
    
}

void VoronoiWall::updateCells() {
    
    bool changed = false;
    if(nCells != cells.size()) {
        genTheVoronoi();
    }
    
}

void VoronoiWall::genTheVoronoi() {
    
    voro::container con(-vbounds.width,vbounds.width,
                        -vbounds.height,vbounds.height,
                        -depth,depth,
                        1,1,1,
                        false,false,false, // set true to flow beyond box
                        8);
    
    for(int i = 0; i < nCells;i++){
        ofPoint newCell = ofPoint(ofRandom(-vbounds.width,vbounds.width),
                                  ofRandom(-vbounds.height,vbounds.height),
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
        cell.mesh = cellMeshes[i];
        
        cells.push_back(cell);
    }
    

}

void VoronoiWall::update() {
    nCells = round(nCells);
    
    wallTime += 0.01 * wallSpeed;
    
    for(int b=0; b<breakPoints.size(); b++) {

        if(breakPoints[b].pressure > 6) {
            breakPoints[b].pressure = 6;
        }
           
        breakPoints[b].pressure *= 0.98;
        // radius ? ofMap(breakPoints[b].pressure, 0, 1, 0.2, 0.8);
        
    }
    
    updateCells();
}

void VoronoiWall::guiEvent(ofxUIEventArgs &e)
{
    
    string name = e.getName();
	int kind = e.getKind();
	//cout << "got event from: " << name << endl;
    
}

void VoronoiWall::receiveOsc(ofxOscMessage * m, string rest) {
    
    if(rest == "/speed/x" ) {
        wallSpeed = m->getArgAsFloat(0);
    } if(rest == "/br/x"){
         for(int i = 0; i < breakPoints.size(); i++) {
             breakPoints[i].pos.x = m->getArgAsFloat(i);
             breakPoints[i].pressure += 0.001;
         
         }
        
    } else if(rest == "/br/y"){
        
         for(int i = 0; i < breakPoints.size(); i++) {
         
             breakPoints[i].pos.y = m->getArgAsFloat(i);
             breakPoints[i].pressure += 0.001;
         
         }
    } else if(rest == "/solid/x"){
        wallBreakPos.x = m->getArgAsFloat(0);
    } else if(rest == "/solid/y"){
        wallBreakPos.y = m->getArgAsFloat(0);
    }
    
}
