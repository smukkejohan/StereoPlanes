//
//  TestScene.cpp
//  StereoPlanes
//
//  Created by Johan Bichel Lindegaard on 21/01/14.
//
//

#include "TestScene.h"


void TestScene::setup() {
    name = "Test Scene";
    oscAddress = "/testscene";
    light.setPointLight();
    light.setPosition(-1, -1, -1);
    
    
}


void TestScene::draw(int _surfaceId) {
    
    ofBackground (0);
    
    // A scene can draw to multiple surfaces
    if(_surfaceId == 0) {
        
        light.enable();
                
        // ofSetColor(0,255,0,250);
        // FIXME: there is an alpha problem here, the box can't be seen through the rect if drawn
        // ofRect(-1, -1, 2, 2);
        
        ofSetColor(255,255,255);
        //ofDrawGrid(1);
        
        ofPushMatrix();
        ofRotateX(ofGetElapsedTimef()*10);
        ofDrawBox(0.5);
        ofPopMatrix();
        
        light.disable();
    
    }
    
    if(_surfaceId == 1) {
        
        light.enable();
        
        ofSetColor(255,255,255);
        //ofDrawGrid(1);
        
        ofPushMatrix();
        ofRotateX(ofGetElapsedTimef()*10);
        ofDrawSphere(0.5);
        ofPopMatrix();
        
        light.disable();
        
    }
    
    
}




void TestScene::update() {
}