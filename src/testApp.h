#pragma once

#include "ofMain.h"
#include "ofxSyphon.h"
#include "ofxGui.h"
#include "ofxOscParameterSync.h"
#include "ofxXmlSettings.h"
#include "ofxOscReceiver.h"
//#include "ofxStereoCamera.h"
#include "ofxCoolGlasses.h"
#include "ofxGLWarper.h"


class StereoPlane {
    
public:
    // TODO frustrum - dynamicly calculate
    // TODO viewer world location - inherit from world

    //ofxStereoCamera<ofCamera> cam;
    
    ofxCoolGlasses cam;
    
    int width;
    int height;
    string name;
    
    ofxGLWarper warpLeft;
    ofxGLWarper warpRight;
    
    StereoPlane(string n) {
        name = n;
    };
    
    ~StereoPlane() {
    };
    
    ofxXmlSettings * settings;
    
    ofVec3f observerPosition;
    
    // Corners
    ofPoint wlTopLeft;
    ofPoint wlTR;
    ofPoint wlBL;
    ofPoint wlBR;
    
    ofPoint lTL;
    ofPoint lTR;
    ofPoint lBL;
    ofPoint lBR;
    
    ofPoint rTL;
    ofPoint rTR;
    ofPoint rBL;
    ofPoint rBR;
    
    void setup(int w, int h, ofxXmlSettings * s) {
        
        settings = s;
        
        if( !settings->tagExists(name) ) {
            settings->addTag(name);
        }
        
        width = w; height = h;
        
        cam.setup(w, h);
        
        observerPosition.set(0,0,0);
        
        cam.setPhysicalFocusDistance(120);
        cam.setFocusDistance(50);
        
        cam.setPosition(ofVec3f(0, 0, 2));
        //cam.setOf
        
        
        //cam.setupPerspective();
        //cam.lookAt(ofVec3f(0,0,0));
        
        warpLeft.setup(0,0, w, h);
        warpRight.setup(0,0, w, h);
        
        lTL.set(0, 0);
        lTR.set(w, 0);
        lBL.set(0, h);
        lBR.set(w, h);
        
        warpLeft.setCorner(ofxGLWarper::TOP_LEFT,     lTL);
        warpLeft.setCorner(ofxGLWarper::TOP_RIGHT,    lTR);
        warpLeft.setCorner(ofxGLWarper::BOTTOM_LEFT,  lBL);
        warpLeft.setCorner(ofxGLWarper::BOTTOM_RIGHT, lBR);
        
        rTL.set(w,   0);
        rTR.set(w*2, 0);
        rBL.set(w,   h);
        rBR.set(w*2, h);
        
        warpRight.setCorner(ofxGLWarper::TOP_LEFT,     rTL);
        warpRight.setCorner(ofxGLWarper::TOP_RIGHT,    rTR);
        warpRight.setCorner(ofxGLWarper::BOTTOM_LEFT,  rBL);
        warpRight.setCorner(ofxGLWarper::BOTTOM_RIGHT, rBR);
        
        ofAddListener(warpLeft.changeEvent,  this, &StereoPlane::onCornerChangeLW);
        ofAddListener(warpRight.changeEvent, this, &StereoPlane::onCornerChangeRW);
        
        settings->pushTag(name);
        if(!settings->tagExists("warpLeft"))  settings->addTag("warpLeft");
        
        if(!settings->tagExists("warpRight")) settings->addTag("warpRight");
        
        settings->pushTag("warpLeft");
        warpLeft.loadFromXml(*settings);
        settings->popTag();
        
        settings->pushTag("warpRight");
        warpRight.loadFromXml(*settings);
        settings->popTag();
        
        settings->popTag();
    };
    
    void controlDraw() {
        /*if(controlSide == 0) {
            drawLeftControl();
        } else {
            drawRightControl();
        }*/
    };
    
    int controlSide = 0; // 0: left, 1: right
    
    void activateLeftControl() {
        controlSide = 0;
        if (!warpLeft.isActive() )  warpLeft.activate();
        
        if ( warpRight.isActive())  warpRight.deactivate();
    }
    
    void activateRightControl() {
        controlSide = 1;
        if (!warpRight.isActive() ) warpRight.activate();
        
        if ( warpLeft.isActive())   warpLeft.deactivate();
    }
    
    void deactivateControl() {
        if (warpRight.isActive() ) warpRight.deactivate();
        if ( warpLeft.isActive())   warpLeft.deactivate();
        controlSide = 2;
    }
    
    void beginLeft(){
        warpLeft.begin();
        if(warpLeft.isActive()) warpLeft.draw();
        cam.beginLeft();
    }
    
    void endLeft(){
        cam.endLeft();
        warpLeft.end();
    }
    
    void beginRight(){
        warpRight.begin();
        if(warpRight.isActive()) warpRight.draw();
        cam.beginRight();
    }
    
