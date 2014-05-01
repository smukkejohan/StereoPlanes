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
    light.setPosition(-2, -2, -2);
    
    
}


void TestScene::draw(int _surfaceId) {
    
    ofBackground (0);
    
    // A scene can draw to multiple surfaces
    
        light.enable();
    
        ofSetColor(255,255,255);
    
        ofPushMatrix();
        ofRotateX(ofGetElapsedTimef()*10);
        ofDrawBox(0.5);
        ofPopMatrix();
        
        light.disable();
    
}

void TestScene::update() {
}


void TestScene::setGui(ofxUICanvas * gui, float width){
    ContentScene::setGui(gui, width);

}

void TestScene::receiveOsc(ofxOscMessage * m, string rest) {
}

void TestScene::guiEvent(ofxUIEventArgs &e)
{
    
    string name = e.getName();
	int kind = e.getKind();
	//cout << "got event from: " << name << endl;
    
}

