//
//  TestScene.cpp
//  StereoPlanes
//
//  Created by Johan Bichel Lindegaard on 21/01/14.
//
//

#include "TestScene.h"

void TestScene::draw(int _sceneId) {
    
    ofSetColor(255,255,255);
    ofRect(-1, -1, 1, 1);
    ofDrawBox(1);
    
    ofSetColor(0,0,0);
    ofDrawBitmapString(ofToString(_sceneId), 0,0);
    
}


void TestScene::setup() {
}


void TestScene::update() {
}