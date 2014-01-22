//
//  ContentScene.cpp
//  StereoPlanes
//
//  Created by Johan Bichel Lindegaard on 21/01/14.
//
//

#pragma once
#include "ofMain.h"

class ContentScene {
    
public:
    
    int index;
    string name;
    string oscAddress = "/default";
    
    // bool syphonDirect;
    bool enabled = true;
    
    ContentScene() {
    }
    
    virtual ~ContentScene(){}
    
    virtual void setup(){}
    virtual void update(){}
    virtual void draw(int _surfaceId=0){}
    virtual void exit(){}
    
    //virtual void parseOscMessage(ofxOscMessage * m){
        /*
        vector<string> adrSplit = ofSplitString(m->getAddress(), "/");
        string rest = ofSplitString(m->getAddress(), "/"+adrSplit[1])[1];
        //cout<<adrSplit[1]<<"   "<<rest<<endl;
        
        if(adrSplit[1] == "scene"+ofToString(index) || "/"+adrSplit[1] == oscAddress) {
            
            if( rest == "/opacity/x" ) {
                opacity = m->getArgAsFloat(0);
            } else if(rest == "/enable/x" ) {
                enabled = m->getArgAsInt32(0);
            } else if(rest == "/speed/x" ) {
                speed =m->getArgAsFloat(0);
            }
        }*/
    //}
    
    void setupScene() {
        // common setup for all scenes
        
        setup();
    }
    
    void updateScene() {
        if(enabled) {
            update();
        }
    }
    
    void drawScene(int _surfaceId=0) {
        if(enabled) {
            draw(_surfaceId);
            debugDraw(_surfaceId);
        }
    }
    
    virtual void debugDraw(int _surfaceId=0) {
    }
    
    
};