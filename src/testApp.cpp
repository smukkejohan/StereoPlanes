#include "testApp.h"
#include <OpenGL/OpenGL.h>
#include <ofGLUtils.h>

// Syphon together with 3D primitive and NoFill does not run

vector<ofVec3f> points;

//--------------------------------------------------------------
void testApp::setup()
{
    
    currentScene = 6;
    
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
    
    testScene = new TestScene();
    testScene->setupScene();
    contentScenes.push_back(testScene);
    
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
    
    parameters.add(camPosWall.set("Wall Cam", ofVec3f(0.,0.,-1), ofVec3f(-3,-3,-8.), ofVec3f(3,3,-0.25)));
    parameters.add(camPosFloor.set("Floor Cam", ofVec3f(0.,0.,-1), ofVec3f(-3,-3,-8.), ofVec3f(3,3,-0.25)));
    
    parameters.add(eyeSeperation.set("Eye Seperation", 6.5, 0., 7.));
    parameters.add(dancerEllipseSize.set("Dancer Ellipse Size", 0., 0., .5));
    parameters.add(dancerEllipseBrightness.set("Dancer Ellipse Brightness", 0., 0., 1.));
    parameters.add(dancerPos.set("Dancer position", ofVec2f(-1.,-1.), ofVec2f(-1,-1), ofVec2f(1,1)));
    
    //parameters.add(meshOffsetFloor.set("Mesh Offset Floor", ofVec3f(0, 0, 1), ofVec3f(-4,-4,-4), ofVec3f(4,4,4)));
    //parameters.add(meshOffsetWall.set("Mesh Offset Wall", ofVec3f(0, 0, 1), ofVec3f(-4,-4,-4), ofVec3f(4,4,4)));
    
    //parameters.add(scene.set("Scene", 0, 0, 5));
    
    
    
    
    oscReceiver.setup(9001);
    gui.setup(parameters);
    
}


//--------------------------------------------------------------
void testApp::update()
{
    
    
    while(oscReceiver.hasWaitingMessages()){
		// get the next message
		ofxOscMessage m;
		oscReceiver.getNextMessage(&m);
        
        //cout<<m.getAddress()<<endl;
        
		if(m.getAddress() == "/Floor/Camera/x"){
            
            ofVec3f pos = camPosFloor.get();
            pos.x = m.getArgAsFloat(0);
			camPosFloor.set(pos);
            
		} else if(m.getAddress() == "/Floor/Camera/y"){
            ofVec3f pos = camPosFloor.get();
            pos.y = m.getArgAsFloat(0);
			camPosFloor.set(pos);
            
		} else if(m.getAddress() == "/Floor/Cameraz/x"){
            
            ofVec3f pos = camPosFloor.get();
            pos.z = m.getArgAsFloat(0);
			camPosFloor.set(pos);
            
        } else if(m.getAddress() == "/Wall/Camera/x"){
                ofVec3f pos = camPosWall.get();
                pos.x = m.getArgAsFloat(0);
                camPosWall.set(pos);
                
        } else if(m.getAddress() == "/Wall/Camera/y"){
                ofVec3f pos = camPosWall.get();
                pos.y = m.getArgAsFloat(0);
                camPosWall.set(pos);
                
        } else if(m.getAddress() == "/Wall/Cameraz/x"){
                
                ofVec3f pos = camPosWall.get();
                pos.z = m.getArgAsFloat(0);
                camPosWall.set(pos);
            
		} else if(m.getAddress() == "/eyeSeperation/x"){
			eyeSeperation.set(m.getArgAsFloat(0));
            
		} else if(m.getAddress() == "/Dancer/x"){
            dancerPos.set(ofVec2f(m.getArgAsFloat(0), dancerPos.get().y));
            
		} else if(m.getAddress() == "/Dancer/y"){
            dancerPos.set(ofVec2f(dancerPos.get().x, m.getArgAsFloat(0)));
            
		} else if(m.getAddress() == "/Voronoi/x"){
            if(currentScene == 3) {
           /* for(int i = 0; i < voronoiWall->breakPoints.size(); i++) {
                voronoiWall->breakPoints[i].pos.x = m.getArgAsFloat(i);
                voronoiWall->breakPoints[i].pressure += 0.001;
                
                voronoiPlaza->breakPoints[i].pos.x = m.getArgAsFloat(i);
                voronoiPlaza->breakPoints[i].pressure += 0.001;
            }*/
            }
        } else if(m.getAddress() == "/Voronoi/y"){
            if(currentScene == 3) {
            
              /*  for(int i = 0; i < voronoiWall->breakPoints.size(); i++) {
                
                    voronoiWall->breakPoints[i].pos.y = m.getArgAsFloat(i);
                
                    voronoiWall->breakPoints[i].pressure += 0.001;
                
                voronoiPlaza->breakPoints[i].pos.y = m.getArgAsFloat(i);
                voronoiPlaza->breakPoints[i].pressure += 0.001;
            
                }*/
            }
            
        } else if(m.getAddress() == "/Voronoi/z"){
            
            /*for(int i = 0; i < voronoiWall->breakPoints.size(); i++) {
                //voronoiWall->breakPoints[i].pressure = m.getArgAsFloat(i);
            }*/
            
        } else if(m.getAddress() == "/planerot/x"){
            ofVec3f rot = ceilingPlane->rotation.get();
            rot.x = m.getArgAsFloat(0);
            ceilingPlane->rotation.set(rot);
            
        } else if(m.getAddress() == "/breakstrength1/x"){
            /*if(currentScene == 3) {
            voronoiPlaza->wallBreakStrength.set(m.getArgAsFloat(0));
            voronoiWall->wallBreakStrength.set(m.getArgAsFloat(0));
            }*/
            
        } else if(m.getAddress() == "/OffsetWall/x"){
            meshOffsetWall.set(ofVec3f(m.getArgAsFloat(0), meshOffsetWall.get().y, meshOffsetWall.get().z));
            
		} else if(m.getAddress() == "/OffsetWall/y"){
            meshOffsetWall.set(ofVec3f(meshOffsetWall.get().x, m.getArgAsFloat(0), meshOffsetWall.get().z));
        }
        else if(m.getAddress() == "/OffsetFloor/x"){
            meshOffsetFloor.set(ofVec3f(m.getArgAsFloat(0), meshOffsetFloor.get().y, meshOffsetWall.get().z));
            
		} else if(m.getAddress() == "/OffsetFloor/y"){
            meshOffsetFloor.set(ofVec3f(meshOffsetFloor.get().x, m.getArgAsFloat(0), meshOffsetWall.get().z));
        }

    }
    
    planes[0]->cam.setPosition(camPosFloor.get());
    planes[1]->cam.setPosition(camPosWall.get());
    
    for(int i=0; i<planes.size(); i++) {
        planes[i]->cam.setPhysicalEyeSeparation(eyeSeperation.get());
        planes[i]->update();
    }

    for(int s=0; s<contentScenes.size();s++) {
        contentScenes[s]->update();
    }
    
    
}




