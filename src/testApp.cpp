#include "testApp.h"
#include <OpenGL/OpenGL.h>
#include <ofGLUtils.h>

// Syphon together with 3D primitive and NoFill does not run

vector<ofVec3f> points;

//--------------------------------------------------------------
void testApp::setup()
{
    
    
    ofSetFrameRate(30);
    ofSetVerticalSync(true);
    ofSetBackgroundAuto(true);
    ofBackground(0);
    
    //leftOutputServer.setName("Left");
    //rightOutputServer.setName("Right");
    sbsOutputServer.setName("Side By Side");
    
    fbo.allocate(2048, 768);
    
    settings.load("stereoplanes.xml");
    
    floor = new StereoPlane("floor");
    floor->setup(512, 768, &settings);
    floor->pos = ofVec2f(0,0);
    planes.push_back(floor);
        
    wall = new StereoPlane("wall");
    wall->setup(512, 768, &settings);
    wall->pos = ofVec2f(1024,0);
    planes.push_back(wall);
    
    activePlaneIndex = 0;
    activePlane = planes[activePlaneIndex];
    
    oscReceiver.setup(9001);
    
    camPosFloor = ofVec3f(0, 0, -1);
    camPosWall = ofVec3f(0, 0, -1);
    
    testScene = new TestScene();
    contentScenes.push_back(testScene);
    
    voronoiWall = new VoronoiWall();
    contentScenes.push_back(voronoiWall);
    
    boxFloor = new BoxFloor();
    contentScenes.push_back(boxFloor);

    attractorControl = new AttractorControl();
    contentScenes.push_back(attractorControl);
    
    wireMesh = new WireMesh();
    contentScenes.push_back(wireMesh);
    
    voro3d = new Voro3D();
    contentScenes.push_back(voro3d);
    
    for(int i=0; i<contentScenes.size(); i++) {
        contentScenes[i]->setupScene(i);
    }
    
    // add all parameterGroups from scenes to parameters
    // gui.setup(parameters);
    
    float xInit = OFX_UI_GLOBAL_WIDGET_SPACING;
    float width = 255-xInit;
    hideGUI = false;
    
    gui = new ofxUIScrollableCanvas(20, 0, width+xInit, ofGetHeight());
    
    gui->setScrollAreaToScreenHeight();
    gui->setScrollableDirections(false, true);
    
    gui->setFont("GUI/Arial.ttf");
    gui->setWidgetFontSize(OFX_UI_FONT_SMALL);
    gui->setColorBack(ofColor(10, 10, 10,220));
    
    gui->addLabel("Stereo", OFX_UI_FONT_LARGE);
    
    gui->addSlider("Eye seperation", 0, 7, &eyeSeperation);
    
    gui->addSlider("Floor X",  -2, 2, &camPosFloor.x);
    gui->addSlider("Floor Y",  -2, 2, &camPosFloor.y);
    gui->addSlider("Floor Z",  -2, -0.25, &camPosFloor.z);
    
    gui->addSlider("Wall X",  -2, 2, &camPosWall.x);
    gui->addSlider("Wall Y",  -2, 2, &camPosWall.y);
    gui->addSlider("Wall Z",  -2, -0.25, &camPosWall.z);
    
    gui->addSlider("Dancer X",  -1, 1, &dancerPos.x);
    gui->addSlider("Dancer Y",  -1, 1, &dancerPos.y);
    
    for(int i=0; i<contentScenes.size(); i++) {
        gui->addSpacer(width, 3)->setDrawOutline(true);
        contentScenes[i]->setGui(gui, width);
    }
    
    gui->autoSizeToFitWidgets();
    ofAddListener(gui->newGUIEvent,this,&testApp::guiEvent);
    
    gui->setDrawBack(true);
    gui->setScrollAreaToScreenHeight();
    gui->loadSettings("GUI/guiSettings.xml");
    
}


