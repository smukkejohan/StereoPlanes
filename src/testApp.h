#pragma once

#include "ofMain.h"
#include "ofxSyphon.h"
#include "ofxGui.h"
#include "ofxXmlSettings.h"
#include "ofxOscReceiver.h"
#include "StereoPlane.h"
#include "VoronoiWall.h"
#include "BoxFloor.h"
#include "Lines.h"
#include "Voro3D.h"
#include "GhostLights.h"
#include "ContentScene.h"
#include "TestScene.h"
#include "ofxUI.h"
#include "ofxTimeline.h"
#include "Lights.h"
#include "FlyLight.h"

class testApp : public ofBaseApp
{
public:
	
	void setup();
	void update();
	void draw();
    
    
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
    ofxSyphonServer sbsOutputServer;
    
    bool showGrid;

    ofxOscReceiver oscReceiver;
//  ofxOscSender oscSender;
    
    float eyeSeperation;
    
    ofxTimeline timeline;
    
    bool drawFBOs = false;
    bool drawChessboards = false;
    bool drawGrids = false;
    float aspect = 1.0;
    
    float guiWidth;
    
    bool hideGUI = false;
    bool hideMonitor = false;
    
    vector<ofxUIScrollableCanvas *> guis;
    
    void guiEvent(ofxUIEventArgs &e);
    
    ofFbo fbo;
    StereoPlane * floor;
    StereoPlane * wall;
    StereoPlane * wallRight;
    
    // --
    StereoPlane * activePlane;
    int activePlaneIndex;
    
    void drawFloor();
    vector<StereoPlane *> planes;
    ofxXmlSettings settings;
    
    void drawScenes(int _surfaceId=0);
    
    // Scenes
    
    Lights * lights;
    
    FlyLight * flyLightLeft;
    FlyLight * flyLightRight;
    FlyLight * flyLightFloor;
    
    TestScene * testScene;
    VoronoiWall * voroWallLeft;
    VoronoiWall * voroWallRight;
    VoronoiWall * voroFloor;
    
    GhostLights * ghostLights;
    
    vector<ofxOlaShaderLight *> shaderLights;
    
    vector<ContentScene*> contentScenes;
        
};