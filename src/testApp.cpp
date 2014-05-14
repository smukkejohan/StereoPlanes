#include "testApp.h"
#include <OpenGL/OpenGL.h>
#include <ofGLUtils.h>

// Syphon together with 3D primitive and NoFill does not run

vector<ofVec3f> points;

//--------------------------------------------------------------
void testApp::setup()
{
    
    ofSetLogLevel(OF_LOG_ERROR);
    ofSetFrameRate(30);
    ofSetVerticalSync(true);
    ofSetBackgroundAuto(true);
    ofBackground(0);
    
    //leftOutputServer.setName("Left");
    //rightOutputServer.setName("Right");
    sbsOutputServer.setName("Side By Side");
    
    // we output 4x1920x1080 layered in two units
    ofSeedRandom(10);
    
    int resx = 1920;
    int resy = 1080;
    
    fbo.allocate(resx*3, resy);
    
    settings.load("stereoplanes.xml");
    
    floor = new StereoPlane("floor");
    floor->setup(resx/2, resy, &settings);
    floor->setViewPort(ofRectangle(-1, -1, 2, 2));
    floor->pos = ofVec2f(0,0);
    planes.push_back(floor);
        
    wall = new StereoPlane("wallLeft");
    wall->setup(resx/2, resy, &settings);
    wall->setViewPort(ofRectangle(-1, -1, 2, 2));
    wall->pos = ofVec2f(resx,0);
    planes.push_back(wall);
    
    wallRight = new StereoPlane("wallRight");
    wallRight->setup(resx/2, resy, &settings);
    wallRight->setViewPort(ofRectangle(-1, -1, 2, 2));
    wallRight->pos = ofVec2f(resx*2,0);
    
    planes.push_back(wallRight);
    
    activePlaneIndex = 0;
    activePlane = planes[activePlaneIndex];
    
    oscReceiver.setup(9001);
    
    // #### Setup timeline
    timeline.setup();
    timeline.setupFont("GUI/Arial.ttf", 7);
    timeline.setDurationInSeconds(60*30); // half an hour
    
    tlAudioMain = timeline.addAudioTrack("Samlet lyd");
    tlAudioMain->loadSoundfile("audio/SAMLET_stereo_click.wav");
    
    tlAudioMain->setUseFFTEnvelope(false);
    
    // #### pointers to lights, a hack for different light on different surfaces
    
    shaderLights.push_back(new ofxOlaShaderLight);
    
    
    // ####  Setup scenes
    testScene = new TestScene();
    contentScenes.push_back(testScene);
    
    voroWallLeft = new VoronoiWall();
    voroWallLeft->setSurface(1);
    voroWallLeft->setName("voroLeft");
    contentScenes.push_back(voroWallLeft);
    
    voroWallRight = new VoronoiWall();
    voroWallRight->setSurface(2);
    voroWallRight->setName("voroRight");
    contentScenes.push_back(voroWallRight);
    
    voroFloor = new VoronoiWall();
    voroFloor->setName("voroFloor");
    voroFloor->setSurface(0);
    contentScenes.push_back(voroFloor);
    
    crystalsLeft = new GhostLights();
    crystalsLeft->setSurface(1);
    crystalsLeft->setName("Crystal left");
    contentScenes.push_back(crystalsLeft);
    
    crystalsRight = new GhostLights();
    crystalsRight->setSurface(2);
    crystalsRight->setName("Crystal right");
    contentScenes.push_back(crystalsRight);
    
    lights = new Lights();
    lights->enabled = true;
    contentScenes.push_back(lights);
    
    flyLightLeft = new FlyLight();
    flyLightLeft->setName("flyLightLeft");
    flyLightLeft->setLightPtr(shaderLights[0]);
    flyLightLeft->setSurface(1);
    flyLightLeft->enabled = true;
    contentScenes.push_back(flyLightLeft);
    
    flyLightRight = new FlyLight();
    flyLightRight->setName("flyLightRight");
    flyLightRight->setLightPtr(shaderLights[0]);
    flyLightRight->setSurface(2);
    flyLightRight->enabled = true;
    contentScenes.push_back(flyLightRight);
    
    flyLightFloor = new FlyLight();
    flyLightFloor->setName("flyLightFloor");
    flyLightFloor->setLightPtr(shaderLights[0]);
    flyLightFloor->setSurface(0);
    flyLightFloor->enabled = true;
    contentScenes.push_back(flyLightFloor);
    
    
    for(int i=0; i<contentScenes.size(); i++) {
        contentScenes[i]->mainTimeline = &timeline;
        contentScenes[i]->setupScene(i);
    }

    // #### Setup GUI's
    float xInit = OFX_UI_GLOBAL_WIDGET_SPACING;
    guiWidth = 255;
    float w = guiWidth-xInit*2;
    
    ofxUIScrollableCanvas * mainGui;
    mainGui = new ofxUIScrollableCanvas(0, 0, guiWidth-xInit, ofGetHeight());
    guis.push_back(mainGui);
    
    ofxUIScrollableCanvas * sceneGui;
    sceneGui = new ofxUIScrollableCanvas((guiWidth+xInit)*2, 0, guiWidth-xInit, ofGetHeight());
    guis.push_back(sceneGui);
    
    ofxUIScrollableCanvas * planeGui;
    planeGui = new ofxUIScrollableCanvas(guiWidth+xInit, 0, guiWidth-xInit, ofGetHeight());
    guis.push_back(planeGui);
    
    for(int i=0; i<guis.size(); i++) {
        guis[i]->setDrawBack(true);
        guis[i]->setScrollableDirections(false, true);
        guis[i]->setFont("GUI/Arial.ttf");
        guis[i]->setWidgetFontSize(OFX_UI_FONT_SMALL);
        guis[i]->setColorBack(ofColor(10, 10, 10,220));
    }
    
    mainGui->setName("Main");
    mainGui->addLabel("Stereo", OFX_UI_FONT_LARGE);
    mainGui->addSpacer(w, 3)->setDrawOutline(true);
    mainGui->addFPS();
    mainGui->addSlider("Eye seperation", 0, 7, &eyeSeperation);
    mainGui->addToggle("Draw checkers", &drawChessboards);
    mainGui->addToggle("Draw planes", &drawGrids);
    mainGui->addToggle("Draw FBOs", &drawMonitor);
    
    planeGui->setName("Planes");
    planeGui->addLabel("Surfaces", OFX_UI_FONT_LARGE);
    for(int i=0; i<planes.size(); i++) {
        planeGui->addSpacer(w, 3)->setDrawOutline(true);
        
        planeGui->addSlider(planes[i]->name + " Cam X",  -2, 2, &planes[i]->camPos.x);
        planeGui->addSlider(planes[i]->name + " Cam Y",  -2, 2, &planes[i]->camPos.y);
        planeGui->addSlider(planes[i]->name + " Cam Z",  -2, -0.25, &planes[i]->camPos.z);
        planeGui->addSlider(planes[i]->name + " Aspect",  0, 2, &planes[i]->aspect);
        
    }
    
    
    sceneGui->setName("Scenes");
    sceneGui->addLabel("Scenes", OFX_UI_FONT_LARGE);
    for(int i=0; i<contentScenes.size(); i++) {
        sceneGui->addSpacer(w, 3)->setDrawOutline(true);
        contentScenes[i]->setGui(sceneGui, w);
    }
    
    for(int i=0; i<guis.size(); i++) {
        guis[i]->loadSettings("GUI/" + guis[i]->getName() + ".xml");
        guis[i]->autoSizeToFitWidgets();
        guis[i]->setAutoDraw(false);
        ofAddListener(guis[i]->newGUIEvent,this,&testApp::guiEvent);
    }
    
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
        
		if(m.getAddress() == "/eyeSeperation/x"){
			eyeSeperation = m.getArgAsFloat(0);
            
		} else if(m.getAddress() == "/activescene/x"){
            for(int i=0; i<contentScenes.size(); i++) {
                contentScenes[i]->enabled = false;
            }
            contentScenes[m.getArgAsInt32(0)]->enabled = true;
        }
    }
    
    
    for(int i=0; i<planes.size(); i++) {
        planes[i]->cam.setPhysicalEyeSeparation(eyeSeperation);
        planes[i]->update();
    }

    for(int s=0; s<contentScenes.size();s++) {
        contentScenes[s]->updateScene();
    }
    
    //ofSetWindowTitle(ofToString(ofGetFrameRate()));
}


