//
//  Lights.h
//  Trae
//
//  Created by Ole Kristensen on 28/03/14.
//
//

#pragma once
#include "ofMain.h"
#include "ContentScene.h"
#include "ofxOlaShaderLight.h"

class FlyLight : public ContentScene {
    
public:
    void draw(int _surfaceId);
    void update();
    
    void setLightPtr(ofxOlaShaderLight * _light);
    
    void setup();
    
    void beginWorld(int _surfaceId);
    void endWorld(int _surfaceId);
    
    void setMaterial(ofxOlaShaderLight::Material m);
    void setNoisePoints(ofxOlaShaderLight::NoisePoints n);
    
    void setGui(ofxUICanvas * gui, float width);
    void guiEvent(ofxUIEventArgs &e);
    void receiveOsc(ofxOscMessage * m, string rest);
    
    ofxUICanvas * gui;
    
    // Lights
    ofxOlaShaderLight * flyLight;
    //ofxOlaShaderLight::Material material;
    
    // Fly
    ofxTLColorTrack * flyLightColor;
    ofxTLCurves     * flyLightAttenuation;
    ofxTLCurves     * flyLightPosX;
    ofxTLCurves     * flyLightPosY;
    ofxTLCurves     * flyLightPosZ;
    ofxTLCurves     * flyLightPosNoise;
    ofxTLCurves     * flyLightPosNoiseSpeed;
    
    bool drawLights = false;
    
};
