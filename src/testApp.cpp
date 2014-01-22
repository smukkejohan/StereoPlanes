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
    
    oscReceiver.setup(9001);
    
    /*parameters.setName("Stereo");
    ofParameterGroup cameraParams;
    cameraParams.setName("Cameras");
    cameraParams.add(camPosWall.set("Wall", ofVec3f(0.,0.,-1), ofVec3f(-3,-3,-8.), ofVec3f(3,3,-0.25)));
    cameraParams.add(camPosFloor.set("Floor", ofVec3f(0.,0.,-1), ofVec3f(-3,-3,-8.), ofVec3f(3,3,-0.25)));
    cameraParams.add(eyeSeperation.set("Eye Seperation", 6.5, 0., 7.));
    parameters.add(cameraParams);
    
    ofParameterGroup trackingParams;
    trackingParams.setName("Tracking");
    trackingParams.add(dancerEllipseSize.set("Dancer Ellipse Size", 0., 0., .5));
    trackingParams.add(dancerEllipseBrightness.set("Dancer Ellipse Brightness", 0., 0., 1.));
    trackingParams.add(dancerPos.set("Dancer position", ofVec2f(-1.,-1.), ofVec2f(-1,-1), ofVec2f(1,1)));
    
     parameters.add(trackingParams);*/
    //parameters.add(meshOffsetFloor.set("Mesh Offset Floor", ofVec3f(0, 0, 1), ofVec3f(-4,-4,-4), ofVec3f(4,4,4)));
    //parameters.add(meshOffsetWall.set("Mesh Offset Wall", ofVec3f(0, 0, 1), ofVec3f(-4,-4,-4), ofVec3f(4,4,4)));
    //parameters.add(scene.set("Scene", 0, 0, 5));
    
    camPosFloor = ofVec3f(0, 0, -1);
    camPosWall = ofVec3f(0, 0, -1);
    
    testScene = new TestScene();
    testScene->setupScene();
    contentScenes.push_back(testScene);
    
    // add all parameterGroups from scenes to parameters
    //gui.setup(parameters);
    
    float dim = 16;
    float xInit = OFX_UI_GLOBAL_WIDGET_SPACING;
    float width = 255-xInit;
    hideGUI = false;
    
    gui = new ofxUIScrollableCanvas(0, 0, width+xInit, ofGetHeight());
    
    gui->setScrollAreaToScreenHeight();
    gui->setScrollableDirections(false, true);
    
    gui->setFont("GUI/Arial.ttf");
    gui->setWidgetFontSize(OFX_UI_FONT_SMALL);
    gui->setColorBack(ofColor(30, 30, 30,200));
    
    gui->addLabel("Stereo", OFX_UI_FONT_LARGE);
    gui->addFPSSlider("FPS");
    
    gui->addSlider("Eye seperation", 0, 7, &eyeSeperation);
    
    gui->add2DPad("Floor cam", ofVec2f(-3, 3), ofVec2f(-3,3), ofVec2f(0, 0));
    gui->addSlider("Floor Z", -2, 6, &camPosFloor.z);
    
    gui->add2DPad("Wall cam", ofVec2f(-3, 3), ofVec2f(-3,3), ofVec2f(0, 0));
    gui->addSlider("Wall Z",  -2, 6, &camPosWall.z);
    
    //gui->add2DPad(<#string _name#>, <#ofxUIVec3f _rangeX#>, <#ofxUIVec3f _rangeY#>, <#ofxUIVec3f *_value#>)
    
    //cameraParams.add(camPosWall.set("Wall", ofVec3f(0.,0.,-1), ofVec3f(-3,-3,-8.), ofVec3f(3,3,-0.25)));
    //cameraParams.add(camPosFloor.set("Floor", ofVec3f(0.,0.,-1), ofVec3f(-3,-3,-8.), ofVec3f(3,3,-0.25)));
    //cameraParams.add(eyeSeperation.set("Eye Seperation", 6.5, 0., 7.));
    
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
            
		} else if(m.getAddress() == "/Voronoi/x"){
           /* for(int i = 0; i < voronoiWall->breakPoints.size(); i++) {
                voronoiWall->breakPoints[i].pos.x = m.getArgAsFloat(i);
                voronoiWall->breakPoints[i].pressure += 0.001;
                
                voronoiPlaza->breakPoints[i].pos.x = m.getArgAsFloat(i);
                voronoiPlaza->breakPoints[i].pressure += 0.001;
            }*/
            
        } else if(m.getAddress() == "/Voronoi/y"){
            
              /*  for(int i = 0; i < voronoiWall->breakPoints.size(); i++) {
                
                    voronoiWall->breakPoints[i].pos.y = m.getArgAsFloat(i);
                
                    voronoiWall->breakPoints[i].pressure += 0.001;
                
                voronoiPlaza->breakPoints[i].pos.y = m.getArgAsFloat(i);
                voronoiPlaza->breakPoints[i].pressure += 0.001;
            
                }*/
            
        } else if(m.getAddress() == "/Voronoi/z"){
            
            /*for(int i = 0; i < voronoiWall->breakPoints.size(); i++) {
                //voronoiWall->breakPoints[i].pressure = m.getArgAsFloat(i);
            }*/
            
        } else if(m.getAddress() == "/planerot/x"){
           /* ofVec3f rot = ceilingPlane->rotation.get();
            rot.x = m.getArgAsFloat(0);
            ceilingPlane->rotation.set(rot);
            */
        } else if(m.getAddress() == "/breakstrength1/x"){
            /*if(currentScene == 3) {
            voronoiPlaza->wallBreakStrength.set(m.getArgAsFloat(0));
            voronoiWall->wallBreakStrength.set(m.getArgAsFloat(0));
            }*/
            
        } else if(m.getAddress() == "/OffsetWall/x"){
            //meshOffsetWall.set(ofVec3f(m.getArgAsFloat(0), meshOffsetWall.get().y, meshOffsetWall.get().z));
            
		} else if(m.getAddress() == "/OffsetWall/y"){
            //meshOffsetWall.set(ofVec3f(meshOffsetWall.get().x, m.getArgAsFloat(0), meshOffsetWall.get().z));
        }
        else if(m.getAddress() == "/OffsetFloor/x"){
            //meshOffsetFloor.set(ofVec3f(m.getArgAsFloat(0), meshOffsetFloor.get().y, meshOffsetWall.get().z));
            
		} else if(m.getAddress() == "/OffsetFloor/y"){
            //meshOffsetFloor.set(ofVec3f(meshOffsetFloor.get().x, m.getArgAsFloat(0), meshOffsetWall.get().z));
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
    
    gui->update();
    
    
}


