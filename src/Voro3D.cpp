//
//  Voro3D.cpp
//  StereoPlanes
//
//  Created by Johan Bichel Lindegaard on 21/01/14.
//
//

#include "Voro3D.h"



void Voro3D::setup() {
    
    light.setPosition(2, 2, -2);
    light.setSpotlight();
    light.lookAt(ofVec3f(0,0,0));
    light.setDiffuseColor(ofColor(255,55,255));
    light.setAmbientColor(ofColor(85,235,199));
    light.setSpecularColor(ofColor::red);
    
    dirLight.setSpotlight();
    dirLight.setPosition(-1, -1, 1);
    dirLight.lookAt(ofVec3f(0,0,0));
    dirLight.setDiffuseColor(ofColor(180,100,100));
    dirLight.setSpecularColor(ofColor::white);
        
    wallCube = new VoroCube();
    wallCube->setup(0.2,0.2,0.2,40);

    floorCube = new VoroCube();
    floorCube->setup(0.2,0.2,0.2,10);
    
    /*
    extraCube = new VoroCube();
    extraCube->setup(0.4,0.01,0.2,60);
    

    */
    
    logo.allocate(602, 242, OF_IMAGE_COLOR_ALPHA);
    logo.loadImage("recoil_logo.png");
    logo.mirror(true, false);
    
    font.loadFont("andale mono", 24);
    font.setLetterSpacing(0.8);
    
}

void Voro3D::draw(int _surfaceId) {
    
    
    
    ofBackground(0);
    
    light.enable();
    dirLight.enable();
    
    if(_surfaceId == 1) {
    
        /*for (int i = 0; i < cellCentroids.size(); i++){
            ofSetColor(255,0,0);
            //ofDrawSphere(cellCentroids[i], cellRadius[i]*0.15);
         }*/
        
        
        ofPushMatrix(); {
        
        ofTranslate(0.15, 0);
        
        ofRotateX(ofGetElapsedTimef()*6);
        ofRotateY(ofGetElapsedTimef()*7);
        ofRotateZ(ofGetElapsedTimef()*8);
        
        for(int i = 0; i < wallCube->cellMeshes.size(); i++){
            ofPushMatrix(); {
            
            ofScale(1.65,1.65,1.65);
            ofSetColor(255,255,255,255);
            
            float explode = ofMap(ofSignedNoise(ofGetElapsedTimef()), 0, 1, 0.8, 1);
            
            ofTranslate(wallCube->cellMeshes[i].getCentroid().x, wallCube->cellMeshes[i].getCentroid().y, wallCube->cellMeshes[i].getCentroid().z);
            ofScale(explode,explode,explode);
            ofTranslate(-wallCube->cellMeshes[i].getCentroid().x, -wallCube->cellMeshes[i].getCentroid().y, -wallCube->cellMeshes[i].getCentroid().z);
            
            wallCube->cellMeshes[i].drawFaces();
            } ofPopMatrix();

        }
        } ofPopMatrix();
        
        
        ofPushMatrix(); {
        ofTranslate(-0.5,-0.1,0.08);
        
        ofTranslate(0,1,0);
        ofRotateX(-90);
        
        for(int i=0; i<12; i++) {
            ofPushMatrix(); {
            
            ofTranslate(0, ofMap(i,0,12,0,-3),0);
                ofSetColor(ofMap(i,0,12,180,10));
            ofDrawBox(0.6,0.02,0.02);
            
            } ofPopMatrix();
        }
        
        } ofPopMatrix();
        
        ofPushMatrix(); {
            
            ofTranslate(0.1, 0.5, -0.2);
            
            ofRotateY(ofMap(ofSignedNoise(ofGetElapsedTimef()/10), 0, 1, 4, -4));
            ofSetColor(255);
            logo.draw(0,0, 0.43, 0.2);
            
        } ofPopMatrix();
        
    }
    
    if(_surfaceId == 0) {
    
        
        ofSetColor(255);
        // Draw Logo

        
        // Draw "In Stereo"
        ofSetColor(255,255,255, 230);
        ofPushMatrix(); {
            ofTranslate(-0.9, -0.9, 0.65);
            ofRotateZ(180);
            ofRotateY(180);
            ofPushMatrix();{
                
                ofScale(0.005,0.005,0.005);
                
                ofPushMatrix(); {
                    
                    ofRotateX(70);
                    ofRotateY(33);
                    ofPushMatrix();
                    ofTranslate(0, 0, -0.1);
                    ofSetColor(255, 255, 255,200);
                    //ofRect(-10,-80,140,125);
                    ofPopMatrix();
                    ofSetColor(255);
                    //font.drawString(" Cool", 0,0);
                    //font.drawString(" Glasses! ", 0,-24);
                    font.drawString("\\o-o/", 0,-28);
                    //font.drawString(" \\o-o/ ", 0,-60);
                }ofPopMatrix();
                
                /*ofPushMatrix();{
                 //ofRotateY(8);
                 ofScale(3, 3);
                 font.drawString("\\o-o/", 12,0);
                 }ofPopMatrix();*/
                
            }ofPopMatrix();
            
        } ofPopMatrix();
        
        
        ofPushMatrix(); {
            
            ofTranslate(0.1, 0.2, -0.38);
            
            ofRotateX(ofGetElapsedTimef()*6);
            ofRotateY(ofGetElapsedTimef()*7);
            ofRotateZ(ofGetElapsedTimef()*8);
            
            for(int c=0;c<3;c++) {
                
                ofPushMatrix(); {
                if(c==1) {
                    
                    ofTranslate(0.2, 0.2, 0.2);
                    ofRotateX(ofGetElapsedTimef()*8);
                    ofRotateY(ofGetElapsedTimef()*9);
                    ofRotateZ(ofGetElapsedTimef()*7);
                }
                if(c==2) {
                    
                    ofTranslate(-0.2, -0.2, 0.2);
                    ofRotateX(ofGetElapsedTimef()*6);
                    ofRotateY(ofGetElapsedTimef()*5);
                    ofRotateZ(ofGetElapsedTimef()*3);
                }
                
            ofScale(0.6,0.6,0.6);
            ofSetColor(255,255,255,200);
            for(int i = 0; i < floorCube->cellMeshes.size(); i++){
                ofPushMatrix(); {
                    
                    
                    
                    floorCube->cellMeshes[i].drawFaces();
                } ofPopMatrix();
            }
            }ofPopMatrix();
            }
        } ofPopMatrix();
        
        ofPushMatrix(); {
            ofTranslate(0.8, 0.8, 0.08);
            
            //ofRotateX(-90);
            
            for(int i=0; i<12; i++) {
                ofPushMatrix(); {
                    
                    ofTranslate(0, 0, ofMap(i,0,12,0,3));
                    ofSetColor(ofMap(i,0,12,180,10));
                    ofDrawBox(0.02,0.4,0.02);
                    
                } ofPopMatrix();
            }
            
        } ofPopMatrix();
        

    
    }
    
    dirLight.disable();
    light.disable();
        
        
    
    
}

void Voro3D::update() {
    wallCube->update();
    
}

void Voro3D::setGui(ofxUICanvas * gui, float width){
    ContentScene::setGui(gui, width);

    
}

void Voro3D::receiveOsc(ofxOscMessage * m, string rest) {
}

void Voro3D::guiEvent(ofxUIEventArgs &e)
{
    
    string name = e.getName();
	int kind = e.getKind();
	//cout << "got event from: " << name << endl;
    
}



