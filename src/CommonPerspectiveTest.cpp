//
//  CommonPerspectiveTest.cpp
//  StereoPlanes
//
//  Created by Johan Bichel Lindegaard on 20/01/14.
//
//

#include "CommonPerspectiveTest.h"

void CommonPerspectiveTest::setup(ofParameterGroup * params) {
    
    box.set(1.00);
    
    ofLogo.loadImage("of.png");
    
    light.setPosition(2, 2, -1.5);
    light.setSpotlight();
    light.lookAt(ofVec3f(0,0,0));
    light.setDiffuseColor(ofColor(245,245,170));
    light.setAmbientColor(ofColor(64,84,89));
    light.setSpecularColor(ofColor::white);
    
    
    
    
}


void CommonPerspectiveTest::draw() {
    
    ofEnableNormalizedTexCoords();
    ofEnableLighting();
    light.enable();
    
    ofPushMatrix();
    
    ofScale(0.5,0.5,0.5);
    
    //light.draw();
    
    //ofRotateX(40);
    ofRotateY(ofGetElapsedTimef()*61);
    ofRotateX(ofGetElapsedTimef()*60);
    ofRotateZ(ofGetElapsedTimef()*62);
    
    ofFill();
    ofSetColor(255);
    
    ofLogo.bind();
    box.draw();
    ofLogo.unbind();
    
    
    ofPushMatrix();
    ofTranslate(-1, 0);
    ofLogo.bind();
    box.draw();
    ofLogo.unbind();
    ofPopMatrix();
    
    ofPushMatrix();
    ofTranslate(1, 0);
    ofLogo.bind();
    box.draw();
    ofLogo.unbind();
    ofPopMatrix();
    
    ofPushMatrix();
    ofTranslate(0, 1);
    ofLogo.bind();
    box.draw();
    ofLogo.unbind();
    ofPopMatrix();
    
    ofPushMatrix();
    ofTranslate(0, -1);
    ofLogo.bind();
    box.draw();
    ofLogo.unbind();
    ofPopMatrix();
    
    
    
    ofNoFill();
    ofSetColor(255,0,255);
    ofSetLineWidth(10);
    box.draw();
    
    ofPopMatrix();
    
    light.disable();
    ofDisableLighting();
    ofDisableNormalizedTexCoords();
    
}

// wall is primary
void CommonPerspectiveTest::drawWall() {
    glPushMatrix();
    ofPushMatrix();
    ofTranslate(0,1);
    //ofTranslate(0,1, -0.5);
    
    draw();
    
    ofPopMatrix();
    glPopMatrix();
}


void CommonPerspectiveTest::drawFloor() {
    glPushMatrix();
    ofPushMatrix();
    
    ofTranslate(0,-1);
    //ofScale(4,4,4);
    //ofTranslate(0,-0.5, -0.5);
    
    // this will rotate the floor to match the screen space
     //ofTranslate(0, -1);
    ofRotate(90, 1, 0, 0);
     //ofTranslate(0, 1);
    
    draw();
    
    ofPopMatrix();
    glPopMatrix();
}


void CommonPerspectiveTest::update() {
    
}
