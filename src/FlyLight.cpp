//
//  Lights.cpp
//  Trae
//
//  Created by Ole Kristensen on 28/03/14.
//
//

#include "FlyLight.h"

void FlyLight::setup() {
    
    mainTimeline->addPage(name);
    
    flyLightColor = mainTimeline->addColors("Fly Light Color");
    flyLightAttenuation = mainTimeline->addCurves("Fly Light Attenuation", ofRange(0.001,10.0));
    flyLightPosX = mainTimeline->addCurves("Fly Light Pos X", ofRange(-3.0, 3.0));
    flyLightPosY = mainTimeline->addCurves("Fly Light Pos Y", ofRange(-3.0, 3.0));
    flyLightPosZ = mainTimeline->addCurves("Fly Light Pos Z", ofRange(-3.0, 9.0));
    flyLightPosNoise = mainTimeline->addCurves("Fly Light Pos Noise", ofRange(0.0, 1.0));
    flyLightPosNoiseSpeed = mainTimeline->addCurves("Fly Light Pos Noise Speed", ofRange(-1.0, 1.0));
    
    material.diffuseColor = ofVec4f(1.0, 1.0, 1.0, 0.4);
    material.specularColor = ofVec4f(1.0, 1.0, 1.0, 0.6);
    material.specularShininess = 2.5;
    
    flyLight->setNormalisedBrightness(1.0);
    flyLight->setAttenuation(1.0/2.);
    flyLight->setTemperature(4200);
    flyLight->setDiffuseColor(flyLightColor->getColor());
    
}

void FlyLight::setLightPtr(ofxOlaShaderLight * _light) {
    flyLight = _light;
}

void FlyLight::beginWorld(int _surfaceId){
    
    if(_surfaceId == primarySurface) {
        

        
        ofVec3f pos = (ofVec3f(
                               flyLightPosX->getValue(),
                               flyLightPosY->getValue(),
                               flyLightPosZ->getValue()
                               ));
        
        flyLight->setGlobalPosition(pos);
        
        flyLight->setDiffuseColor(flyLightColor->getColor());
        flyLight->setAttenuation(1./flyLightAttenuation->getValue());
        flyLight->setNormalisedBrightness(2);
        


    }
    
    ofxOlaShaderLight::begin();
    ofxOlaShaderLight::setMaterial(material);
    

    
}

void FlyLight::endWorld(int _surfaceId){
    
    
    
    if(_surfaceId == primarySurface) {
        ofxOlaShaderLight::end();
        
        //ofxOlaShaderLight::end();
        
    }
}

void FlyLight::setMaterial(ofxOlaShaderLight::Material m){
    material = m;
}

void FlyLight::draw(int _surfaceId) {
    
    if(_surfaceId == primarySurface) {
        bool lightWasEnabled = ofxOlaShaderLight::isEnabled();
        if(lightWasEnabled){
            ofxOlaShaderLight::end();
        }
        
        ofSetColor(255,0,0);
        ofDrawSphere(flyLight->getGlobalPosition(), 0.4);
        
        if(lightWasEnabled){
            ofxOlaShaderLight::begin();
        }
    }
}

void FlyLight::update() {
    
    ofxOlaShaderLight::update(); //nly for dmx ? - would not work with this hacked setup
}

void FlyLight::setGui(ofxUICanvas * gui, float width){
    this->gui = gui;
    ContentScene::setGui(gui, width);
}

void FlyLight::receiveOsc(ofxOscMessage * m, string rest) {
    
}

void FlyLight::guiEvent(ofxUIEventArgs &e)
{
    string name = e.getName();
	int kind = e.getKind();
	//cout << "got event from: " << name << endl;
}