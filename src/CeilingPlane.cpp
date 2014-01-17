//
//  CeilingPlane.cpp
//  StereoPlanes
//
//  Created by Johan Bichel Lindegaard on 16/01/14.
//
//

#include "CeilingPlane.h"


void CeilingPlane::setup(ofParameterGroup * params) {
    
    
    //params->add
    
    params->add(position.set("Plane position", ofVec3f(0,0,0), ofVec3f(-2,-2,-2), ofVec3f(2,2,2)));
    
    params->add(dimensions.set("Plane dimensions    ", ofVec3f(0.6,0.6,0), ofVec3f(-2,-2,-2), ofVec3f(2,2,2)));
    
    params->add(rotation.set("Plane rotation    ", ofVec3f(0,0,0), ofVec3f(0,0,0), ofVec3f(360,360,360)));

    
    
}



void CeilingPlane::update() {
    
}



void CeilingPlane::draw() {
    
    
    ofPushMatrix();
    ofSetRectMode(OF_RECTMODE_CENTER);
    ofFill();
    ofTranslate(position.get());
    ofRotateX(rotation.get().x);
    ofRotateY(rotation.get().y);
    ofRotateZ(rotation.get().z);
    
    ofSetColor(255, 255, 200);
    ofRect(0,0,dimensions.get().x,dimensions.get().y);
    
    ofSetRectMode(OF_RECTMODE_CORNER);
    ofPopMatrix();
    
}