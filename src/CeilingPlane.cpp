//
//  CeilingPlane.cpp
//  StereoPlanes
//
//  Created by Johan Bichel Lindegaard on 16/01/14.
//
//

#include "CeilingPlane.h"
#include "ofxVoronoi.h"

ofFbo fbo;
//ofxVoronoi voro;

void CeilingPlane::setup(ofParameterGroup * params) {
    
    
    //params->add
    
    params->add(position.set("Plane position", ofVec3f(-1,-1,0), ofVec3f(-2,-2,-2), ofVec3f(2,2,2)));
    
    params->add(dimensions.set("Plane dimensions", ofVec3f(2,2,0), ofVec3f(-2,-2,-2), ofVec3f(2,2,2)));
    
    params->add(rotation.set("Plane rotation", ofVec3f(0,0,0), ofVec3f(-360,-360,-360), ofVec3f(360,360,360)));
    
    fbo.allocate(400, 400);
    
    fbo.begin();
    
    ofBackground(255);
    
    ofSetColor(0);
    
    //voro.setup
    
    for(int i=0; i<600; i++) {
        ofCircle(ofRandom(400), ofRandom(400), ofRandom(6));
    }
    
    /*for(int i=0; i<30; i++) {
        ofCircle(ofSignedNoise(ofGetElapsedTimef()/100+i)*dimensions.get().x, ofSignedNoise(ofGetElapsedTimef()/140+i)*dimensions.get().y, dimensions.get().y/40);
    }*/
    
    fbo.end();
    
}



void CeilingPlane::update() {
    
}



void CeilingPlane::draw(ofFbo * content) {
    
    ofPushMatrix();
    //ofSetRectMode(OF_RECTMODE_CENTER);
    ofFill();
    
    ofTranslate(position.get());
    ofRotateX(rotation.get().x);
    ofRotateY(rotation.get().y);
    ofRotateZ(rotation.get().z);
    
    ofSetColor(255, 255, 200);
    //ofRect(0,0,dimensions.get().x,dimensions.get().y);
    
    ofSetColor(255);

    content->draw(0,0,dimensions.get().x,dimensions.get().y);
    
    //ofSetRectMode(OF_RECTMODE_CORNER);
    ofPopMatrix();
    
}