//
//  ContentScene.cpp
//  StereoPlanes
//
//  Created by Johan Bichel Lindegaard on 21/01/14.
//
//

#include "ContentScene.h"
#pragma once

class ContentScene {
    
public:
    
    int index;
    string name;
    string oscAddress = "/default";
    
    // bool syphonDirect;
    bool enabled;

    ContentScene() {
    }
    
    virtual ~ContentScene(){}
    
    virtual void setup(){}
    virtual void update(){}
    virtual void draw(){}
    virtual void exit(){}
    
    virtual void parseOscMessage(ofxOscMessage * m){
        
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
            
        }
        
    }
    
    void setupScene(int width, int height, int i) {
        index = i;
        name = "Scene" + ofToString(i);
        setup();
    }
    
    void updateScene() {
        if(enabled) {
            update();
        }
    }
    
    void drawScene() {
        if(enabled) {
            fbo.begin();
            ofClear(0, 0);
            
            //glEnable(GL_BLEND);
            //glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
            draw();
            
            //glDisable(GL_BLEND);
            
            fbo.end();
        }
        
        if (solo) {
            syphonOut.publishTexture(&fbo.getTextureReference());
        }
    }
    
    
    
};