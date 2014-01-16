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
    
    params->add(position.set("Plane position", ofVec3f(-0.6,-0.6,0), ofVec3f(-1,-1,-1), ofVec3f(2,2,2)));
    
    params->add(dimensions.set("Plane dimensions    ", ofVec3f(-0.6,-0.6,0), ofVec3f(-1,-1,-1), ofVec3f(2,2,2)));

    
    
}



void CeilingPlane::update() {
    
}



void CeilingPlane::draw() {
    
    
    ofPushMatrix();
    
    ofTranslate(position.get());
    
    ofSetColor(200, 255, 200);
    ofRect(0,0,2,2);
    
    ofPopMatrix();
    
}