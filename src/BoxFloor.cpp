//
//  BoxWall.cpp
//  StereoPlanes
//
//  Created by Bernardo Schorr on 1/16/14.
//
//

#include "BoxFloor.h"

void BoxFloor::setup(ofParameterGroup * params) {
    
    params->add(waveHeight.set("Boxes Height", 0.5, 0, 1));
    params->add(speed.set("Boxes Speed", 0.0003, 0, 0.001));
    params->add(plateauHeight.set("Plateau Height", 0.5, 0, 1));
    params->add(lightHeight.set("Light Height", -0.3, -1.0, 0));
    
    light.setPointLight();
    light.setPosition(0, 0, lightHeight);
    
}

void BoxFloor::update() {
    
    light.setPosition(0, 0, lightHeight);

}


void BoxFloor::draw( ofVec2f dancerPos ) {
    
    glEnable(GL_DEPTH_TEST);
    //ofEnableLighting();
    light.enable();
    
    //h = 100 * sin (ofGetElapsedTimeMillis() * 0.001);
    
    //cam.begin();
    
    for (float x = -1; x < 1; x += 0.1) {
        for (float y = -1; y < 1; y += 0.1) {
            
            float tempNoise = ofNoise(x + ofGetElapsedTimeMillis()*speed, y);
            
            if (ofDist(dancerPos.x, dancerPos.y, x, y) > 0.2) {
                ofDrawBox(x, y, 0, 0.09, 0.09, waveHeight*tempNoise);
            } else {
                ofDrawBox(x, y, 0, 0.09, 0.09, plateauHeight);
            }
        }
    }
    
    //cam.end();
    
    light.disable();
    //ofDisableLighting();
    glDisable(GL_DEPTH_TEST);
    
}