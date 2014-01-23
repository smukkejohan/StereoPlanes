//
//  Voro3D.cpp
//  StereoPlanes
//
//  Created by Johan Bichel Lindegaard on 21/01/14.
//
//

#include "Voro3D.h"



void Voro3D::setup() {
    
    light.setPosition(2, 2, -2);
    light.setSpotlight();
    light.lookAt(ofVec3f(0,0,0));
    light.setDiffuseColor(ofColor(255,55,255));
    light.setAmbientColor(ofColor(85,235,199));
    light.setSpecularColor(ofColor::red);
    
    dirLight.setSpotlight();
    dirLight.setPosition(-1, -1, 1);
    dirLight.lookAt(ofVec3f(0,0,0));
    dirLight.setDiffuseColor(ofColor(180,100,100));
    dirLight.setSpecularColor(ofColor::white);
        
    wallCube = new VoroCube();
    wallCube->setup(0.2,0.2,0.2,40);

    /*
    extraCube = new VoroCube();
    extraCube->setup(0.4,0.01,0.2,60);
    
    floorCube = new VoroCube();
    floorCube->setup(0.2,0.2,0.1,100);
    */
}

void Voro3D::draw(int _surfaceId) {
    
    
    
    ofBackground(0);
    
    light.enable();
    dirLight.enable();
    
    if(_surfaceId == 1) {
    
        /*for (int i = 0; i < cellCentroids.size(); i++){
            ofSetColor(255,0,0);
            //ofDrawSphere(cellCentroids[i], cellRadius[i]*0.15);
         }*/
        
        
        ofPushMatrix();
        
        ofTranslate(0.15, 0);
        
        ofRotateX(ofGetElapsedTimef()*6);
        ofRotateY(ofGetElapsedTimef()*7);
        ofRotateZ(ofGetElapsedTimef()*8);
        
        for(int i = 0; i < wallCube->cellMeshes.size(); i++){
            ofPushMatrix();
            
            ofScale(1.8,1.8,1.8);
            ofSetColor(255,255,255,255);
            
            float explode = ofMap(ofSignedNoise(ofGetElapsedTimef()), 0, 1, 0.8, 1);
            
            ofTranslate(wallCube->cellMeshes[i].getCentroid().x, wallCube->cellMeshes[i].getCentroid().y, wallCube->cellMeshes[i].getCentroid().z);
            ofScale(explode,explode,explode);
            ofTranslate(-wallCube->cellMeshes[i].getCentroid().x, -wallCube->cellMeshes[i].getCentroid().y, -wallCube->cellMeshes[i].getCentroid().z);
            
            wallCube->cellMeshes[i].drawFaces();
            ofPopMatrix();

        }
        ofPopMatrix();
        
        
        ofPushMatrix();
        ofTranslate(-0.5,-0.1,0.08);
        
        ofTranslate(0,1,0);
        ofRotateX(-90);
        
        for(int i=0; i<6; i++) {
            ofPushMatrix();
            
            ofTranslate(0, ofMap(i,0,6,0,-1),0);
            ofDrawBox(0.6,0.02,0.02);
            
            ofPopMatrix();
        }
        
        ofPopMatrix();
    
    
    }
    
    if(_surfaceId == 0) {
    
        
        
        
        
    
    
    
    }
    
    dirLight.disable();
    light.disable();
        
        
    
    
}

void Voro3D::update() {
    wallCube->update();
    
}

void Voro3D::setGui(ofxUICanvas * gui, float width){
    ContentScene::setGui(gui, width);

    
}

void Voro3D::receiveOsc(ofxOscMessage * m, string rest) {
}

void Voro3D::guiEvent(ofxUIEventArgs &e)
{
    
    string name = e.getName();
	int kind = e.getKind();
	//cout << "got event from: " << name << endl;
    
}



