//
//  StereoPlane.h
//  StereoPlanes
//
//  Created by Johan Bichel Lindegaard on 14/01/14.
//
//

#pragma once
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
    
    int camWidth; // resolution
    int camHeight;
    
    float aspect = 1.0;
    
    ofVec3f camPos;
    
    string name;
    
    ofVec2f pos;
    
    //ofxGLWarper warpLeft;
    //ofxGLWarper warpRight;
    
    StereoPlane(string n) {
        name = n;
    };
    
    ~StereoPlane() {
    };
    
    ofxXmlSettings * settings;
    
    void setup(int w, int h, ofxXmlSettings * s) {
        
        settings = s;
        
        if( !settings->tagExists(name) ) {
            settings->addTag(name);
        }
        
        camHeight = 2.;
        camWidth = height *(w/h);
        
        width = w;
        height = h;
        
        cam.setup(w, h);
        
        //observerPosition.set(0,0,2);
        
        cam.setPhysicalFocusDistance(120);
        cam.setFocusDistance(50);
        

    };
    
    void drawGrid() {
        
        
        ofFill();

        ofDrawGrid(1., 10, true);
        ofDrawGrid(1., 10, true,false,true,false);
        
        ofLight l;
        
        l.setDirectional();
        l.setPosition(ofVec3f(-1,1,-1));
        l.lookAt(ofVec3f(0,0,0));
        
        l.enable();
        
        ofPushMatrix();
        ofTranslate(0,0,1);
        ofEnableLighting();
        ofDrawSphere(0.1);
        ofDisableLighting();
        ofPopMatrix();
        
        ofEnableLighting();
        ofDrawSphere(0.1);
        ofDisableLighting();
        
        ofPushMatrix();
        ofTranslate(0,0,-1);
        ofEnableLighting();
        ofDrawSphere(0.1);
        ofDisableLighting();
        ofPopMatrix();
        
        l.disable();
        
        ofPushMatrix();
        ofTranslate(0,1,0);
        ofDrawGrid(1., 10, true,false,true,false);
        ofPopMatrix();
        
        ofPushMatrix();
        ofTranslate(0,-1,0);
        ofDrawGrid(1., 10, true,false,true,false);
        ofPopMatrix();
        
    }
    
    void drawChessboard() {
        
        ofPushStyle();
        ofFill();
        ofDisableDepthTest();
        float chessSize = 0.05;
        
        ofPushMatrix();
        ofScale(1./(aspect*height*1.0/width),1, 1);
        
        bool white = true;
        
        ofTranslate(-1, -1);
        for(float x = 0; x < 2; x+=chessSize){
            for(float y = 0; y < 2; y+=chessSize){
                if(white){
                    ofSetColor(255);
                } else {
                    ofSetColor(0);
                }
                white = !white;
                ofRect(x, y, chessSize, chessSize);
            }
        }
        ofPopMatrix();
        
        ofPushMatrix();
        
        ofSetColor(255,0,0);
        ofEllipse(0, 0, 1, 1);
        ofDrawAxis(1);
        ofPopMatrix();
        ofPopStyle();
        
    }
    
    void drawChessboards() {
        //warpLeft.begin();
        beginLeft();
        drawChessboard();
        endLeft();
        //warpLeft.end();
        
        //warpRight.begin();
        beginRight();
        drawChessboard();
        endRight();
        //warpRight.end();
    }
    
    void drawGrids() {
        cam.beginLeft();
        drawGrid();
        cam.endLeft();
        
        cam.beginRight();
        drawGrid();
        cam.endRight();
    }
    
    
    int controlSide = 0; // 0: left, 1: right
    
    void beginLeft(){
        ofPushView();
        glPushMatrix();
        cam.beginLeft();
        ofScale(aspect*height*1.0/width, 1.0, 1.0);
    }
    
    void endLeft(){
        cam.endLeft();
        glPopMatrix();
        ofPopView();
    }
    
    void beginRight(){
        ofPushView();
        glPushMatrix();
        cam.beginRight();
        ofScale(aspect*height*1.0/width, 1.0, 1.0);
    }
    
    void endRight(){
        cam.endRight();
        glPopMatrix();
        ofPopView();
    }
    
    void drawLeft() {
        
        ofPushMatrix();
        ofTranslate(pos.x, pos.y);
        cam.leftFbo.draw(0,0,width,height);
        ofPopMatrix();
        
    }
    
    void drawRight() {
        ofPushMatrix();
        ofTranslate(pos.x + width, pos.y);
        cam.rightFbo.draw(0,0,width,height);
        ofPopMatrix();
    }
    
    void draw(){
        ofSetColor(255);
        if(controlSide == 1) {
            drawRight();
            drawLeft();
        } else {
            drawLeft();
            drawRight();
        }
    }
    
    void update() {
        // observerPosition.set(sin(ofGetElapsedTimef()), cos(ofGetElapsedTimef()), -1);
        cam.setPosition(camPos);
        // cam.setFocusDistance( cam.left.getGlobalPosition().length() );
        cam.update(ofRectangle(-1, -1, 2, 2));
    }
    
    void drawInfo() {
        ofDrawBitmapString(name, 10, 10);
    }
    
    /*
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
    */
    
    void exit() {
        
        ofLog(OF_LOG_NOTICE, "Saving data for plane: " + name);
        
        settings->pushTag(name);
        
        /*
        settings->pushTag("warpLeft");
        warpLeft.saveToXml(*settings);
        settings->popTag();
        settings->pushTag("warpRight");
        warpRight.saveToXml(*settings);
        settings->popTag();
        */
        
        settings->popTag();
    }
    
    
private:
    
    
};


