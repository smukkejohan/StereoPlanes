//
//  attractorControl.cpp
//  StereoPlanes
//
//  Created by Bernardo Schorr on 1/17/14.
//
//

#include "AttractorControl.h"

void AttractorControl::setup() {
    
    name = "Attractor Control";
    oscAddress = "/attractorcontrol";
    
    damping = 4.0;
    acceleration = 1.05;
    multiplier = 2.0;
    bkgColor = 255.0;
    bkgTransparency = 7.0;
    particleColor = 255.0;
    createStructure = false;
    hideDancer = false;
    angleMovement = false;
    freeze = false;
    
    attractorPos.set(ofRandom(-1, 1), ofRandom(-1, 1), ofRandom(-1, 1));
    
    ofVec3f particleStartPos = attractorPos;
    
    ofVec3f particleStartVel = ofVec3f (0, 0, 0);
    
    for( int i=0; i<60; i++){
        AttractorParticle myParticle;
        myParticle.setup( particleStartPos, particleStartVel );
        particleList.push_back( myParticle );
    }
    
    explosionCountdown = 60;
    finalExplosionCountdown = 0;
    
    localLight.setPointLight();
    localLight.setPosition(0, 0, -15);
    
}

void AttractorControl::update() {
    
    ofBackground(bkgColor, bkgTransparency);
    //cout << ofGetCurrentRenderer()->getBgColor() << endl;
    
    for( int i = 0; i < particleList.size(); i++ ){
        if (!freeze) particleList[i].update( damping, acceleration, ofVec3f(dancerPos.x, dancerPos.y, 0));
    }
    
    if ( createStructure ) {
        acceleration = 0.0;
        
        if (explosionCountdown%12== 0) {
            for( int i = 0; i < particleList.size(); i++ ){
                particleList[i].velocity.set(ofRandom(-0.02, 0.02), ofRandom(-0.02, 0.02), ofRandom(-0.015, 0.0));
            }
            
        }
        
        if (explosionCountdown < 0) {
            
            createStructure = false;
            explosionCountdown = 60;
            freeze= true;
            acceleration = 1.05;
            
            for( int i = 0; i < particleList.size(); i++ ){
                particleList[i].velocity.set(0, 0, 0);
            }
            
        } else {
            explosionCountdown--;
        }
    }
    
    if (hideDancer) {
        
        acceleration = 10.0;
        bkgTransparency = 0.0;
        
        if (finalExplosionCountdown%12== 0) {
            for( int i = 0; i < particleList.size(); i++ ){
                particleList[i].velocity.set(ofRandom(-0.04, 0.04), ofRandom(-0.04, 0.04), ofRandom(-0.04, 0.04));
            }
        
        }
        
        finalExplosionCountdown++;
        
    }
    
    if (angleMovement) {
        hideDancer = false;
        acceleration = 0.0;
        bkgTransparency = 10.0;
        
        if (finalExplosionCountdown%24== 0) {
            for( int i = 0; i < particleList.size(); i++ ){
                particleList[i].velocity.set(ofRandom(-0.02, 0.02), ofRandom(-0.02, 0.02), ofRandom(-0.02, 0.02));
            }
        }
        
        finalExplosionCountdown++;
    }
}

void AttractorControl::draw( int _surfaceId ) {
    
    if ( _surfaceId == 1 ) {
    
        ofDisableLighting();
        
        if (!freeze) {
            
            ofSetColor ( particleColor );
            
            for( int i = 0; i < particleList.size(); i++ ){
                particleList[i].draw();
            }
        } else {
            ofBackground(0, 0);
        }
        
    }
}

void AttractorControl::setGui(ofxUICanvas * gui, float width){
    ContentScene::setGui(gui, width);
    
    gui->addSlider(indexStr+"Damping", 1.0, 10.0, &damping);
    gui->addSlider(indexStr+"Acceleration", 1.0, 10.0, &acceleration);
    gui->addSlider(indexStr+"Multiplier", 0.0, 5.0, &multiplier);
    gui->addSlider(indexStr+"Background Color", 0.0, 255.0, &bkgColor);
    gui->addSlider(indexStr+"Background Transparency", 0.0, 255.0, &bkgTransparency);
    gui->addSlider(indexStr+"Particle Color", 0.0, 255.0, &particleColor);
    
    //    params->add(damping.set("Damping", 4.0, 1.0, 10.0));
    //    params->add(acceleration.set("Acceleration", 1.05, 1.0, 10.0));
    //    params->add(multiplier.set("Explosion Size", 2.0, 0.0, 5.0));
    //    params->add(bkgColor.set("Bkg Color", 255.0, 0.0, 255.0));
    //    params->add(bkgTransparency.set("Bkg Transparency", 7.0, 0.0, 255.0));
    //    params->add(particleColor.set("Particle Color", 0.0, 255.0, 255.0));
    //    params->add(explode.set("Explode", false, false, true));
    //    params->add(finalExplode.set("Final Explode", false, false, true));
    //    params->add(angleMovement.set("Angle Movement", false, false, true));
    //    params->add(showAttractor.set("Show Attractor", false, false, true));
    //    params->add(freeze.set("Freeze", false, false, true));
    
    gui->addToggle(indexStr+"Create Structure", &createStructure);
    gui->addToggle(indexStr+"Dissolve Back", &freeze);
    gui->addToggle(indexStr+"Hide Dancer", &hideDancer);
    gui->addToggle(indexStr+"Angle Movement", &angleMovement);
    
}