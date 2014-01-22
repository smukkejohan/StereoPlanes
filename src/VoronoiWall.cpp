//
//  VoronoiWall.cpp
//  StereoPlanes
//
//  Created by Johan Bichel Lindegaard on 15/01/14.
//
//

#include "VoronoiWall.h"

void VoronoiWall::setup() {
    
    // Voronoi wall
    
    //vbounds = bounds;
    //voronoi.setBounds(vbounds);
    
    genTheVoronoi();
    
   // params->add(shivering.set("Shiver", 0, 0, 6));
    /*params->add(wallSpeed.set("wallSpeed", 0, 0, 2));
    params->add(subdivisions.set("Subdivisions", 4, 0, 400));
    
    params->add(wallBreakPos.set("wallBreakPos", ofVec3f(0.1,0.5,0), ofVec3f(-1,-1,-1), ofVec3f(1,1,1)));
    params->add(wallBreakReach.set("wallBreakReach", ofVec3f(0.2,2,1), ofVec3f(0,0,0), ofVec3f(2,2,2)));
    params->add(wallBreakStrength.set("wallBreakStrength", 0, 0, 1.8));
    
    params->add(breakPointRadius.set("breakPointRadius", 0.2, 0, 1.4));
     
    
    for(int i=0; i<5; i++) {
        BreakPoint br;
        br.pos = ofVec3f(0,0,0);
        br.radius = 0.25;
        br.pressure = 0;
        
        breakPoints.push_back(br);
    }
    
    params->add(autoOn);*/

}


void VoronoiWall::draw() {
    
    /*updateCells();
    
    ofRectangle bounds = ofRectangle(wallBreakPos.get().x-wallBreakReach.get().x/2, wallBreakPos.get().y-wallBreakReach.get().y/2, wallBreakReach.get().x, wallBreakReach.get().y);
    
    glPushMatrix();
    ofNoFill();
    */
    
    //light.enable();
    //dirLight.enable();
    
    // draw a frame for the breaking wall
    //ofFill();
    //ofSetColor(200,230,200);
    // left
    /*ofRect(-1, -1, 0.1, 2);
     ofRect(0.9, -1, 0.1, 2);
     ofRect(-1, -1, 2, 0.1);
     ofRect(-1, 0.9, 2, 2);
     */
    
    /*ofPushMatrix();
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
                cells[i].offset.z = ofSignedNoise(wallTime + i) * wallBreakStrength.get();
            }
            
            
            bool inBreakpoint = false;
            for(int b=0; b<breakPoints.size(); b++) {
                
                //todo: break more with distance
                
               if(breakPoints[b].pos.distance(cells[i].mesh.getCentroid()) < breakPoints[b].radius) {
                   cells[i].offset.z = ofMap(breakPoints[b].pos.distance(cells[i].mesh.getCentroid()), 0, breakPoints[b].radius, breakPoints[b].pressure, 0);
                    inBreakpoint = true;
                }
            }
            
            if(!inBreakpoint) {
                cells[i].offset.z * 0.6978;
            }
            
        } else {
            
            cells[i].offset.z = 0;
            
        }
        
        
        for(int c=0; c<cells[i].mesh.getColors().size(); c++) {
            cells[i].mesh.setColor(c, ofColor(ofMap(cells[i].offset.z, -0.2, 0.2, 255,100)));
            //cells[i].mesh.setColor(c,cells[i].color);

        }
        
        ofPushMatrix();
        ofTranslate(cells[i].offset);
        cells[i].mesh.draw();
        
        
        
        ofPopMatrix();*/
        
    //}
    
    /*for(int b=0; b<breakPoints.size(); b++) {
        ofPushMatrix();
        ofTranslate(0, 0, -0.1);
        ofSetColor(0,0,0,20);
        //ofCircle(breakPoints[b].pos, breakPoints[b].radius);
        ofPopMatrix();
    }*/
    
    //light.disable();
    //dirLight.disable();
    
    //ofDisableLighting();
    /*glPopMatrix();
        
    }*/
    
}

void VoronoiWall::updateCells() {
    /*
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
            
            cell.offset = ofVec3f(0,0,0);
            int r = ofRandom(0,255);
            cell.color = ofColor(ofRandom(r/2,r), ofRandom(r/2,r), r);
            
            cells.push_back(cell);
        }
    }*/
    
}

void VoronoiWall::genTheVoronoi() {
    /*
    int n = subdivisions.get();
    for(int i=0; i<n; i++) {
        vpts.push_back(ofRandomPointInRect(vbounds));
    }
    
    //vpts.push_back(ofVec3f(0,0,0));
    voronoi.clear();
    for(int i=0; i<vpts.size(); i++) {
        voronoi.addPoint(vpts[i]);
    }
    
    
    voro::wall
    
    voronoi.generateVoronoi();*/
}

void VoronoiWall::update() {
    /*
    if(active) {
    
        wallTime += 0.01 * wallSpeed;
    
        for(int b=0; b<breakPoints.size(); b++) {
        

        if(breakPoints[b].pressure > 6) {
            breakPoints[b].pressure = 6;
        }
           
        breakPoints[b].pressure *= 0.98;
        // radius ? ofMap(breakPoints[b].pressure, 0, 1, 0.2, 0.8);
        
        }
    }
     */
}


void VoronoiWall::drawVoronoiWall3d() {
}