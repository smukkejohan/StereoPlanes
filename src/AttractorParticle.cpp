//
//  attractorParticle.cpp
//  StereoPlanes
//
//  Created by Bernardo Schorr on 1/17/14.
//
//

#include "attractorParticle.h"

void AttractorParticle::setup( ofVec3f _pos, ofVec3f _vel ) {
    //pos.set(ofRandom(-1, 1), ofRandom(-1, 1), ofRandom(-1, 1));
    //velocity.set(ofRandom(-0.03, 0.03), ofRandom(-0.03, 0.03), ofRandom(-0.03, 0.03));
    
    pos = _pos;
    velocity = _vel;
}

void AttractorParticle::update(float _damping, float _accelMult, ofVec3f attractorPos) {
    
    damping= _damping;
    accelMult = _accelMult;
    
    damping = ofMap( damping, 1, 10, 1.0, 0.9 );
    accelMult = ofMap( accelMult, 1, 10, 0.001, 0.01);
    
    prevPos = pos;
    
    ofVec3f accel = (attractorPos - pos) * accelMult;
    velocity += accel;
    
    pos += velocity;
    velocity *= damping;
    
    points.push_back(pos);
    
    if (points.size() > 255) {
        points.pop_back();
    }
    
}

void AttractorParticle::draw() {
    
    //ofSetColor(255);
    //ofCircle(pos, 0.01);
    
//    for (int i = 1; i < points.size(); i++) {
//        
//        ofBeginShape();
//        
//        ofVertex(points[i-1].x, points[i-1].y, points[i-1].z);
//        ofVertex(points[i].x, points[i].y, points[i].z);
//        ofVertex(points[i].x, points[i].y+0.01, points[i].z);
//        ofVertex(points[i-1].x, points[i-1].y+0.01, points[i-1].z);
//        
//        ofEndShape();
//        
//    }
    
    if (pos != prevPos) {
        ofBeginShape();
        
        ofVertex(prevPos.x, prevPos.y, prevPos.z);
        ofVertex(pos.x, pos.y, pos.z);
        ofVertex(pos.x, pos.y+0.02, pos.z);
        ofVertex(prevPos.x, prevPos.y+0.02, prevPos.z);
        
        ofEndShape();
        
    }

    
}