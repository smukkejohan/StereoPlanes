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
    
    vbounds.set(-2, 0, 4, 1);
    depth = 0.005;
    nCells = 40;
    
    mainTimeline->addPage("Wall Left");
    voroWall = new VoronoiPlane;
    voroWall->setup(ofRectangle(-2, -2, 8, 8), mainTimeline, "L");
    
    mainTimeline->addPage("Wall Right");
    voroWallRight = new VoronoiPlane;
    voroWallRight->setup(ofRectangle(-2, -2, 8, 8), mainTimeline, "R");
    
    mainTimeline->addPage("Floor");
    voroFloor = new VoronoiPlane;
    voroFloor->setup(ofRectangle(-2, -2, 8, 8), mainTimeline, "F");
    

    
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
    gui->addSlider(indexStr+"Rotation", 0, -90, &rotation);
    
    gui->addSlider(indexStr+"Floor", 0, 1, &floor);
}


void VoronoiPlane::draw() {
    
    for(int i=0; i < cells.size(); i++) {
        cells[i].offset = ofVec3f(0,0,0);
        
        for(int b = 0; b<breakZones.size(); b++) {
            
            if(cells[i].mesh.getCentroid().distance(breakZones[b]->pos) < breakZones[b]->radius) {
                cells[i].offset.z += breakZones[b]->strength;
                
                // add noise
                cells[i].offset.z += (ofSignedNoise(breakZones[b]->time + cells[i].r) + cells[i].r)*breakZones[b]->noise;
                
                // add rotation
                //cells[i].offset.z += (ofSignedNoise(tl->getCurrentTime() + cells[i].r) + cells[i].r);
                
            }
            
        }
     
        ofPushMatrix();
        ofTranslate(cells[i].offset);
        cells[i].mesh.draw();
        ofPopMatrix();
        
    }
    
}



void VoronoiWall::draw(int _surfaceId) {
    
    if(_surfaceId == 1) {
        
        ofTranslate(1, 1);
        ofRotateX(rotation);
        ofTranslate(-1, -1);
        
        voroWall->draw();
        
    
    }
    
    if(_surfaceId == 2) {
        
        ofRectangle bounds = ofRectangle(wallBreakPos.x-wallBreakReach.x/2, wallBreakPos.y-wallBreakReach.y/2, wallBreakReach.x, wallBreakReach.y);
        
        
        for(int i=0; i < voroWallRight->cells.size(); i++) {
            
            
            bool insideSolid = false;
            if(!bounds.inside(voroFloor->cells[i].mesh.getCentroid())) {
                    voroWallRight->cells[i].offset.z = (ofSignedNoise(wallTime + voroWallRight->cells[i].r) + voroWallRight->cells[i].r) * wallBreakStrength ;

            } else {
                voroWallRight->cells[i].offset.z = 0;
                insideSolid = true;
            }
            
            ofPushMatrix();
            ofTranslate(voroWallRight->cells[i].offset);
            
            ofColor col = ofColor(ofMap(ofClamp(voroWallRight->cells[i].offset.z, -0.2,0.2), -0.2, 0.2, 255,100));
            col.a = 225;
            
            if(insideSolid) {
                voroWallRight->cells[i].mesh.disableNormals();
                col.a = ofMap(darksolid, 0, 1, 255,0);
            } else {
                voroWallRight->cells[i].mesh.enableNormals();
            }
            
            col.a *= floor;
            
            ofSetColor(col);
            voroWallRight->cells[i].mesh.draw();
            ofPopMatrix();
                
        }
    }
    
    
}
    


void VoronoiWall::update() {
    nCells = round(nCells);
    
    voroFloor->nCells = nCells;
    voroWallRight->nCells = nCells;
    voroWall->nCells = nCells;
    
    wallTime += 0.01 * wallSpeed;
    
    /*for(int b=0; b<breakPoints.size(); b++) {

        if(breakPoints[b].pressure > 6) {
            breakPoints[b].pressure = 6;
        }
           
        breakPoints[b].pressure *= 0.98;
        // radius ? ofMap(breakPoints[b].pressure, 0, 1, 0.2, 0.8);
        
    }*/
    
    voroWall->update();
    if(floor > 0) {
        voroFloor->update();
    }
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
    } else if(rest == "/br/x") {
         /*for(int i = 0; i < breakPoints.size(); i++) {
             breakPoints[i].pos.x = m->getArgAsFloat(i);
             breakPoints[i].pressure += 0.001;
         
         }*/
        
    } else if(rest == "/br/y"){
        /*
         for(int i = 0; i < breakPoints.size(); i++) {
         
             breakPoints[i].pos.y = m->getArgAsFloat(i);
             breakPoints[i].pressure += 0.001;
         
         }*/
    } else if(rest == "/solid/x"){
        wallBreakPos.x = m->getArgAsFloat(0);
    } else if(rest == "/solid/y"){
        wallBreakPos.y = m->getArgAsFloat(0);
    } else if(rest == "/darksolid/x"){
        darksolid = m->getArgAsFloat(0);
    } else if(rest == "/breakstrength/x"){
        wallBreakStrength = m->getArgAsFloat(0);
    } else if(rest == "/rotation/x"){
        rotation = m->getArgAsFloat(0);
    } else if(rest == "/floor/x"){
        floor = m->getArgAsFloat(0);
    } else if(rest == "/cells/x"){
        nCells = m->getArgAsFloat(0);
    }
    
}
