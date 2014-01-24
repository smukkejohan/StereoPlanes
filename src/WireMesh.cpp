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
        if(abs(dancerPos.x - vert.x) < shadowX && abs(dancerPos.y - vert.y) < shadowY){
            mesh.setColor(i, ofColor(shadowColor));
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
    
    if(changeSurface) mySurface = 1;
    else mySurface = 0;
    
    
    if(shadInc)shadowIncrease();
    if(shadDec)shadowDecrease();
    if(bCloser)blackCloser();
    if(bFurther)blackFurther();
    if(wCloser)whiteCloser();
    if(wFurther)whiteFurther();
    if(lightBg)lightenBackground();
    if(darkBg)darkenBackground();
    if(speedDec)speedDecrease();
    if(speedInc)speedIncrease();
    
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
    gui->addSlider(indexStr+"Shadow Color",  0, 255, &shadowColor);
    gui->addSlider(indexStr+"White Mesh Z",  -5, 5, &whiteMesh);
    gui->addSlider(indexStr+"Black Mesh Z",  -5, 5, &blackMesh);
    gui->addSlider(indexStr+"Shadow X", 0.1, 4, &shadowX);
    gui->addSlider(indexStr+"Shadow Y", 0.1, 4, &shadowY);
    gui->addSlider(indexStr+"Number of Vertices",  100, 600, &numVerts);
    gui->addSlider(indexStr+"Connection Threshold",  0.1, 1, &threshold);
    
    
    gui->addToggle(indexStr+"Surface", &changeSurface);
    gui->addToggle(indexStr+"Create Vertex", &createVert);
    gui->addToggle(indexStr+"Reset Mesh", &reset);
    
    
    gui->addSlider(indexStr+"Animation speed", 0.0001, 0.02, &sliderspeed);
    gui->addToggle(indexStr+"Decrease shadow", &shadDec);
    gui->addToggle(indexStr+"Increase shadow", &shadInc);
    gui->addToggle(indexStr+"Take White further", &wFurther);
    gui->addToggle(indexStr+"Bring White closer", &wCloser);
    gui->addToggle(indexStr+"Take Black further", &bFurther);
    gui->addToggle(indexStr+"Bring Black closer", &bCloser);
    gui->addToggle(indexStr+"Lighten Background", &lightBg);
    gui->addToggle(indexStr+"Darken Background", &darkBg);
    gui->addToggle(indexStr+"Increase Speed", &speedInc);
    gui->addToggle(indexStr+"Decrease Speed", &speedDec);
    
    
}



