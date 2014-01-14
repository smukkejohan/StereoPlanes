#include "testApp.h"
#include <OpenGL/OpenGL.h>
#include <ofGLUtils.h>

// Syphon together with 3D primitive and NoFill does not run

const int width = 1024*2;
const int height = 768;

vector<ofVec3f> points;

//--------------------------------------------------------------
void testApp::setup()
{
    ofSetWindowShape(width, height);
    
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
    
    //light.setPointLight();
    light.setPosition(0, 0, 10);
    light.setSpotlight();
    
}

//--------------------------------------------------------------
void testApp::update()
{
    for(int i=0; i<planes.size(); i++) {
        planes[i]->update();
    }
}


void testApp::drawFloor() {
    
    glPushMatrix();
    
//    ofBackground(0,0,0,255);
    
    ofSetColor(255,255,255,100);
    //ofDrawGrid(1);
    
    ofPushMatrix();
    //ofRotateX(ofGetElapsedTimef()*100);
    
    ofFill();
    ofSetColor(255,255,255,255);
    //ofDrawBox(0.1);
    //ofDrawBox(0.2);
    ofSetLineWidth(6);
    ofDrawBox(0.5);
    //ofDrawBox(1);
    //ofDrawBox(1.5);
    ofPopMatrix();
   
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
    
    //fbo.begin();
    ofSetColor(255);
    ofSetLineWidth(2);
    ofFill();
    
    //leftOutputServer.publishTexture(&cam.getLeftTexture());
    //rightOutputServer.publishTexture(&cam.getRightTexture());
    //cam.getLeftFbo()->draw(0, 0, ofGetWidth()/2, ofGetHeight()/2);
    //gui.draw();
    
    for(int i=0; i<planes.size(); i++) {
        planes[i]->draw();
    }
    
    if(showGrid) {
        for(int i=0; i<planes.size(); i++) {
            planes[i]->drawGrids();
        }
    }
    
    fbo.end();
    
    sbsOutputServer.publishTexture(&fbo.getTextureReference());
    //sbsOutputServer.publishScreen();
    //fbo.draw(0,0);
    
    ofPushMatrix();
    ofScale(0.8,0.8,0.8);
    fbo.draw(0, 0);
    ofPopMatrix();
    
    
    ofSetColor(255);
    ofDrawBitmapString(ofToString(ofGetFrameRate()), 40, 40);
    activePlane->drawInfo();

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
    ofVec3f mousePoint(x - ofGetWidth()/2,y-ofGetHeight()/2, 400);
    points.push_back(mousePoint);
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