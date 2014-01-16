//
//  VoronoiWall.cpp
//  StereoPlanes
//
//  Created by Johan Bichel Lindegaard on 15/01/14.
//
//

#include "VoronoiWall.h"



void VoronoiWall::setup(ofParameterGroup * params) {
    
    // Voronoi wall
    vbounds.set(-0.9, -1, 1.8, 2);
    voronoi.setBounds(vbounds);
    
    genTheVoronoi();
    
    params->add(shivering.set("Shiver", 0, 0, 6));
    params->add(wallSpeed.set("wallSpeed", 0, 0, 2));
    params->add(subdivisions.set("Subdivisions", 4, 0, 400));
    
    params->add(wallBreakPos.set("wallBreakPos", ofVec3f(0.1,0.5,0), ofVec3f(-1,-1,-1), ofVec3f(1,1,1)));
    params->add(wallBreakReach.set("wallBreakReach", ofVec3f(0.2,2,1), ofVec3f(0,0,0), ofVec3f(2,2,2)));
    params->add(wallBreakStrength.set("wallBreakStrength", 0, 0, 1.8));
    

}


void VoronoiWall::draw() {
    
    ofRectangle bounds = ofRectangle(wallBreakPos.get().x-wallBreakReach.get().x/2, wallBreakPos.get().y-wallBreakReach.get().y/2, wallBreakReach.get().x, wallBreakReach.get().y);
    
    bool changed = false;
    while(subdivisions.get() > voronoi.getPoints().size()) {
        voronoi.addPoint(ofRandomPointInRect(vbounds));
        changed = true;
    }
    
    while(subdivisions.get() < voronoi.getPoints().size()) {
        voronoi.getPoints().erase(voronoi.getPoints().begin());
        changed = true;
    }
    
    if(changed) {
        voronoi.generateVoronoi();
    }
    
    //TODO:Factor out to seperate class
    
    glPushMatrix();
    
    ofNoFill();
    
    //light.enable();
    //dirLight.enable();
    
    // draw a frame for the breaking wall
    ofFill();
    ofSetColor(200,230,200);
    // left
    /*ofRect(-1, -1, 0.1, 2);
     ofRect(0.9, -1, 0.1, 2);
     ofRect(-1, -1, 2, 0.1);
     ofRect(-1, 0.9, 2, 2);
     */
    
    ofPushMatrix();
    ofNoFill();
    ofSetLineWidth(5);
    ofSetColor(0);
    ofTranslate(wallBreakPos.get().x, wallBreakPos.get().y);
    // ofEllipse(0, 0, wallBreakReach.get().x, wallBreakReach.get().y);
    
    ofPopMatrix();
    
    //ofDrawRect(bounds.getPosition(), );
    
    
    //voronoi.draw();
    //voronoi.getPoints().size();
    
    for(int i=0; i < voronoi.cells.size(); i++) {
        
        ofMesh vcell;
        vcell.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
        
        bool inbreakzone = false;
        
        for(int v=0; v<voronoi.cells[i].pts.size(); v++) {
            
            vcell.addVertex(voronoi.cells[i].pts[v]);
            
            ofColor col;
            //if(i%2 == 0) {
            //    col.set(ofMap(i,0,voronoi.cells.size(),255, 2),255,250);
            //} else {
            col.set(ofMap(0.0, -0.2, 0.2, 255,100));
            //}
            
            if(bounds.inside(voronoi.cells[i].pts[v])) {
                inbreakzone = false;
            }
            
            vcell.addColor(col);
        }
        
        ofPushMatrix();
        
        
        if(inbreakzone) {
            
        }
        
        // ofRect(bounds.getPosition().x, bounds.getPosition().y, 0, bounds.getWidth(), bounds.getHeight());
        
        
        if(!bounds.inside(vcell.getCentroid())) {
            
            
            float z = ofSignedNoise(wallTime + i) * wallBreakStrength.get();
            ofTranslate(0, 0, z);
            //ofRotateY( ofSignedNoise(ofGetElapsedTimef()*2 + i) * shivering);
            
            for(int c=0; c<vcell.getColors().size(); c++) {
                vcell.getColors()[c];
                
                vcell.setColor(c, ofColor(ofMap(z, -0.2, 0.2, 255,100)));
                
            }
            
            vcell.draw();
            
        }
        
        
        
        ofPopMatrix();
    }
    
    //light.disable();
    //dirLight.disable();
    
    ofDisableLighting();
    glPopMatrix();

    
}

void VoronoiWall::genTheVoronoi() {
    int n = subdivisions.get();
    for(int i=0; i<n; i++) {
        vpts.push_back(ofRandomPointInRect(vbounds));
    }
    
    //vpts.push_back(ofVec3f(0,0,0));
    voronoi.clear();
    for(int i=0; i<vpts.size(); i++) {
        voronoi.addPoint(vpts[i]);
    }
    voronoi.generateVoronoi();
    
}

void VoronoiWall::update() {
    
    
    wallTime += 0.01 * wallSpeed;

    
}

void VoronoiWall::drawVoronoiWall3d() {
    
}