    void endRight(){
        cam.endRight();
        warpRight.end();
    }
    
    void drawLeft() {
        
        warpLeft.begin();
        if(warpLeft.isActive()) warpLeft.draw();
        
        //cam.getLeftFbo()->draw(0,0);
        //cam.left.draw();
        warpLeft.end();
        
        /*if(controlSide < 2) {
            ofNoFill();
            ofCircle(lTL, 6);
            ofCircle(lTR, 6);
            ofCircle(lBL, 6);
            ofCircle(lBR, 6);
        }*/
    }
    
    void drawRight() {
        
        warpRight.begin();
        if(warpRight.isActive()) warpRight.draw();
        //cam.getRightFbo()->draw(0,0);
        cam.right.draw();
        warpRight.end();
        
        /*if(controlSide < 2) {
            ofNoFill();
            ofCircle(rTL, 6);
            ofCircle(rTR, 6);
            ofCircle(rBL, 6);
            ofCircle(rBR, 6);
        }*/
        
    }
    
    void draw(){
        if(controlSide == 1) {
            drawRight();
            drawLeft();
        } else {
            drawLeft();
            drawRight();
        }
    }
    
    void update() {
        
        
        
        observerPosition.set(ofMap(fmodf(ofGetElapsedTimef(),1), 0, 1, -1, 1), 0, 2);
        cam.setPosition(observerPosition);
        cam.setFocusDistance( cam.left.getGlobalPosition().length() );
        
        //cam.setPosition(headPosition);
        ///cam.setupOffAxisViewPortal(ofVec3f(0,0,0), ofVec3f(0, height, 0), ofVec3f(width,height,0));
        
        cam.update(ofRectangle(0, 0, 1, 1));
    }
    
    
    void drawInfo() {
        ofDrawBitmapString(name, 10, 10);
        
    }
    
    void onCornerChangeLW(ofxGLWarper::CornerLocation & cornerLocation){
        switch (cornerLocation) {
            case ofxGLWarper::TOP_LEFT:
                lTL = warpLeft.getCorner(cornerLocation);
                break;
            case ofxGLWarper::TOP_RIGHT:
                lTR = warpLeft.getCorner(cornerLocation);
                break;
            case ofxGLWarper::BOTTOM_LEFT:
                lBL = warpLeft.getCorner(cornerLocation);
                break;
            case ofxGLWarper::BOTTOM_RIGHT:
                lBR = warpLeft.getCorner(cornerLocation);
                break;
                
        }
    }
    
    void onCornerChangeRW(ofxGLWarper::CornerLocation & cornerLocation){
        switch (cornerLocation) {
            case ofxGLWarper::TOP_LEFT:
                rTL = warpRight.getCorner(cornerLocation);
                break;
            case ofxGLWarper::TOP_RIGHT:
                rTR = warpRight.getCorner(cornerLocation);
                break;
            case ofxGLWarper::BOTTOM_LEFT:
                rBL = warpRight.getCorner(cornerLocation);
                break;
            case ofxGLWarper::BOTTOM_RIGHT:
                rBR = warpRight.getCorner(cornerLocation);
                break;
        }
    }
    
    
    void exit() {
        
        ofLog(OF_LOG_NOTICE, "Saving data for plane: " + name);
        
        settings->pushTag(name);
        
        settings->pushTag("warpLeft");
        warpLeft.saveToXml(*settings);
        settings->popTag();
        settings->pushTag("warpRight");
        warpRight.saveToXml(*settings);
        settings->popTag();
        settings->popTag();
    }
    
    
private:
    
    
};


class testApp : public ofBaseApp
{
public:
	
	void setup();
	void update();
	void draw();
	
	void drawScene1();
    void drawScene2();
    
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
    void exit();
        
	ofxSyphonServer leftOutputServer;
	ofxSyphonServer rightOutputServer;
    
    ofxSyphonServer sbsOutputServer;
    
    ofLight directionalLight;
    
    ofMaterial material;
    
    bool showGrid;
    bool showDots;
    bool setBulge;
    
    float centerBulge;
    
    
    ofxOscReceiver oscReceiver;
    ofxOscSender oscSender;
    
    ofParameter<bool>  strobeOn;
    ofParameter<float> boxOneZ; // a fraction of clock
    ofParameter<float> boxTwoZ;
    
    ofxOscParameterSync sync;
    
    ofParameterGroup parameters;
    
    ofxPanel gui;
    
    void drawRibbon();
    
    //this holds all of our points
    vector<ofVec3f> points;
    //this keeps track of the center of all the points
    ofVec3f center;
    
    ofFbo fbo;
    
    StereoPlane * floor;
    StereoPlane * wall;
    
    StereoPlane * activePlane;
    int activePlaneIndex;
    
    //StereoPlane thing;
    
    void drawFloor();
    
    vector<StereoPlane *> planes;
    
    ofxXmlSettings settings;
    
    
};