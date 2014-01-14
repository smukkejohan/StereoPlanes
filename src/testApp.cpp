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
    
    leftOutputServer.setName("Left");
    rightOutputServer.setName("Right");
    sbsOutputServer.setName("Side By Side");
    
    fbo.allocate(ofGetWidth(), ofGetHeight());
    
    settings.load("stereoplanes.xml");
    
    floor = new StereoPlane("floor");
    floor->setup(1024, 1024, &settings);
    planes.push_back(floor);
    
    wall = new StereoPlane("wall");
    wall->setup(1024, 1024, &settings);
    planes.push_back(wall);
    
    activePlaneIndex = 0;
    activePlane = planes[activePlaneIndex];
    
    light.setPosition(2, 2, -2);
    light.setSpotlight();
    light.lookAt(ofVec3f(0,0,0));
    light.setDiffuseColor(ofColor(245,245,170));
    light.setAmbientColor(ofColor(64,84,89));
    light.setSpecularColor(ofColor::white);
    
    dirLight.setSpotlight();
    dirLight.setPosition(-1, -1, 1);
    dirLight.lookAt(ofVec3f(0,0,0));
    dirLight.setDiffuseColor(ofColor(191,191,170));
    
    parameters.setName("Stereo");
    parameters.add(camPos.set("Cam position", ofVec3f(0.,0.,-1), ofVec3f(-2,-2,-8.), ofVec3f(2,2,-0.5)));
    parameters.add(eyeSeperation.set("Eye Seperation", 6.5, 0., 7.));
    
    sync.setup(parameters, 9002, "localhost", 8000);
    
    gui.setup(parameters);
    
    oscReceiver.setup(9001);
    
}

//--------------------------------------------------------------
void testApp::update()
{
    
    
    while(oscReceiver.hasWaitingMessages()){
		// get the next message
		ofxOscMessage m;
		oscReceiver.getNextMessage(&m);
        
		if(m.getAddress() == "/Camera/x"){
            ofVec3f pos = camPos.get();
            pos.x = m.getArgAsFloat(0);
			camPos.set(pos);
		
		} else if(m.getAddress() == "/Camera/y"){
            ofVec3f pos = camPos.get();
            pos.y = m.getArgAsFloat(0);
			camPos.set(pos);
            
		} else if(m.getAddress() == "/Camera/z"){
            
            ofVec3f pos = camPos.get();
            pos.z = m.getArgAsFloat(0);
			camPos.set(pos);
            
		} else if(m.getAddress() == "/eyeSeperation/x"){
			eyeSeperation.set(m.getArgAsFloat(0));
            
		}
        
        
        
        
    }
    
    
    for(int i=0; i<planes.size(); i++) {
        planes[i]->cam.setPosition(camPos.get());
        planes[i]->cam.setPhysicalEyeSeparation(eyeSeperation.get());
        planes[i]->update();
        //cout<<camPos.get()<<endl;
    }
    
}


void testApp::drawFloor() {
    
    glPushMatrix();
    glEnable (GL_FOG);
    glFogi (GL_FOG_MODE, GL_EXP2);
    glHint (GL_FOG_HINT, GL_NICEST);
    
    glFogi(GL_FOG_DENSITY, 1);
    
    light.enable();
    dirLight.enable();
    
//  ofBackground(0,0,0,255);
    
    ofSetColor(255,255,255,100);
    //ofSetLineWidth(3);
    //ofDrawGrid(1);
    
    ofPushMatrix();
    //ofRotateX(ofGetElapsedTimef()*100);
    ofFill();
    ofSetColor(255,255,255,255);
    //ofDrawBox(0.1);
    //ofDrawBox(0.2);
    //ofSetLineWidth(6);
    ofSetBoxResolution(10);
    ofRotateX(ofGetElapsedTimef()*10);
    ofDrawBox(0.5);
    
    ofPopMatrix();
    ofRect(-1,-1, 2, 2);
    
    
    for (int i = 0; i< 200; i++) {
        ofDrawSphere(ofSignedNoise((ofGetElapsedTimef()*0.01)+i, 0, 0), ofSignedNoise((ofGetElapsedTimef()*0.01)+i, (ofGetElapsedTimef()*0.01)+i, 0), ofSignedNoise(0,0,(ofGetElapsedTimef()*0.01)+i)*0.5,  0.05);
    }
    
    //ofDrawBox(1);
    //ofDrawBox(1.5);
    
    light.disable();
    dirLight.disable();
    
    
    glDisable(GL_FOG);
    ofDisableLighting();
    glPopMatrix();
    
}

//--------------------------------------------------------------
void testApp::draw()
{
    ofEnableDepthTest();
    ofBackground(ofColor(0,0,0));
    
    fbo.begin();
    
    ofClear(0, 0, 0);
    
    floor->beginLeft();
        drawFloor();
    floor->endLeft();
    
    floor->beginRight();
        drawFloor();
    floor->endRight();
    
    wall->beginLeft();
        drawFloor();
    wall->endLeft();
    
    wall->beginRight();
        drawFloor();
    wall->endRight();
    
    ofDisableDepthTest();
    
    ofSetColor(255);
    ofSetLineWidth(2);
    ofFill();
    
    if(showGrid) {
        for(int i=0; i<planes.size(); i++) {
            planes[i]->drawGrids();
        }
    }
    
    for(int i=0; i<planes.size(); i++) {
        planes[i]->draw();
    }
    
    fbo.end();
    
    sbsOutputServer.publishTexture(&fbo.getTextureReference());
    
    ofPushMatrix();
    fbo.draw(0, 0);
    ofPopMatrix();
    
    ofSetColor(255);
    ofDrawBitmapString(ofToString(ofGetFrameRate()), 40, 40);
    activePlane->drawInfo();
    
    gui.draw();

}

//--------------------------------------------------------------
void testApp::keyPressed(int key)
{

	if (key == 'f')
	{
		ofToggleFullscreen();
	} else if (key == 'g')
	{
		showGrid = !showGrid;
	} else if (key == 'd')
	{
		showDots =! showDots;
	} else if(key == 'r') {
        activePlane->activateRightControl();
    } else if(key == 'l') {
        activePlane->activateLeftControl();
    } else if(key == 'p') {
        
        if(activePlaneIndex == planes.size()-1) {
            activePlaneIndex = 0;
        } else {
            activePlaneIndex++;
        }
        
        activePlane->deactivateControl();
        activePlane = planes[activePlaneIndex];
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
    
    for(int i=0; i < planes.size(); i++) {
        planes[i]->exit();
    }
    
    settings.save("stereoplanes.xml");
    cout<<"exit"<<endl;
}