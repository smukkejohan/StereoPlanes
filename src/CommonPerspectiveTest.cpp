//
//  CommonPerspectiveTest.cpp
//  StereoPlanes
//
//  Created by Johan Bichel Lindegaard on 20/01/14.
//
//

#include "CommonPerspectiveTest.h"

void CommonPerspectiveTest::setup(ofParameterGroup * params) {
    
    box.set(0.40);
    cyl.setHeight(0.4);
    cyl.setRadius(0.015);
    cyl.setResolution(40, 4);
    
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
    ofTranslate(0, -1.2, -0.2);
     ofLogo.bind();
    ofDrawBox(0.7, 0.05, 1);
    ofLogo.unbind();

    ofPopMatrix();
    
    ofPushMatrix();
    ofTranslate(0, 1.2);
    //ofLogo.bind();
    ofDrawBox(0.7, 0.05, 1);
    //ofLogo.unbind();
    
    ofPopMatrix();
    
    
    ofPushMatrix();
    
    //light.draw();
    //ofRotateX(40);
    
    ofRotateY(90);
    //ofRotateX(ofGetElapsedTimef()*7);
    //ofRotateZ(ofGetElapsedTimef()*8);
    
    ofFill();
    ofSetColor(255);
    
   
    //box.draw();
    
    ofSetColor(255, 255, 255);
    
    for(int i=0; i<360;i+=4 ) {
        ofPushMatrix();
        
        ofRotateX(i);
        cyl.setHeight((sin(ofGetElapsedTimef()+i/360.0)+1) / 10);
        
        ofRotateZ(sin(ofGetElapsedTimef()+i)*12);
        ofTranslate(0,-0.6);
        
        cyl.draw();
        ofPopMatrix();
    }
    
    
    ofDrawGrid(6);
    
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

void CommonPerspectiveTest::transformWall() {
    ofTranslate(0,1);
}

void CommonPerspectiveTest::transformFloor() {
    ofTranslate(0,-1);
    // this will rotate the floor to match the screen space
    ofRotate(90, 1, 0, 0);
    
}

void CommonPerspectiveTest::drawFloor() {
    glPushMatrix();
    ofPushMatrix();
    
    ofTranslate(0,-1);
    //ofScale(4,4,4);
    
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
