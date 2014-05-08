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

class Lights : public ContentScene {
    
public:
    void draw(int _surfaceId);
    void update();
    void setup();
    
    void begin();
    void end();
    
    void updateCamPos(ofVec3f p);
    
    void setMaterial(ofxOlaShaderLight::Material m);
    void setNoisePoints(ofxOlaShaderLight::NoisePoints n);
    
    void setGui(ofxUICanvas * gui, float width);
    void guiEvent(ofxUIEventArgs &e);
    void receiveOsc(ofxOscMessage * m, string rest);
    
    ofxUICanvas * gui;
    
    // Lights
    ofxOlaShaderLight flyLight;
    ofxOlaShaderLight skyLight;
    ofxOlaShaderLight::Material material;
    
    ofxOlaShaderLight::NoisePoints noisePoints;
    
    ofxTLCurves     * lightShading;
    ofxTLCurves     * lightsVertexNoise;
    
    // Fly
    
    ofxTLColorTrack * flyLightColor;
    ofxTLCurves     * flyLightAttenuation;
    ofxTLCurves     * flyLightPosX;
    ofxTLCurves     * flyLightPosY;
    ofxTLCurves     * flyLightPosZ;
    ofxTLCurves     * flyLightPosNoise;
    ofxTLCurves     * flyLightPosNoiseSpeed;
    
    ofxTLColorTrack * flyLightDotColor;
    ofxTLCurves     * flyLightDotSize;
    
    ofVec3f camPos;
    
    //Sky
    
    ofxTLColorTrack * skyLightColor;
    ofxTLCurves     * skyLightAttenuation;
    
};
