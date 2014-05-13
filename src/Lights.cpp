//
//  Lights.cpp
//  Trae
//
//  Created by Ole Kristensen on 28/03/14.
//
//

#include "Lights.h"


void Lights::setup() {
    
    name = "World Light";
    oscAddress = "/lights";
    
    mainTimeline->addPage(name);
    
    //lightShading = mainTimeline->addCurves("Shading", ofRange(0,3));
    
    /*flyLightColor = mainTimeline->addColors("Fly Light Color");
    flyLightAttenuation = mainTimeline->addCurves("Fly Light Attenuation", ofRange(0.001,10.0));
    flyLightPosX = mainTimeline->addCurves("Fly Light Pos X", ofRange(-3.0, 3.0));
    flyLightPosY = mainTimeline->addCurves("Fly Light Pos Y", ofRange(-3.0, 3.0));
    flyLightPosZ = mainTimeline->addCurves("Fly Light Pos Z", ofRange(-3.0, 9.0));*/
    
    //flyLightPosNoise = mainTimeline->addCurves("Fly Light Pos Noise", ofRange(0.0, 1.0));
    //flyLightPosNoiseSpeed = mainTimeline->addCurves("Fly Light Pos Noise Speed", ofRange(-1.0, 1.0));
    
    //flyLightDotColor = mainTimeline->addColors("Fly Light Dot Color");
    //flyLightDotSize = mainTimeline->addCurves("Fly Light Dot Size", ofRange(0.0, 1.0));
    
    //lightsVertexNoise = mainTimeline->addCurves("Vertex Noise", ofRange(0.0,1.0));
    skyLightAttenuation = mainTimeline->addCurves("Sky Light Attenuation", ofRange(0.001,40.0));    
    skyLightColor = mainTimeline->addColors("Sky Light Color");

    material.diffuseColor = ofVec4f(1.0, 1.0, 1.0, 1.0);
    material.specularColor = ofVec4f(1.0, 1.0, 1.0, 1.0);
    material.specularShininess = 2.5;
    
    /*flyLight.setNormalisedBrightness(1.0);
    flyLight.setAttenuation(1.0/2.);
    flyLight.setTemperature(4200);
    flyLight.setSpotConcentration(0.1);
    */
    
    skyLight.setNormalisedBrightness(1.0);
    skyLight.setAttenuation(1./10.);
    skyLight.setTemperature(20000);
    
    skyLight2.setNormalisedBrightness(1.0);
    skyLight2.setAttenuation(1./10.);
    skyLight2.setTemperature(20000);
    
}

void Lights::beginWorld(int _surfaceId){
    ofxOlaShaderLight::begin();
    ofxOlaShaderLight::setMaterial(material);
    
    //flyLight.setDiffuseColor(flyLightColor->getColor());
    //flyLight.setAttenuation(1./flyLightAttenuation->getValue());
    
    skyLight.setDiffuseColor(skyLightColor->getColor());
    skyLight.setAttenuation(1./skyLightAttenuation->getValue());
    
    skyLight2.setDiffuseColor(skyLightColor->getColor());
    skyLight2.setAttenuation(1./skyLightAttenuation->getValue());
    
}

void Lights::endWorld(int _surfaceId){
    ofxOlaShaderLight::end();
}

void Lights::draw(int _surfaceId) {
    
    /*if(drawLight) {
    bool lightWasEnabled = ofxOlaShaderLight::isEnabled();
    if(lightWasEnabled){
        ofxOlaShaderLight::end();
    }
    ofSetColor(ofColor(255,0,0));
    ofDrawSphere(flyLight.getGlobalPosition(), 0.1);
    
    if(lightWasEnabled){
        ofxOlaShaderLight::begin();
    }
    }*/
}

void Lights::update() {
    
    //float flyTime = mainTimeline->getCurrentTime() * flyLightPosNoiseSpeed->getValue();
    
    //float zPos =ofSignedNoise(0,0,flyTime);
    //float reduction = fmaxf(0,ofMap(zPos, 1, -1, 0.0, 1));
    //reduction = pow(reduction, 3);
    
    //ofVec3f posNoise(ofSignedNoise(flyTime),
    //                 ofSignedNoise(0,flyTime),
    //                 zPos
    //                 );
    
    /*ofVec3f pos = (ofVec3f(
                           flyLightPosX->getValue(),
                           flyLightPosY->getValue(),
                           flyLightPosZ->getValue()
                           ));
    
    flyLight.setGlobalPosition(pos);
    */
    skyLight.setGlobalPosition(0,-4,-0.5);
    skyLight2.setGlobalPosition(0,-4,0.5);
    
    ofxOlaShaderLight::setShadingType(ofxOlaShaderLight::OFX_OLA_SHADER_LIGHT_GOURAUD);
    
    /*switch (int(lightShading->getValue())) {
        case 0:
            ofxOlaShaderLight::setShadingType(ofxOlaShaderLight::OFX_OLA_SHADER_LIGHT_FLAT);
            break;
        case 1:
            ofxOlaShaderLight::setShadingType(ofxOlaShaderLight::OFX_OLA_SHADER_LIGHT_GOURAUD);
            break;
        case 2:
            ofxOlaShaderLight::setShadingType(ofxOlaShaderLight::OFX_OLA_SHADER_LIGHT_PHONG);
            break;
        default:
            ofxOlaShaderLight::setShadingType(ofxOlaShaderLight::OFX_OLA_SHADER_LIGHT_PHONG);
            break;
    }*/
    
    ofxOlaShaderLight::update();
}

void Lights::setGui(ofxUICanvas * gui, float width){
    this->gui = gui;
    ContentScene::setGui(gui, width);
}

void Lights::receiveOsc(ofxOscMessage * m, string rest) {
    
}

void Lights::guiEvent(ofxUIEventArgs &e)
{
    string name = e.getName();
	int kind = e.getKind();
	//cout << "got event from: " << name << endl;
}