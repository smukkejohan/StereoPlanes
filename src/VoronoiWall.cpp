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
    
    depth = 0.015;
    nCells = 20;
    
    mainTimeline->addPage("Wall Left");
    voroWall = new VoronoiPlane;
    voroWall->setup(ofRectangle(-2, -2, 4, 4), mainTimeline, "L");
    mainTimeline->addCurves("Cells");
    
    
    mainTimeline->addPage("Wall Right");
    voroWallRight = new VoronoiPlane;
    voroWallRight->setup(ofRectangle(-2, -2, 4, 4), mainTimeline, "R");
    
    mainTimeline->addPage("Floor");
    voroFloor = new VoronoiPlane;
    voroFloor->setup(ofRectangle(-2, -2, 4, 4), mainTimeline, "F");
    
}

void VoronoiWall::setGui(ofxUICanvas * gui, float width){
    ContentScene::setGui(gui, width);
}


void VoronoiPlane::draw() {
    
    ofxOlaShaderLight::setMaterial(mat);
    
    ofPushMatrix();
    
    ofTranslate(1, 1);
    ofRotateX(tlrotationy->getValue());
    ofTranslate(-1, -1);
    
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
        
        
        /*cells[i].mat.diffuseColor = ofVec4f(1.0, 1.0, 1.0, ofMap(cells[i].offset.z, -0.2, 0.2, 0.8, 0.0));
        cells[i].mat.specularShininess = 2.8;
        cells[i].mat.specularColor = ofVec4f(1.0, 1.0, 1.0, ofMap(cells[i].offset.z, -0.2, 0.2, 0.8, 0.0));
        
        ofxOlaShaderLight::setMaterial(cells[i].mat);
        */
        cells[i].mesh.draw();
        ofPopMatrix();
    }
    
    ofPopMatrix();
    
}



void VoronoiWall::draw(int _surfaceId) {
    
    if(_surfaceId == 1) {
        
        voroWall->draw();
        
    
    }
    
    if(_surfaceId == 2) {
        
        voroWallRight->draw();
        
    }
    
    if(_surfaceId == 0) {
        
        voroFloor->draw();
        
    }
    
    
    
    
}
    


void VoronoiWall::update() {
    
    voroWall->update();
    voroFloor->update();
    voroWallRight->update();
    
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