void testApp::drawScenes(int _surfaceId) {
    for(int s=0; s<contentScenes.size();s++) {
        contentScenes[s]->drawScene(_surfaceId);
    }
}

//--------------------------------------------------------------
void testApp::draw()
{
    ofEnableDepthTest();
    ofBackground(ofColor(0,0,0));

    fbo.begin();
    
    ofClear(0, 0, 0);
    
    for(int i=0;i < planes.size(); i++) {
        
        planes[i]->beginLeft();
        drawScenes(i);
        planes[i]->endLeft();
        
        planes[i]->beginRight();
        drawScenes(i);
        planes[i]->endRight();
        
    }
    
/*
    floor->beginLeft(); {
        ofPushMatrix();
        
        
        if(currentScene == 6) {
            ofPushMatrix();
            cmpTest->transformFloor();
            commonWorldDraw();
            ofPopMatrix();
        }
        
        if(currentScene == 0) {
            drawFloor();
        }
        
        if(currentScene == 2) {
            boxFloor->draw( dancerPos.get() );
        }
        
        if(currentScene == 1) {
            ofPushMatrix();
            cmpTest->transformFloor();
            wireMesh->draw(meshOffsetFloor.get());
            ofPopMatrix();
        }
        
        
        if(currentScene == 5) {
            cmpTest->drawFloor();
            
            
            
        }
        
        this will rotate the floor to match the screen space
         ofTranslate(0, -1);
         ofRotate(90, 1, 0, 0);
         ofTranslate(0, 1);
         
        //voronoiWall->draw();
        //
        //voronoiWall->draw();
    //    ofPopMatrix();
    //} floor->endLeft();
 
    floor->beginRight(); {
        ofPushMatrix();
        
        if(currentScene == 6) {
            ofPushMatrix();
            cmpTest->transformFloor();
            commonWorldDraw();
            ofPopMatrix();
        }
        
        if(currentScene == 0) {
            drawFloor();
        }
        
        if(currentScene == 2) {
            boxFloor->draw( dancerPos.get() );
        }
        
        if(currentScene == 1) {
            ofPushMatrix();
            cmpTest->transformFloor();
            wireMesh->draw(meshOffsetFloor.get());
            ofPopMatrix();
        }
        
        if(currentScene == 5) {
            cmpTest->drawFloor();
        }
        //voronoiWall->draw();
        this will rotate the floor to match the screen space
         ofTranslate(0, -1);
         ofRotate(90, 1, 0, 0);
         ofTranslate(0, 1);
     
        //voronoiWall->draw();
        //boxFloor->draw( dancerPos.get() );
        //lines->draw();
        //wireMesh->draw(meshOffsetFloor.get());
      //  ofPopMatrix();
   // } floor->endRight();
    

    wall->beginLeft(); {
        
        if(currentScene == 6) {
            ofPushMatrix();
            cmpTest->transformWall();
            commonWorldDraw();
            ofPopMatrix();
        }
        
        if(currentScene == 1) {
            ofPushMatrix();
            cmpTest->transformWall();
            wireMesh->draw(meshOffsetFloor.get());
            ofPopMatrix();
        }
        
        if(currentScene == 3) {
            //voronoiWall->draw();
        }
        
        if(currentScene == 5) {
            cmpTest->drawWall();
        }
        
        
    } wall->endLeft();
    
    wall->beginRight(); {
        
        
        if(currentScene == 6) {
            ofPushMatrix();
            cmpTest->transformWall();
            commonWorldDraw();
            ofPopMatrix();
        }
        
        
        if(currentScene == 1) {
            ofPushMatrix();
            cmpTest->transformWall();
            wireMesh->draw(meshOffsetFloor.get());
            ofPopMatrix();
        }
        
        if(currentScene == 3) {
            //voronoiWall->draw();
        }
        
        if(currentScene == 5) {
            cmpTest->drawWall();
        }
        
        
    } wall->endRight();
    */
    
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
    ofScale(0.5,0.5);
    ofRect(0,0,2048,768);
    fbo.draw(0, 0);
    ofPopMatrix();
    
    ofSetColor(255);
    ofDrawBitmapString(ofToString(ofGetFrameRate()), 20, 20);
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
    
    settings.save("stereoplanes.xml");
    cout<<"exit"<<endl;
}