void testApp::drawScenes(int _surfaceId) {
    
    ofClear(ofColor::black);
    glPushMatrix();
    
    for(int s=0; s<contentScenes.size();s++) {
        contentScenes[s]->beginSceneWorld(_surfaceId);
    }
    
    for(int s=0; s<contentScenes.size();s++) {
        contentScenes[s]->drawScene(_surfaceId);
    }
    
    for(int s=0; s<contentScenes.size();s++) {
        contentScenes[s]->endSceneWorld(_surfaceId);
    }
    
    glPopMatrix();
    
}

//--------------------------------------------------------------
void testApp::draw()
{
    
    float fboHeight = 200;//(ofGetWidth())*(fbo.getHeight()*1./fbo.getWidth());
    float fboWidth = fbo.getWidth() * (fboHeight/fbo.getHeight());
    
    //float fboHeight = 200;
    
    for(int s=0; s<contentScenes.size();s++) {
        contentScenes[s]->time = timeline.getCurrentTime();
    }
    
    ofSetColor(255);
    ofEnableDepthTest();
    ofEnableAlphaBlending();
    glEnable(GL_DEPTH_TEST);
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
    
    if(drawChessboards) {
        for(int i=0; i<planes.size(); i++) {
            planes[i]->drawChessboards();
        }
    }
    
    if(drawGrids) {
        for(int i=0; i<planes.size(); i++) {
            planes[i]->drawGrids();
        }
    }
    
    // Draw the scenes to the output fbo
    fbo.begin(); {
        //ofClear(0.0, 0.0, 0.0, 1.0); // This causes flickering if alpha gets cleared
        ofSetColor(255);
        ofFill();
        for(int i=0; i<planes.size(); i++) {
            planes[i]->draw();
        }
        
    }fbo.end();
    
    ofBackground(20);
    
    if(drawMonitor) {
        ofSetColor(255,255);
        fbo.draw(0,0,fboWidth,fboHeight);
        
    } else {
        fboHeight = 0;
    }
    
    
    if(drawTimeline) {
        timeline.setOffset(ofVec2f(0,fboHeight));
        timeline.draw();
    }
    
    
        for(int i=0; i<guis.size(); i++) {
            
            /*guis[i]->setScrollArea(guis[i]->getRect()->x, ofGetHeight()-200, guiWidth, 200);
            */
            
            guis[i]->setPosition(guis[i]->getRect()->x, ofGetHeight()-600);
            guis[i]->setScrollAreaHeight(400);
            
            if(drawGUI) {
                guis[i]->setVisible(true);
                guis[i]->draw();
            } else {
                guis[i]->setVisible(false);
            }
    }
    
    sbsOutputServer.publishFBO(&fbo);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
	if (key == 'f'){
		ofToggleFullscreen();
    } else if(key=='t') {
        
        drawTimeline = !drawTimeline;
        
    } else if(key=='g') {
        drawGUI = !drawGUI;
        
    } else if(key=='p') {
        drawMonitor = !drawMonitor;
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
    
    for(int i=0; i<guis.size(); i++) {
        guis[i]->saveSettings("GUI/"+guis[i]->getName() + ".xml");
        delete guis[i];
        
    }
    
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