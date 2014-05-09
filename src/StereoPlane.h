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
    
    ofRectangle viewport;
    
    int width;
    int height;
    
    int camWidth; // resolution
    int camHeight;
    
    float aspect = 1.0;
    
    ofVec3f camPos;
    
    string name;
    
    ofVec2f pos;
    
    StereoPlane(string n) {
        name = n;
    };
    
    ~StereoPlane() {
    };
    
    ofxXmlSettings * settings;
    
    void setup(int w, int h, ofxXmlSettings * s) {
        
        viewport = ofRectangle(-1, -1, 2, 2);
        
        settings = s;
        
        if( !settings->tagExists(name) ) {
            settings->addTag(name);
        }
        
        camHeight = 2.;
        camWidth = height *(w/h);
        
        width = w;
        height = h;
        
        cam.setup(w, h);
        
        cam.setPhysicalFocusDistance(120);
        cam.setFocusDistance(50);
        

    };
    
    void setViewPort(ofRectangle _viewport) {
        viewport = _viewport;
    };
    
    void drawGrid() {
        
        
        ofFill();

        ofDrawGrid(1., 4, true);
        ofDrawGrid(1., 4, true,false,true,false);
        
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
        ofDrawGrid(1., 4, true,false,true,false);
        ofPopMatrix();
        
        ofPushMatrix();
        ofTranslate(0,-1,0);
        ofDrawGrid(1., 4, true,false,true,false);
        ofPopMatrix();
        
    }
    
    void drawChessboard() {
        
       
        
        ofPushStyle();
        ofFill();
        ofDisableDepthTest();
        
            float chessSize = 0.05;
            
            ofPushMatrix(); {
                ofScale(1./(aspect*height*1.0/width),1, 1);
        
                bool white = true;
                bool red = true;
                int row = 0;
                
                ofColor col;
                
                ofTranslate(-1, -1);
                for(float x = 0; x < 2; x+=chessSize){

                    row += 1;
                    
                    for(float y = 0; y < 2; y+=chessSize){
                        
                        col.set(200,200,200);
                        
                        if(!white) col.a = 0;
                        
                        ofSetColor(col.r, col.g, col.b, col.a);
                        white = !white;
                        
                        ofRect(x, y, chessSize, chessSize);
                    }
                }
            } ofPopMatrix();
        
            
        ofPushMatrix();

        ofSetColor(255,255,255);
        ofEllipse(0, 0, 0.5, 0.5);
        
        
        ofSetColor(255,255,0);
        
        ofCircle(-0.9, -0.9, 0.04);
        ofCircle(0.9, 0.9, 0.04);
        ofCircle(-0.9, 0.9, 0.04);
        ofCircle(0.9, -0.9, 0.04);
        
        ofCircle(0, 0.9, 0.04);
        ofCircle(0, -0.9, 0.04);
        
        
        /*ofRect(-0.95, -0.95, 1.85, 0.1);
        ofSetColor(0,155,255);
        ofRect(-0.95, -0.1, 1.85, 0.1);
        ofSetColor(255,155,0);
        ofRect(0, -0.95, 0.1, 1.95);*/
         
        //ofDrawAxis(1);
        ofPopMatrix();
        ofPopStyle();
        
    }
    
    
    /*void drawMappingGrid() {
        
        
        ofPushStyle();
        ofFill();
        ofDisableDepthTest();
        
        float chessSize = 0.05;
        
        ofPushMatrix(); {
            ofScale(1./(aspect*height*1.0/width),1, 1);
            
            
            ofRect(-1,-1, 0.2, 2);
            
            
        
        
        } ofPopMatrix();
        
        
        ofPushMatrix();
        
        ofSetColor(255,0,0);
        ofEllipse(0, 0, 1, 1);
        //ofDrawAxis(1);
        ofPopMatrix();
        ofPopStyle();
        
    }

    void drawMappingGrids() {
        
        beginLeft();
        drawChessboard();
        endLeft();
        
        beginRight();
        drawChessboard();
        endRight();
    }*/
    
    
    void drawChessboards() {
        
        beginLeft();
        drawChessboard();
        endLeft();

        beginRight();
        drawChessboard();
        endRight();
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
        cam.update(viewport);
    }
    
    void drawInfo() {
        ofDrawBitmapString(name, 10, 10);
    }
    
    void exit() {
        
        ofLog(OF_LOG_NOTICE, "Saving data for plane: " + name);
        
        settings->pushTag(name);

        settings->popTag();
    }
    
    
private:
    
    
};


