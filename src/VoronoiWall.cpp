//
//  VoronoiWall.cpp
//  StereoPlanes
//
//  Created by Johan Bichel Lindegaard on 15/01/14.
//
//

// TODO:
// Knock wall over
// set to floor
// Set solid space
// break at points
// Create dark space
// Increase decrease number of cells


#include "VoronoiWall.h"

void VoronoiWall::setup() {
    
    name = "Voronoi Wall";
    oscAddress = "/voronoiwall";
    // Voronoi wall
    
    vbounds.set(-1, 0, 1, 1);
    depth = 0.02;
    
    nCells = 40;
    
    light.setPosition(2, 2, -2);
    light.setSpotlight();
    light.lookAt(ofVec3f(0,0,0));
    light.setDiffuseColor(ofColor(245,245,170));
    light.setAmbientColor(ofColor(84,104,89));
    light.setSpecularColor(ofColor::white);
    
    dirLight.setSpotlight();
    dirLight.setPosition(-1, -1, 1);
    dirLight.lookAt(ofVec3f(0,0,0));
    dirLight.setDiffuseColor(ofColor(191,191,170));
    
    //voronoi.setBounds(vbounds);
    genTheVoronoi();
    
/*  params->add(subdivisions.set("Subdivisions", 4, 0, 400));
    
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
*/
    
    
    for(int i=0; i<5; i++) {
        BreakPoint br;
        br.pos = ofVec3f(0,0,0);
        br.radius = 0.25;
        br.pressure = 0;
        breakPoints.push_back(br);
    }

}

void VoronoiWall::setGui(ofxUICanvas * gui, float width){
    ContentScene::setGui(gui, width);
    
    gui->addToggle(indexStr+"Animate", &autoOn);
    gui->addSlider(indexStr+"Break strength", 0, 1.8, &wallBreakStrength);
    gui->addSlider(indexStr+"Speed", 0, 2, &wallSpeed);
    
    gui->addSlider(indexStr+"Cells", 0, 200, 20);
    
    gui->add2DPad(indexStr+"Solid pos", ofVec2f(-1, -1), ofVec2f(1,1), ofVec2f(0, 0));
    gui->add2DPad(indexStr+"Solid reach", ofVec2f(0, 0), ofVec2f(4,4), ofVec2f(0.5, 0.5));
    
}


void VoronoiWall::draw(int _surfaceId) {
    
    if(_surfaceId == 0) {
    /*for(int i = 0; i < cellMeshes.size(); i++){
        ofSetColor(255,255,255,255);
        cellMeshes[i].drawFaces();
        
        ofPushStyle();
        ofSetLineWidth(3);
        ofSetColor(0,0,0);
        //cellMeshes[i].drawWireframe();
        ofPopStyle();
    }*/
        
        light.enable();
        dirLight.enable();
        
        ofRectangle bounds = ofRectangle(wallBreakPos.x-wallBreakReach.x/2, wallBreakPos.y-wallBreakReach.y/2, wallBreakReach.x, wallBreakReach.y);
        
        
        for(int i=0; i < cells.size(); i++) {
            
            if(!bounds.inside(cells[i].mesh.getCentroid())) {
                
                if(autoOn) {
                    cells[i].offset.z = ofSignedNoise(wallTime + i) * wallBreakStrength;
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
            
            ofPushMatrix();
            ofTranslate(cells[i].offset);
            
            ofColor col = ofColor(ofMap(cells[i].offset.z, -0.4, 0.4, 255,100));
            col.a = 225;
            ofSetColor(col);
            cells[i].mesh.draw();
            
            ofPopMatrix();
        }
        
        light.disable();
        dirLight.disable();

        
    }
    
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
    
    bool changed = false;
    if(nCells != cells.size()) {
        genTheVoronoi();
    }
    
    
    /*while(nCells > voronoi.getPoints().size()) {
        //voronoi.addPoint(ofRandomPointInRect(vbounds));
        changed = true;
    }
    
    while(nCells < voronoi.getPoints().size()) {
        //voronoi.getPoints().erase(voronoi.getPoints().begin());
        changed = true;
    }
    
    if(changed) {
        cells.clear();
        
        
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
    
    voro::container con(-vbounds.width,vbounds.width,
                        -vbounds.height,vbounds.height,
                        -depth,depth,
                        1,1,1,
                        false,false,false, // set true to flow beyond box
                        8);
    
    for(int i = 0; i < nCells;i++){
        ofPoint newCell = ofPoint(ofRandom(-vbounds.width,vbounds.width),
                                  ofRandom(-vbounds.height,vbounds.height),
                                  ofRandom(-depth,depth));
        
        addCellSeed(con, newCell, i, true);
    }
    
    
    cellMeshes = getCellsFromContainer(con);
    
    cells.clear();
    
    for (int i=0; i < cellMeshes.size(); i++) {
        
        Cell cell;
        cell.mesh.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
        
        cell.offset = ofVec3f(0,0,0);
        int r = ofRandom(0,255);
        cell.mesh = cellMeshes[i];
        
        cells.push_back(cell);
    }
    
    
    //cellRadius = getCellsRadius(con);
    //cellCentroids = getCellsCentroids(con);
    
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
    
    wallTime += 0.01 * wallSpeed;
    
    for(int b=0; b<breakPoints.size(); b++) {

        if(breakPoints[b].pressure > 6) {
            breakPoints[b].pressure = 6;
        }
           
        breakPoints[b].pressure *= 0.98;
        // radius ? ofMap(breakPoints[b].pressure, 0, 1, 0.2, 0.8);
        
    }
    
    updateCells();
}

void VoronoiWall::guiEvent(ofxUIEventArgs &e)
{
    
    string name = e.getName();
	int kind = e.getKind();
	//cout << "got event from: " << name << endl;
    
    if(name==indexStr+"Cells") {
        ofxUISlider *slider = e.getSlider();
        nCells = round(slider->getScaledValue());
        slider->setValue(nCells);
    }
}

void VoronoiWall::receiveOsc(ofxOscMessage * m, string rest) {
    
    if(rest == "/spped/x" ) {
        wallSpeed = m->getArgAsFloat(0);
    }
    
}
