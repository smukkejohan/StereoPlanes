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
    
    
    params->add(breakPointRadius.set("breakPointRadius", 0.2, 0, 1.4));
    
    for(int i=0; i<5; i++) {
        BreakPoint br;
        breakPoints.push_back(br);
    }
    
    params->add(autoOn);

}


void VoronoiWall::draw() {
    
    updateCells();
    
    ofRectangle bounds = ofRectangle(wallBreakPos.get().x-wallBreakReach.get().x/2, wallBreakPos.get().y-wallBreakReach.get().y/2, wallBreakReach.get().x, wallBreakReach.get().y);
    
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
    
    for(int i=0; i < cells.size(); i++) {
        
        
        // ofRect(bounds.getPosition().x, bounds.getPosition().y, 0, bounds.getWidth(), bounds.getHeight());
        
        if(!bounds.inside(cells[i].mesh.getCentroid())) {
            
            if(autoOn.get()) {
                cells[i].pos.z = ofSignedNoise(wallTime + i) * wallBreakStrength.get();
            }
            
            
            for(int b=0; b<breakPoints.size(); b++) {
                
                //todo: break more with distance
                
                if(breakPoints[i].pos.distance(cells[i].pos) < breakPoints[i].radius) {
                    
                    cells[i].pos.z += breakPoints[i].pressure;
                    // todo: map reverse distance to center multiply by pressure
                    
                } else {
                    cells[i].pos.z *= 0.998;
                }
            }
            
            for(int c=0; c<cells[i].mesh.getColors().size(); c++) {
                cells[i].mesh.setColor(c, ofColor(ofMap(cells[i].pos.z, -0.2, 0.2, 255,100)));
                cells[i].mesh.setColor(c, ofColor(255));
            }
            
        }
        
        ofPushMatrix();
        //ofTranslate(cells[i].pos);
        cells[i].mesh.draw();
        //ofCircle(cells[i].pos.x, cells[i].pos.y, 0.1);
        ofPopMatrix();
    }
    
    //light.disable();
    //dirLight.disable();
    
    //ofDisableLighting();
    glPopMatrix();
    
}

void VoronoiWall::updateCells() {
    
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
        cells.clear();
        
        voronoi.generateVoronoi();
        
        for(int i=0; i<voronoi.cells.size(); i++) {
            
            Cell cell;
            cell.mesh.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
            
            for(int v=0; v<voronoi.cells[i].pts.size(); v++) {
                cell.mesh.addVertex(voronoi.cells[i].pts[v]);
                ofColor col; // move to draw
                col.set(ofMap(0.0, -0.2, 0.2, 255,100));
                cell.mesh.addColor(col);
            }
            
            cell.pos = cell.mesh.getCentroid();

            cells.push_back(cell);
        }
        
    }
    

    
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