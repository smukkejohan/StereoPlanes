//
//  BoxWall.cpp
//  StereoPlanes
//
//  Created by Bernardo Schorr on 1/16/14.
//
//

#include "BoxFloor.h"

void BoxFloor::setup() {
    
    name = "Box Floor";
    oscAddress = "/boxfloor";
    
    boxLight.setPointLight();
    boxLight.setPosition(0, 0, -1);
    
    for (int i = 0; i < 400; i++) {
        rotation.push_back(ofRandom(90));
    }
    
}

void BoxFloor::update() {
    boxLight.setPosition(0, 0, lightHeight);
}


void BoxFloor::draw( int _surfaceId ) {
    
    ofBackground(0);
    
    if (_surfaceId == 0) {
    
        ofEnableLighting();
        boxLight.enable();

        for (float x = -1; x < 1; x += 0.1) {
            for (float y = -1; y < 1; y += 0.1) {
                
                float tempNoise = ofNoise(x + ofGetElapsedTimeMillis()*speed, y);
                
                ofPushMatrix();
                ofTranslate(x, y, 0);
                if (bRotation)ofRotateZ (rotation[((x+1) * 10 * 20) + ((y+1) * 10)]);
                    if (ofDist(dancerPos.x, dancerPos.y, x, y) > platformRadius) {
                        ofDrawBox(0, 0, surfaceHeight, 0.09, 0.09, waveHeight*tempNoise);
                    } else {
                        ofDrawBox(0, 0, surfaceHeight, 0.09, 0.09, plateauHeight);
                    }
                ofPopMatrix();
            }
        }
        
        boxLight.disable();
        ofDisableLighting();
    
    }
    
}

void BoxFloor::setGui(ofxUICanvas * gui, float width){
    ContentScene::setGui(gui, width);
    
    gui->addSlider(indexStr+"Surface Height", -2.0, 2.0, &surfaceHeight);
    gui->addSlider(indexStr+"Wave Height", 0.0, 1.0, &waveHeight);
    gui->addSlider(indexStr+"Speed", 0.0, 0.001, &speed);
    gui->addSlider(indexStr+"Plateau Height", 0.0, 1.0, &plateauHeight);
    gui->addSlider(indexStr+"Light Position", -1.0, 0.0, &lightHeight);
    gui->addSlider(indexStr+"Platform Radius", 0, 0.6, &platformRadius);
    
    gui->addToggle(indexStr+"Animate", &bRotation);
    
    
}

void BoxFloor::guiEvent(ofxUIEventArgs &e)
{
    
    string name = e.getName();
	int kind = e.getKind();
	//cout << "got event from: " << name << endl;
    
}

void BoxFloor::receiveOsc(ofxOscMessage * m, string rest) {
    
    //cout << "yay" << endl;
    //cout << m->getAddress() << endl;
    //cout << rest << endl;
        
    if(rest == "/dancerpos/x" ) {
        dancerPos.x = m->getArgAsFloat(0);

    } else if(rest == "/dancerpos/y" ) {
        dancerPos.y = m->getArgAsFloat(0);
    
    } else if(rest == "/surfaceheight/x" ) {
        surfaceHeight = m->getArgAsFloat(0);
    
    } else if(rest == "/waveheight/x"){
        waveHeight = m->getArgAsFloat(0);
        
    } else if(rest == "/speed/x"){
        speed = m->getArgAsFloat(0);
        
    } else if(rest == "/plateauheight/x"){
        plateauHeight = m->getArgAsFloat(0);
        
    } else if(rest == "/lightheight/x"){
        lightHeight = m->getArgAsFloat(0);
        
    } else if(rest == "/plateauradius/x"){
        platformRadius = m->getArgAsFloat(0);
        
    } else if(rest == "/bRotation/x"){
        bRotation = m->getArgAsFloat(0);
    }
}

