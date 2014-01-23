//
//  WireMesh.cpp
//  StereoPlanes
//
//  Created by Maria Paula Saba on 1/17/14.
//
//

#include "WireMesh.h"

void WireMesh::setup() {
    
    name = "Wire Mesh";
    oscAddress = "/wiremesh";
    
    createMesh();
    
    
}

void WireMesh::update() {
    
    int numVerts = mesh.getNumVertices();
    for (int i=0; i<numVerts; i++) {
        
        ofVec3f vert = mesh.getVertex(i);
        float time = ofGetElapsedTimeMillis();
        float tempNoise = ofNoise(vert.x + time*speed, vert.y);
        vert.z = tempNoise;
        
        //if it is close to the dancer
        if(ofDist(dancerPos.x,dancerPos.y, vert.x, vert.y) < shadowSize){
            mesh.setColor(i, ofColor(0,0,0));
            mesh.setVertex(i, ofVec3f(vert.x, vert.y, blackMesh));
            
        }
        else{
            mesh.setColor(i, colors[i]);
            mesh.setVertex(i, ofVec3f(vert.x, vert.y, vert.z*whiteMesh));
        }
    }
    
    
    if(reset){
        resetMesh();
    }
    if (createVert) {
        createVertex();
    }
}

void WireMesh::setGui(ofxUICanvas * gui, float width){
    ContentScene::setGui(gui, width);
    
    
    gui->addSlider(indexStr+"Line Width", 0.1, 10, &lineWidth);
    gui->addSlider(indexStr+"Offset X", -4, 4, &offset.x);
    gui->addSlider(indexStr+"Offset Y", -4, 4, &offset.y);
    gui->addSlider(indexStr+"Offset Z", -4, 4, &offset.z);
    gui->addSlider(indexStr+"Dancer X", -4, 4, &dancerPos.x);
    gui->addSlider(indexStr+"Dancer Y", -4, 4, &dancerPos.y);
    
    
    gui->addSlider(indexStr+"Speed",  0, 0.002, &speed);
    gui->addSlider(indexStr+"Background Color",  0, 255, &bgColor);
    gui->addSlider(indexStr+"White Mesh Z",  -5, 5, &whiteMesh);
    gui->addSlider(indexStr+"Black Mesh Z",  -5, 5, &blackMesh);
    gui->addSlider(indexStr+"Shadow Size", 0.1, 3, &shadowSize);
    gui->addSlider(indexStr+"Number of Vertices",  100, 600, &numVerts);
    gui->addSlider(indexStr+"Connection Threshold",  0.1, 1, &threshold);
    
    
    gui->addToggle(indexStr+"Surface", &mySurface);
    gui->addToggle(indexStr+"Create Vertex", &createVert);
    gui->addToggle(indexStr+"Reset Mesh", &reset);
    
}



void WireMesh::draw(int _surfaceId ) {
    
    if (_surfaceId == mySurface) {
        
        glEnable(GL_DEPTH_TEST);
        
        //ofSetColor(bgColor);
        //ofFill();
        //ofRect(-10, -10, 5, 20, 20);
        
        ofPushMatrix();
        ofTranslate(offset.x, offset.y, offset.z);
        
        ofDrawGrid(6);
        
        ofSetLineWidth(lineWidth);
        mesh.draw();
        ofPopMatrix();
        
        glDisable(GL_DEPTH_TEST);
    }
    
}


void WireMesh::resetMesh(){
    mesh.clear();
    createMesh();
    reset = false;
    
}


void WireMesh::createMesh(){
    mesh.setMode(OF_PRIMITIVE_LINES);
    
    mesh.enableColors();
    mesh.enableIndices();
    
    
    
    for(int i = 0; i < numVerts; i++){
        ofColor c;
        
        float z =ofRandom(-1,1);
        ofVec3f pos(ofRandom(-0.9, 0.9), ofRandom(-0.9, 0.9), z);
        mesh.addVertex(pos);
        
        if(z > 0.3) c = ofColor(255);
        else if(z < -0.3) c = ofColor(80);
        else c = ofColor(175);
        
        colors.push_back(c);
        mesh.addColor(c);
        
    }
    
    
    
    float connectionDistance = threshold;
    for (int a=1; a<numVerts; ++a) {
        ofVec3f verta = mesh.getVertex(a);
        for (int b=a+1; b<numVerts; ++b) {
            ofVec3f vertb = mesh.getVertex(b);
            float distance = verta.distance(vertb);
            if (distance <= connectionDistance && ofRandom(1) > 0.5) {
                mesh.addIndex(a);
                mesh.addIndex(b);
            }
        }
    }
    
    
}


void WireMesh::createVertex(){
    for(int i = 0; i < 10; i ++){
        
        ofColor c;
        
        float z =ofRandom(-1,1);
        ofVec3f pos(ofRandom(-0.9, 0.9), ofRandom(-0.9, 0.9), z);
        mesh.addVertex(pos);
        
        if(z > 0.3) c = ofColor(255);
        else if(z < -0.3) c = ofColor(80);
        else c = ofColor(175);
        
        colors.push_back(c);
        mesh.addColor(c);
        
        numVerts+= 1;
        
        
        
        float connectionDistance = threshold;
        for (int a=1; a<numVerts; ++a) {
            ofVec3f verta = mesh.getVertex(a);
            ofVec3f vertb = mesh.getVertex(numVerts-1);
            float distance = verta.distance(vertb);
            if (distance <= connectionDistance && ofRandom(1) > 0.5) {
                mesh.addIndex(a);
                mesh.addIndex(numVerts-1);
            }
        }
    }
    createVert = false;
}



