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
    
    if (!started) {
        damping = 4.0;
        acceleration = 1.05;
        multiplier = 2.0;
        bkgColor = 255.0;
        bkgTransparency = 7.0;
        particleColor = 0.0;
        bCreateStructure = false;
        bHideDancer = false;
        bAngleMovement = false;
        bFreeze = false;
        explosionSize = 0.005;
        
    }
    
    //cout << ofGetCurrentRenderer()->getBgColor() << endl;
    
    for( int i = 0; i < particleList.size(); i++ ){
        if (!bFreeze) particleList[i].update( damping, acceleration, ofVec3f(dancerPos.x, dancerPos.y, 0));
    }
    
    if ( bCreateStructure ) {
        
        bAngleMovement = false;
        acceleration = 0.0;
        
        if (explosionCountdown%12== 0) {
            for( int i = 0; i < particleList.size(); i++ ){
                particleList[i].velocity.set(ofRandom(-explosionSize, explosionSize), ofRandom(-explosionSize, explosionSize), ofRandom(-0.015, 0.0));
            }
            
        }
        
        if (explosionCountdown < 0) {
            
            bCreateStructure = false;
            explosionCountdown = 60;
            bFreeze= true;
            acceleration = 1.05;
            
            for( int i = 0; i < particleList.size(); i++ ){
                particleList[i].velocity.set(0, 0, 0);
            }
            
        } else {
            explosionCountdown--;
        }
    }
    
    if (bHideDancer) {
        
        acceleration = 10.0;
        bkgTransparency = 0.0;
        
        if (finalExplosionCountdown%12== 0) {
            for( int i = 0; i < particleList.size(); i++ ){
                particleList[i].velocity.set(ofRandom(-0.04, 0.04), ofRandom(-0.04, 0.04), ofRandom(-0.04, 0.04));
            }
        
        }
        
        finalExplosionCountdown++;
        
    }
    
    if (bAngleMovement) {
        bHideDancer = false;
        acceleration = 0.0;
        bkgTransparency = 10.0;
        
        if (finalExplosionCountdown%24== 0) {
            for( int i = 0; i < particleList.size(); i++ ){
                particleList[i].velocity.set(ofRandom(-0.02, 0.02), ofRandom(-0.02, 0.02), ofRandom(-0.01, 0.02));
            }
        }
        
        finalExplosionCountdown++;
    }
}

void AttractorControl::draw( int _surfaceId ) {
    
    if (!started) {
        ofBackground(255);
        started = true;
        
    } else {
    ofBackground(bkgColor, bkgTransparency);
    }
    
    if ( _surfaceId == 1 ) {
    
        ofDisableLighting();
        
        if (!bFreeze) {
            
            ofSetColor ( particleColor );
            
            for( int i = 0; i < particleList.size(); i++ ){
                particleList[i].draw();
            }
        } else {
            ofBackground(255, 0);
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
    
    gui->addToggle(indexStr+"Create Structure", &bCreateStructure);
    gui->addToggle(indexStr+"Dissolve Back", &bFreeze);
    gui->addToggle(indexStr+"Hide Dancer", &bHideDancer);
    gui->addToggle(indexStr+"Angle Movement", &bAngleMovement);
    
    
}

void AttractorControl::guiEvent(ofxUIEventArgs &e)
{
    
    string name = e.getName();
	int kind = e.getKind();
	//cout << "got event from: " << name << endl;
    
}

void AttractorControl::receiveOsc(ofxOscMessage * m, string rest) {
    
    if(rest == "/dancerpos/x" ) {
        dancerPos.x = m->getArgAsFloat(0);
        
    } else if(rest == "/dancerpos/y" ) {
        dancerPos.y = m->getArgAsFloat(0);
        
    } else if(rest == "/damping/x" ) {
        damping = m->getArgAsFloat(0);
        
    } else if(rest == "/acceleration/x"){
        multiplier = m->getArgAsFloat(0);
        
    } else if(rest == "/bkgcolor/x"){
        bkgColor = m->getArgAsFloat(0);
        
    } else if(rest == "/bkgtransparency/x"){
        bkgTransparency = m->getArgAsFloat(0);
        
    } else if(rest == "/particleColor/x"){
        particleColor = m->getArgAsFloat(0);
        
        
    } else if(rest == "/createstructure/x"){
        bCreateStructure = true;
        
    } else if(rest == "/hidedancer/x"){
        bFreeze = false;
        bHideDancer = true;
    
    } else if(rest == "/anglemovement/x"){
        
//        for( int i = 0; i < particleList.size(); i++ ){
//         
//            particleList[i].pos.set(ofRandom(-1, 1), ofRandom(-1, 1), ofRandom(0, 1));
//            
//        }
        bFreeze = false;
        bAngleMovement = true;
    
    } else if(rest == "/freeze/x"){
        
        for( int i = 0; i < particleList.size(); i++ ){

            particleList[i].velocity.set(0, 0, 0);

        }
        
        acceleration = 1.05;
        bkgTransparency = 7.0;
        
        bFreeze = false;
        bAngleMovement = false;
        bHideDancer = false;
    
    } else if (rest == "/explosionsize/x") {
        explosionSize = m->getArgAsFloat(0);
        
    }
    
    
}
