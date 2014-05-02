//
//  Voro3D.cpp
//  StereoPlanes
//
//  Created by Johan Bichel Lindegaard on 21/01/14.
//
//

#include "GhostLights.h"

void GhostLights::setup() {
    
    name = "Ghost Lights";
    
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
    
}

void GhostLights::draw(int _surfaceId) {
    
    ofBackground(0);
    if(_surfaceId == 1) {

        ofPushMatrix(); {
            ofTranslate(0.15, 0);
            
            ofRotateX(ofGetElapsedTimef()*6);
            ofRotateY(ofGetElapsedTimef()*7);
            ofRotateZ(ofGetElapsedTimef()*8);
            
            for(int i = 0; i < wallCube->cellMeshes.size(); i++){
                ofPushMatrix(); {
                    
                    ofScale(1.65,1.65,1.65);
                    ofSetColor(255,255,255,255);
                    
                    float explode = ofMap(ofSignedNoise(ofGetElapsedTimef()), 0, 1, 0.8, 1);
                    
                    ofTranslate(wallCube->cellMeshes[i].getCentroid().x, wallCube->cellMeshes[i].getCentroid().y, wallCube->cellMeshes[i].getCentroid().z);
                    ofScale(explode,explode,explode);
                    ofTranslate(-wallCube->cellMeshes[i].getCentroid().x, -wallCube->cellMeshes[i].getCentroid().y, -wallCube->cellMeshes[i].getCentroid().z);
                    
                    wallCube->cellMeshes[i].drawFaces();
                } ofPopMatrix();
                
            }
        } ofPopMatrix();
        
        ofPushMatrix(); {
            ofTranslate(-0.5,-0.1,0.08);
            
            ofTranslate(0,1,0);
            ofRotateX(-90);
            
            for(int i=0; i<12; i++) {
                ofPushMatrix(); {
                    
                    ofTranslate(0, ofMap(i,0,12,0,-3),0);
                    ofSetColor(ofMap(i,0,12,180,10));
                    ofDrawBox(0.6,0.02,0.02);
                    
                } ofPopMatrix();
            }
        } ofPopMatrix();
        
    }
    
}

void GhostLights::update() {
    wallCube->update();
    
}

void GhostLights::setGui(ofxUICanvas * gui, float width){
    ContentScene::setGui(gui, width);
    
    
}

void GhostLights::receiveOsc(ofxOscMessage * m, string rest) {
}

void GhostLights::guiEvent(ofxUIEventArgs &e)
{
    
    string name = e.getName();
	int kind = e.getKind();
	//cout << "got event from: " << name << endl;
    
}