void testApp::drawScenes(int _surfaceId) {
    for(int s=0; s<contentScenes.size();s++) {
        contentScenes[s]->drawScene(_surfaceId);
    }
}

//--------------------------------------------------------------
void testApp::draw()
{
    
    ofBackground(ofColor(0,0,0));
    
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
    
    if(showGrid) {
        for(int i=0; i<planes.size(); i++) {
            planes[i]->drawGrids();
        }
    }
    
    ofDisableDepthTest();
    ofDisableLighting();
    
    
    // Draw the scenes to the output fbo
    fbo.begin(); {
        ofClear(0, 0, 0);
        ofSetColor(255);
        ofFill();
    
        for(int i=0; i<planes.size(); i++) {
            planes[i]->draw();
        }
    }fbo.end();
    
    
    // Draw interface and monitor view
    
    ofPushMatrix();
    ofTranslate(300, 20);
    ofPushMatrix();
    ofScale(0.5,0.5);
    ofSetColor(255);
    fbo.draw(0, 0);
    ofPopMatrix();
    ofPopMatrix();
    
    ofSetColor(255);
    ofDrawBitmapString(ofToString(ofGetFrameRate()), 20, 20);
    gui->draw();
    
    sbsOutputServer.publishTexture(&fbo.getTextureReference());

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
	} else if(key == 's') {
        addSphere = true;
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
    
    for(int s=0; s<contentScenes.size();s++) {
        contentScenes[s]->exit();
    }
    
    gui->saveSettings("GUI/guiSettings.xml");
    delete gui;
    
    settings.save("stereoplanes.xml");
    cout<<"exit"<<endl;
}


void testApp::guiEvent(ofxUIEventArgs &e)
{
    
    string name = e.getName();
	int kind = e.getKind();
	//cout << "got event from: " << name << endl;
    
    if(name=="Floor cam") {
        ofxUI2DPad *pad = (ofxUI2DPad *) e.widget;
        camPosFloor.x = pad->getScaledValue().x;
        camPosFloor.y = pad->getScaledValue().y;
    }
    
    if(name=="Wall cam") {
        ofxUI2DPad *pad = (ofxUI2DPad *) e.widget;
        camPosWall.x = pad->getScaledValue().x;
        camPosWall.y = pad->getScaledValue().y;
    }
    
}