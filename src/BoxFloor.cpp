//
//  BoxWall.cpp
//  StereoPlanes
//
//  Created by Bernardo Schorr on 1/16/14.
//
//

#include "BoxFloor.h"

void BoxFloor::setup() {
    
//    params->add(surfaceHeight.set("Surface Height", 0.0, -2.0, 2.0));
//    params->add(waveHeight.set("Boxes Height", 0.5, 0, 1));
//    params->add(speed.set("Boxes Speed", 0.0003, 0, 0.001));
//    params->add(plateauHeight.set("Plateau Height", 0.5, 0, 1));
//    params->add(lightHeight.set("Light Height", -0.3, -1.0, 0));
//    params->add(distance.set("Distance", 0.2, 0, 0.6));
//    params->add(bRotation.set("Rotation", false, false, true));
    
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
    
    if (_surfaceId == 0) {
    
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
