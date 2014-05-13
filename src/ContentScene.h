//
//  ContentScene.cpp
//  StereoPlanes
//
//  Created by Johan Bichel Lindegaard on 21/01/14.
//
//

#pragma once
#include "ofMain.h"
#include "ofxUI.h"
#include "ofxOsc.h"
#include "ofxTimeline.h"

class ContentScene {
    
public:
    
    int index;
    string indexStr;
    string name = "";
    string oscAddress = "/default";
    
    bool enabled;
    float time;
    
    ofxTimeline * mainTimeline;
    ofxTLCurves * tlenabled;
    
    ContentScene() {
    };
    
    virtual ~ContentScene(){}
    
    virtual void debugDraw(int _surfaceId=0) {};
    virtual void exit(){};
    virtual void receiveOsc(ofxOscMessage * m, string rest) {};
    
    virtual void setGui(ofxUICanvas * gui, float width){
        gui->addWidgetDown(new ofxUILabel(name, OFX_UI_FONT_SMALL));
        gui->addWidgetDown(new ofxUILabel("OSC Address: " + oscAddress, OFX_UI_FONT_SMALL));
        gui->addSpacer(width, 1);
        //gui->addToggle(indexStr+"Enabled", &enabled);
    }
    
    virtual void guiEvent(ofxUIEventArgs &e) {};
    
    void setName(string _name) {
        setName(_name, "/"+_name);
    }
    
    void setName(string _name, string _osc) {
        name = _name;
        oscAddress = _osc;
    }
    
    void setupScene(int _index) {
        if(name == "") setName("scene-" + ofToString(_index));
        
        index = _index;
        indexStr = "["+ ofToString(_index) + "] ";

        setup();
        
        mainTimeline->setCurrentPage("Page One");
        tlenabled = mainTimeline->addCurves("Enable " + name);

    };
    
    void updateScene() {
        
        enabled = (tlenabled->getValue() == 1);
        
        if(enabled) {
            update();
        }
    };
    
    void drawScene(int _surfaceId=0) {
        if(enabled) {
            glPushMatrix();ofPushMatrix();ofPushStyle();
            draw(_surfaceId);
            ofPopStyle();ofPopMatrix();glPopMatrix();
            
            glPushMatrix();ofPushMatrix();ofPushStyle();
            debugDraw(_surfaceId);
            ofPopStyle();ofPopMatrix();glPopMatrix();
        }
    };
    void beginSceneWorld(int _surfaceId=0){
        if(enabled) {
            beginWorld(_surfaceId);
        }
    };
    void endSceneWorld(int _surfaceId=0){
        if(enabled) {
            endWorld(_surfaceId);
        }
    };
    
    void parseSceneOsc(ofxOscMessage * m){
        
        vector<string> adrSplit = ofSplitString(m->getAddress(), "/");
        string rest = ofSplitString(m->getAddress(), "/"+adrSplit[1])[1];
        //cout<<adrSplit[1]<<"   "<<rest<<endl;
        //cout << adrSplit[0] << " " << adrSplit[1] << " " <<adrSplit[2] << endl;
        
        if(adrSplit[1] == "scene"+ofToString(index) || "/"+adrSplit[1] == oscAddress) {
            if(rest == "/enable/x" ) {
                enabled = m->getArgAsInt32(0);
            }
            receiveOsc(m, rest);
        }
    }
    
    
    void setSurface(int _surfaceId) {
        primarySurface = _surfaceId;
    }
    
    int primarySurface = 0;
    
private:
    virtual void setup(){};
    virtual void update(){};
    virtual void beginWorld(int _surfaceId=0){};
    virtual void endWorld(int _surfaceId=0){};
    virtual void draw(int _surfaceId=0){};
    
    
    
};