//--------------------------------------------------------------
void testApp::update()
{
    
    
    while(oscReceiver.hasWaitingMessages()){
		// get the next message
		ofxOscMessage m;
		oscReceiver.getNextMessage(&m);
        
        for(int i=0; i<contentScenes.size(); i++) {
            contentScenes[i]->parseSceneOsc(&m);
        }
        
        //cout<<m.getAddress()<<endl;
        
		if(m.getAddress() == "/Floor/Camera/x"){
            camPosFloor.x = m.getArgAsFloat(0);
            
		} else if(m.getAddress() == "/Floor/Camera/y"){
            camPosFloor.y = m.getArgAsFloat(0);
            
		} else if(m.getAddress() == "/Floor/Cameraz/x"){
                camPosFloor.z = m.getArgAsFloat(0);
            
        } else if(m.getAddress() == "/Wall/Camera/x"){
                camPosWall.x = m.getArgAsFloat(0);
            
        } else if(m.getAddress() == "/Wall/Camera/y"){
                camPosWall.y = m.getArgAsFloat(0);
            
        } else if(m.getAddress() == "/Wall/Cameraz/x"){
                camPosWall.z = m.getArgAsFloat(0);
            
		} else if(m.getAddress() == "/eyeSeperation/x"){
			eyeSeperation = m.getArgAsFloat(0);
            
		} else if(m.getAddress() == "/Dancer/x"){
            dancerPos.x = m.getArgAsFloat(0);
            
		} else if(m.getAddress() == "/Dancer/y"){
            dancerPos.y = m.getArgAsFloat(0);
            
		} else if(m.getAddress() == "/activescene/x"){
            for(int i=0; i<contentScenes.size(); i++) {
                contentScenes[i]->enabled = false;
            }
            contentScenes[m.getArgAsInt32(0)]->enabled = true;
        }
    }
    
    planes[0]->cam.setPosition(camPosFloor);
    planes[1]->cam.setPosition(camPosWall);
    
    for(int i=0; i<planes.size(); i++) {
        planes[i]->cam.setPhysicalEyeSeparation(eyeSeperation);
        planes[i]->update();
    }

    for(int s=0; s<contentScenes.size();s++) {
        contentScenes[s]->update();
    }
    
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
}


void testApp::drawScenes(int _surfaceId) {
    for(int s=0; s<contentScenes.size();s++) {
        contentScenes[s]->drawScene(_surfaceId);
    }
}

//--------------------------------------------------------------
void testApp::draw()
{
    
    ofSetColor(0);
    
    ofEnableLighting();
    ofEnableDepthTest();
    ofSetColor(255);
    
    // draw scenes to surfaces, they are kept in the cameras fbo
    for(int i=0;i < planes.size(); i++) {
        planes[i]->beginLeft();
        drawScenes(i);
        planes[i]->endLeft();
        
        planes[i]->beginRight();
        drawScenes(i);
        planes[i]->endRight();
    }
    
    ofDisableDepthTest();
    ofDisableLighting();
    
    if(showGrid) {
        for(int i=0; i<planes.size(); i++) {
            planes[i]->drawGrids();
        }
    }
    
    // Draw the scenes to the output fbo
    fbo.begin(); {
        //ofClear(0, 0, 0);
        ofSetColor(255);
        ofFill();
    
        for(int i=0; i<planes.size(); i++) {
            planes[i]->draw();
        }
        
    }fbo.end();
    sbsOutputServer.publishTexture(&fbo.getTextureReference());
    
    // Draw interface and monitor view
    //ofBackground(60,60,60);
    
    ofPushMatrix();
    ofTranslate(300, 20);
    ofPushMatrix();
    ofScale(0.5,0.5);
    ofSetColor(255);
    fbo.draw(0, 0);
    ofPopMatrix();
    ofPopMatrix();
    

}

//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
    
	if (key == 'f'){
		ofToggleFullscreen();
	} else if (key == 'g') {
		showGrid = !showGrid;
	} else if (key == 'd') {
        hideGUI = !hideGUI;
	}
    
    
    else if (key == 'w'){
        gui->loadSettings("GUI/wallsetting.xml");
        
    }
    else if(key == 'q'){
        gui->loadSettings("GUI/floorsetting.xml");
    }
    
    else if (key == 'W'){
        gui->saveSettings("GUI/wallsetting.xml");
        
    }
    else if(key == 'Q'){
        gui->saveSettings("GUI/floorsetting.xml");
    }
    
}


//--------------------------------------------------------------
void testApp::keyReleased(int key)
{
    
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y)
{
    //ofVec3f mousePoint(x - ofGetWidth()/2,y-ofGetHeight()/2, 400);
    //points.push_back(mousePoint);
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button)
{
    
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button)
{
    
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button)
{
    
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h)
{
    
    gui->setScrollAreaToScreenHeight();
    
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg)
{
    
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo)
{
    
}

void testApp::exit() {
    
    gui->saveSettings("GUI/guiSettings.xml");
    delete gui;
    
    for(int i=0; i < planes.size(); i++) {
        planes[i]->exit();
    }
    
    for(int s=0; s<contentScenes.size();s++) {
        contentScenes[s]->exit();
    }
    
    settings.save("stereoplanes.xml");
    cout<<"exit"<<endl;
}


void testApp::guiEvent(ofxUIEventArgs &e)
{
    
    string name = e.getName();
	int kind = e.getKind();
	//cout << "got event from: " << name << endl;
    
    /*if(name=="Wall cam") {
        ofxUI2DPad *pad = (ofxUI2DPad *) e.widget;
        camPosWall.x = pad->getScaledValue().x;
        camPosWall.y = pad->getScaledValue().y;
    }*/
    
    for(int i=0; i<contentScenes.size(); i++) {
        contentScenes[i]->guiEvent(e);
    }
    
    
}