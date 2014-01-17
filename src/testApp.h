#pragma once

#include "ofMain.h"
#include "ofxSyphon.h"
#include "ofxGui.h"
//#include "ofxOscParameterSync.h"
#include "ofxXmlSettings.h"
#include "ofxOscReceiver.h"
//#include "ofxStereoCamera.h"
#include "StereoPlane.h"
#include "ofxBullet.h"
#include "VoronoiWall.h"
#include "CeilingPlane.h"
#include "Ribbon.h"
#include "BoxFloor.h"

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
//    ofxOscSender oscSender;
    
    //ofParameter<float> boxOneZ; // a fraction of clock
    //ofParameter<float> boxTwoZ;
    
    ofParameter<ofVec3f> camPos;
    ofParameter<float> eyeSeperation;
    ofParameter<float> dancerEllipseSize;
    ofParameter<float> dancerEllipseBrightness;
    ofParameter<ofVec2f> dancerPos;
    
    //    ofxOscParameterSync sync;
    
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
    
    void drawFloor();
    vector<StereoPlane *> planes;
    ofxXmlSettings settings;
    
    // Lights
    ofLight light;
    ofLight dirLight;
    
    // Bullet
    ofxBulletWorldRigid			world;
	ofxBulletBox				ground;
    ofxBulletBox                wallBack;
    ofxBulletBox                wallLeft;
    ofxBulletBox                wallRight;
    ofxBulletBox                wallFront;
    ofxBulletBox                dancerCylinder;
    btGeneric6DofConstraint *   dancerConstraint;
	vector<ofxBulletSphere*>    spheres;
    
    float dancerHeight;
    
    bool addSphere = false;

    void drawBulletFloor();
    
    
    VoronoiWall * voronoiWall;
    CeilingPlane * ceilingPlane;
    Ribbon * ribbon;
    BoxFloor * boxFloor;
    
    ofFbo shareTexture;
    

};