void WireMesh::draw(int _surfaceId ) {
    ofDisableLighting();
    if (_surfaceId == mySurface) {
        
        glEnable(GL_DEPTH_TEST);
        
        ofBackground(bgColor);
        ofPushMatrix();
        ofTranslate(offset.x, offset.y, offset.z);
        
        ofSetLineWidth(lineWidth);
        mesh.draw();
        ofPopMatrix();
        
        glDisable(GL_DEPTH_TEST);
    }
    ofEnableLighting();
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
        ofVec3f pos(ofRandom(-0.9, 0.9), ofRandom(-2, 0.9), z);
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


void WireMesh::shadowIncrease(){
    shadowX += sliderspeed;
    shadowY += sliderspeed;
    
}

void WireMesh::shadowDecrease(){
    
    shadowX -= sliderspeed;
    shadowY -= sliderspeed;
}

void WireMesh::whiteCloser(){
    whiteMesh-= sliderspeed;
    
}
void WireMesh::whiteFurther(){
    whiteMesh+= sliderspeed;
    
}

void WireMesh::blackCloser(){
    blackMesh-= sliderspeed;
    
}

void WireMesh::blackFurther(){
    blackMesh+= sliderspeed;
    
}


void WireMesh::lightenBackground(){
    bgColor+= sliderspeed*500;
    
}

void WireMesh::darkenBackground(){
    bgColor-= sliderspeed*500;
    
}


void WireMesh::speedIncrease(){
    speed += 0.000002;
}

void WireMesh::speedDecrease(){
    speed -= 0.000002;
}

void WireMesh::receiveOsc(ofxOscMessage * m, string rest) {
    
    if(rest == "/Offset/x" ) {
        offset.x = m->getArgAsFloat(0);
        
    } if(rest == "/Offset/y"){
        offset.y = m->getArgAsFloat(0);
        
    } else if(rest == "/dancer/x" ) {
        dancerPos.x = m->getArgAsFloat(0);
        
    } else if(rest == "/dancer/y"){
        dancerPos.y = m->getArgAsFloat(0);
        
    } else if(rest == "/shadowSize/x" ) {
        shadowX = m->getArgAsFloat(0);
        
    } else if(rest == "/shadowSize/y"){
        shadowY = m->getArgAsFloat(0);
        
    } else if(rest == "/OffsetZ/x"){
        offset.z = m->getArgAsFloat(0);
        
    } else if(rest == "/surface/x"){
        if(m->getArgAsFloat(0) == 0){
            changeSurface = false;
        }
        else if(m->getArgAsFloat(0) == 1){
            changeSurface = true;
        }
        
    } else if(rest == "/linewidth/x"){
        lineWidth = m->getArgAsFloat(0);
    }
    else if(rest == "/animationSpeed/x"){
        sliderspeed = m->getArgAsFloat(0);
    }
    else if(rest == "/numvert/x"){
        numVerts = m->getArgAsFloat(0);
    }
    else if(rest == "/speed/x"){
        speed = m->getArgAsFloat(0);
    }
    else if(rest == "/shadowcolor/x"){
        shadowColor = m->getArgAsFloat(0);
    }
    else if(rest == "/backgroundcolor/x"){
        bgColor = m->getArgAsFloat(0);
    }
    
    else if(rest == "/shadowDec/x"){
        if(m->getArgAsFloat(0) == 0){
            shadDec = false;
        }
        else if(m->getArgAsFloat(0) == 1){
            shadDec = true;
        }
    }
    else if(rest == "/shadowInc/x"){
        if(m->getArgAsFloat(0) == 0){
            shadInc = false;
        }
        else if(m->getArgAsFloat(0) == 1){
            shadInc = true;
        }
    }
    else if(rest == "/lightbg/x"){
        if(m->getArgAsFloat(0) == 0){
            lightBg = false;
        }
        else if(m->getArgAsFloat(0) == 1){
            lightBg = true;
        }
    }
    else if(rest == "/darkbg/x"){
        if(m->getArgAsFloat(0) == 0){
            darkBg = false;
        }
        else if(m->getArgAsFloat(0) == 1){
            darkBg = true;
        }
    }
    else if(rest == "/whitecloser/x"){
        if(m->getArgAsFloat(0) == 0){
            wCloser = false;
        }
        else if(m->getArgAsFloat(0) == 1){
            wCloser = true;
        }
    }
    else if(rest == "/whitefurther/x"){
        if(m->getArgAsFloat(0) == 0){
            wFurther = false;
        }
        else if(m->getArgAsFloat(0) == 1){
            wFurther = true;
        }
    }
    else if(rest == "/blackcloser/x"){
        if(m->getArgAsFloat(0) == 0){
            bCloser = false;
        }
        else if(m->getArgAsFloat(0) == 1){
            bCloser = true;
        }
    }
    else if(rest == "/blackfurther/x"){
        if(m->getArgAsFloat(0) == 0){
            bFurther = false;
        }
        else if(m->getArgAsFloat(0) == 1){
            bFurther = true;
        }
    }
    else if(rest == "/reset/x"){
        if(m->getArgAsFloat(0) == 1){
            resetMesh();
        }
    }
    else if(rest == "/create/x"){
        if(m->getArgAsFloat(0) == 1){
            createVertex();
        }
    }
    else if(rest == "/whiteMesh/x"){
        whiteMesh = m->getArgAsFloat(0);
        cout << whiteMesh << endl;
        
    }
    else if(rest == "/shadowMesh/x"){
        blackMesh = m->getArgAsFloat(0);
    }
    
    
}
