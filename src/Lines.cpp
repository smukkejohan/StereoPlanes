//
//  lines.cpp
//  StereoPlanes
//
//  Created by Ole Kristensen on 17/01/14.
//
//

#include "Lines.h"

void Lines::setup(ofParameterGroup * params) {
    params->add(angle.set("lines angle", 0, 0, 90));
    params->add(width.set("lines width", 2, 0, 100));
    params->add(distance.set("lines distance", 10, 0.1, 100));
    params->add(randomWidth.set("lines rand width", 0, 0, 1));
    params->add(origin.set("origin", ofVec3f(0,0,0), ofVec3f(-3,-3,-3), ofVec3f(3,3,3)));
    params->add(randomDepth.set("lines rand depth", 0, 0, 1));
}

void Lines::update() {
    // nothing to update
}

void Lines::draw() {
    
    ofPushMatrix();

    ofTranslate(origin.get());
    ofRotateZ(angle.get());
    
    ofSetColor(255, 255, 255, 255);
    ofSetRectMode(OF_RECTMODE_CENTER);
    
    float dist = distance.get();
    
    for (double i = 0.; i < 100.; i+=dist) {

        ofTranslate(0,0,(ofSignedNoise(0,i)*randomDepth.get()));
        ofRect(ofMap(i, 0, 100, -1, 1),0, (width.get()/100.0) + (ofSignedNoise(i,0,0)*randomWidth.get()), 10.);
        
    
    }
    
    ofSetRectMode(OF_RECTMODE_CORNER);

    ofPopMatrix();
    
}