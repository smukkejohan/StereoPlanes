//
//  WireMesh.cpp
//  StereoPlanes
//
//  Created by Maria Paula Saba on 1/17/14.
//
//

#include "WireMesh.h"

void WireMesh::setup(ofParameterGroup * params) {
    

    params->add(lineWidth.set("Line Width", 1, 0.1, 10));
    params->add(offset.set("Offset", ofVec3f(0, 0, 1), ofVec3f(-4,-4,-4), ofVec3f(4,4,4)));
    params->add(speed.set("Speed", 0.0005, 0, 0.002));
    params->add(bgColor.set("Background Color", 150, 0, 255));
    params->add(whiteMesh.set("White Z Axis", 1, -5, 5));
    params->add(blackMesh.set("Black Z Axis", 0.00000001, -5, 5));
    params->add(shadowSize.set("Shadow Size", 0.4, 0.1, 3));
    
    params->add(triangles.set("Fill", false, false, true));
    params->add(reset.set("Reset Mesh", false, false, true));
    params->add(createVert.set("Create Vertex", false, false, true));
    params->add(numVerts.set("Number of Vertices", 250, 100, 600));
    params->add(threshold.set("Connection Threshold", 0.5, 0.1, 1));
 
    createMesh();
    
}

void WireMesh::update( ofVec2f dancerPos ) {
    
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


void WireMesh::draw() {
    
    glEnable(GL_DEPTH_TEST);
    
    //ofSetColor(bgColor);
    //ofFill();
    //ofRect(-10, -10, 5, 20, 20);

    ofPushMatrix();
    ofTranslate(offset->x, offset->y, offset->z);
    
    //ofDrawGrid(6);
    
    ofSetLineWidth(lineWidth);
    mesh.draw();
    ofPopMatrix();

    glDisable(GL_DEPTH_TEST);
    
}


void WireMesh::resetMesh(){
    mesh.clear();
    createMesh();
    reset.set(false);

}


void WireMesh::createMesh(){
    if(triangles) mesh.setMode(OF_PRIMITIVE_TRIANGLES);
    else mesh.setMode(OF_PRIMITIVE_LINES);
    
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



