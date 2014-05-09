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
    
    depth = 0.015;
    nCells = 2;
    
    mainTimeline->addPage(name);
    voroWall = new VoronoiPlane;
    voroWall->setup(ofRectangle(-1, -1, 2, 2), mainTimeline, indexStr);
    mainTimeline->addCurves("Cells");

}

void VoronoiWall::setGui(ofxUICanvas * gui, float width){
    ContentScene::setGui(gui, width);
}

void VoronoiPlane::draw() {
    
    ofPushMatrix();
    
    ofTranslate(tlrotationfixx->getValue(), tlrotationfixy->getValue());
    ofRotateX(tlrotationy->getValue());
    ofRotateY(tlrotationx->getValue());
    ofTranslate(-tlrotationfixx->getValue(), -tlrotationfixy->getValue());
    
    for(int i=0; i < cells.size(); i++) {
        
        cells[i].mat.diffuseColor = ofVec4f(1.0, 1.0, 1.0, tldifalpha->getValue());
        cells[i].mat.specularColor = ofVec4f(1.0, 1.0, 1.0, tlspecalpha->getValue());
        cells[i].mat.specularShininess = tlshine->getValue();
        
        cells[i].offset = ofVec3f(0,0,0);
        
        for(int b = 0; b<breakZones.size(); b++) {
            
            if(cells[i].mesh.getCentroid().distance(breakZones[b]->pos) < breakZones[b]->radius) {
                cells[i].offset.z += breakZones[b]->strength;
                
                // add noise
                cells[i].offset.z += (ofSignedNoise(breakZones[b]->time + cells[i].r) + cells[i].r)*breakZones[b]->noise;
                
                // add rotation
                //cells[i].offset.z += (ofSignedNoise(tl->getCurrentTime() + cells[i].r) + cells[i].r);
                cells[i].mat.diffuseColor.w  = cells[i].mat.diffuseColor.w  * breakZones[b]->tldifalpha->getValue();
                cells[i].mat.specularColor.w = cells[i].mat.specularColor.w * breakZones[b]->tlspecalpha->getValue();
                //mat.specularShininess = tlshine->getValue();
            }
        }
        
        ofPushMatrix();
        ofTranslate(cells[i].offset);
        
        
        if(tlbackalphamax->getValue() > 0) {
        
            float a = ofMap(ofClamp(abs(cells[i].offset.z), 0, tlbackalphamax->getValue()), 0, tlbackalphamax->getValue(), 1.0, 0.0);
        
        cells[i].mat.diffuseColor.w *= a;
        cells[i].mat.specularColor.w *= a;
            
        }
        
        ofxOlaShaderLight::setMaterial(cells[i].mat);
        
        // if we have really high alpha we don't bother drawing, saves a bit of performance
        if(cells[i].mat.diffuseColor.w + cells[i].mat.specularColor.w > 0.005) {
            cells[i].mesh.draw();
        }
        
        ofPopMatrix();
    }
    
    ofPopMatrix();
    
}



void VoronoiWall::draw(int _surfaceId) {
    
    if(_surfaceId == primarySurface) {
        voroWall->draw();
    }
    
}
    


void VoronoiWall::update() {
    voroWall->update();
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
