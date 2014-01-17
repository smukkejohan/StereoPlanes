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
    
    parameters.setName("Stereo");
    
    parameters.add(camPosWall.set("Wall Cam", ofVec3f(0.,0.,-1), ofVec3f(-3,-3,-8.), ofVec3f(3,3,-0.25)));
    parameters.add(camPosFloor.set("Floor Cam", ofVec3f(0.,0.,-1), ofVec3f(-3,-3,-8.), ofVec3f(3,3,-0.25)));
    
    parameters.add(eyeSeperation.set("Eye Seperation", 6.5, 0., 7.));
    parameters.add(dancerEllipseSize.set("Dancer Ellipse Size", 0., 0., .5));
    parameters.add(dancerEllipseBrightness.set("Dancer Ellipse Brightness", 0., 0., 1.));
    parameters.add(dancerPos.set("Dancer position", ofVec2f(-1.,-1.), ofVec2f(-1,-1), ofVec2f(1,1)));
    parameters.add(meshOffsetFloor.set("Mesh Offset Floor", ofVec3f(0, 0, 1), ofVec3f(-4,-4,-4), ofVec3f(4,4,4)));
    parameters.add(meshOffsetWall.set("Mesh Offset Wall", ofVec3f(0, 0, 1), ofVec3f(-4,-4,-4), ofVec3f(4,4,4)));
   
    voronoiWall = new VoronoiWall();
    //voronoiWall->setup(&parameters);
    
    voronoiWall = new VoronoiWall();
    //voronoiWall->active = false;
    voronoiWall->setup(&parameters);
    
    ceilingPlane = new CeilingPlane();
    //ceilingPlane->setup(&parameters);
    
    //ribbon = new Ribbon();
    //ribbon->setup(&parameters);
    
    boxFloor = new BoxFloor();
    //boxFloor->setup(&parameters);

    wireMesh = new WireMesh();
    wireMesh->setup(&parameters);
    boxFloor->setup(&parameters);
    
    lines = new Lines();
    lines->setup(&parameters);
    
    gui.setup(parameters);
    
    
    oscReceiver.setup(9001);
    
    world.setup();
	world.setGravity(ofVec3f(0.f, 0.f, 9.8f));
    
    ground.create( world.world, ofVec3f(0., 0, 0.5), 0., 100.f, 100.f, 1.f );
	ground.setProperties(.25, .95);
	ground.add();
    
    wallBack.create( world.world, ofVec3f(0., -1.45, 0), 0., 100.f, 1.f, 100.f);
    wallBack.setProperties(.75, .75);
    wallBack.add();
    
    wallLeft.create( world.world, ofVec3f(-1.5, 0, 0), 0., 1.f, 100.f, 100.f );
    wallLeft.setProperties(.75, .75);
    wallLeft.add();
    
    wallRight.create( world.world, ofVec3f(1.5, 0, 0), 0., 1.f, 100.f, 100.f );
    wallRight.setProperties(.75, .75);
    wallRight.add();
    
    wallFront.create( world.world, ofVec3f(0., 1.5, 0), 0., 100.f, 1.f, 100.f );
    wallFront.setProperties(.75, .75);
    wallFront.add();
    /*
     ofxBulletCylinder::create( btDiscreteDynamicsWorld* a_world, ofVec3f a_loc, ofQuaternion a_rot, float a_mass, float a_radius, float a_height ) {
     btTransform tr        = ofGetBtTransformFromVec3f( a_loc );
     tr.setRotation( btQuaternion(btVector3(a_rot.x(), a_rot.y(), a_rot.z()), a_rot.w()) );
     */
    dancerCylinder.create(world.world,ofVec3f(0, 0, -dancerHeight/2.),/* ofQuaternion(0, ofVec3f(0, 0,1)),*/ 1. ,fmaxf(dancerEllipseSize, 0.25),fmaxf(dancerEllipseSize, 0.25), fmaxf(dancerEllipseSize, dancerHeight));
    
    
    
    //  TODO: Operator grabbing of bullet objects from first view?
    //	world.enableGrabbing();
    //	world.enableDebugDraw();
    //	world.setCamera(&camera);
    
}

//--------------------------------------------------------------
void testApp::update()
{
    while(oscReceiver.hasWaitingMessages()){
		// get the next message
		ofxOscMessage m;
		oscReceiver.getNextMessage(&m);
        
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
            
		} else if(m.getAddress() == "/voronoi/x"){
            
            for(int i = 0; i < voronoiWall->breakPoints.size(); i++) {
                voronoiWall->breakPoints[i].pos.x = m.getArgAsFloat(i);
                voronoiWall->breakPoints[i].pressure += 0.001;
            }
        } else if(m.getAddress() == "/voronoi/y"){
            
            for(int i = 0; i < voronoiWall->breakPoints.size(); i++) {
                voronoiWall->breakPoints[i].pos.y = m.getArgAsFloat(i);
                voronoiWall->breakPoints[i].pressure += 0.001;
            }
            
        } else if(m.getAddress() == "/voronoi/z"){
            
            for(int i = 0; i < voronoiWall->breakPoints.size(); i++) {
                //voronoiWall->breakPoints[i].pressure = m.getArgAsFloat(i);
            }
            
        }
        
        else if(m.getAddress() == "/OffsetWall/x"){
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
    
    dancerCylinder.setActivationState( DISABLE_DEACTIVATION );
    
    // This is a hack, thigs should not be moved like this in bullet, but it will do for now. This kind of movement makes jutters in the collisions.
    
    btTransform tr = dancerCylinder.getRigidBody()->getCenterOfMassTransform();
    dancerCylinder.getRigidBody()->translate(btVector3(dancerPos->x, dancerPos->y, -dancerHeight/2.)-tr.getOrigin());
    
    /* Instead this code should be the correct way of moving objects in bullet, however, right now it's not working.
     
     btTransform tr;
     tr.setIdentity();
     //    tr.setRotation(btQuaternion(btVector3(1.0, 0, 0), 90));
     dancerCylinder.getRigidBody()->tran
     tr.
     tr.setOrigin(btVector3(dancerPos->x, dancerPos->y, -.5));
     
     if(dancerConstraint != NULL){
     world.world->removeConstraint( dancerConstraint );
     delete dancerConstraint;
     dancerConstraint = NULL;
     }
     
     dancerConstraint = new btGeneric6DofConstraint(*dancerCylinder.getRigidBody(), tr, false);
     dancerConstraint->setLinearLowerLimit(btVector3(0,0,0));
     dancerConstraint->setLinearUpperLimit(btVector3(0,0,0));
     dancerConstraint->setAngularLowerLimit(btVector3(0,0,0));
     dancerConstraint->setAngularUpperLimit(btVector3(0,0,0));
     
     world.world->addConstraint(dancerConstraint);
     
     dancerConstraint->setParam(BT_CONSTRAINT_STOP_CFM,0.8,0);
     dancerConstraint->setParam(BT_CONSTRAINT_STOP_CFM,0.8,1);
     dancerConstraint->setParam(BT_CONSTRAINT_STOP_CFM,0.8,2);
     dancerConstraint->setParam(BT_CONSTRAINT_STOP_CFM,0.8,3);
     dancerConstraint->setParam(BT_CONSTRAINT_STOP_CFM,0.8,4);
     dancerConstraint->setParam(BT_CONSTRAINT_STOP_CFM,0.8,5);
     
     dancerConstraint->setParam(BT_CONSTRAINT_STOP_ERP,0.1,0);
     dancerConstraint->setParam(BT_CONSTRAINT_STOP_ERP,0.1,1);
     dancerConstraint->setParam(BT_CONSTRAINT_STOP_ERP,0.1,2);
     dancerConstraint->setParam(BT_CONSTRAINT_STOP_ERP,0.1,3);
     dancerConstraint->setParam(BT_CONSTRAINT_STOP_ERP,0.1,4);
     dancerConstraint->setParam(BT_CONSTRAINT_STOP_ERP,0.1,5);
     
     dancerConstraint->getFrameOffsetA().setOrigin(btVector3(dancerPos->x, dancerPos->y, -.5));
     */
    planes[0]->cam.setPosition(camPosFloor.get());
    planes[1]->cam.setPosition(camPosWall.get());
    
    for(int i=0; i<planes.size(); i++) {
        planes[i]->cam.setPhysicalEyeSeparation(eyeSeperation.get());
        planes[i]->update();
        //cout<<camPos.get()<<endl;
    }
    
    /*
     //TODO: Camera frustrums share position, but with individual viewports
     planes[1]->cam.setPosition(camPos.get());
     planes[1]->update();
     
     ofVec4f camInScreenSpace = planes[1]->warpLeft.fromWarpToScreenCoord(camPos.get().x, camPos.get().y, camPos.get().z);
     ofVec4f camInFloorSpace = planes[0]->warpLeft.fromScreenToWarpCoord(camInScreenSpace.x, camInScreenSpace.y, camInScreenSpace.z);
     planes[0]->cam.setPosition(camInScreenSpace.x, camInScreenSpace.y, camInScreenSpace.z);
     planes[0]->update();
     */
    
    if(addSphere){
        ofxBulletSphere * sphere = new ofxBulletSphere();
        sphere->create(world.world, ofVec3f(ofRandom(-0.1,0.1), ofRandom(-0.1,0.1), -1), 0.05, ofRandom(0.02,0.05));
        spheres.push_back(sphere);
        sphere->add();
        addSphere = false;
    }
    
    world.update();
    
    voronoiWall->update();
    //ribbon->update();
    boxFloor->update();
    wireMesh->update(dancerPos.get());
    ceilingPlane->update();
    lines->update();
}



void testApp::drawBulletFloor(){
    
    //TODO:Factor out to seperate class
    
    ofPushMatrix(); {
        //ofRotateY(90);
        //world.drawDebug();
        
        for(int i=0; i<spheres.size(); i++) {
            spheres[i]->draw();
        }
        
        
    } ofPopMatrix();
}

void testApp::drawFloor() {
    
    glPushMatrix();{
        
        ofEnableLighting();
        light.enable();
        dirLight.enable();
        
        //TODO: Find better fog parameters...
        /*glEnable (GL_FOG);
         glFogi (GL_FOG_MODE, GL_EXP2);
         glHint (GL_FOG_HINT, GL_NICEST);
         glFogi(GL_FOG_DENSITY, 1);
         */
        
        /** A rotating box intersects the floor
         ofPushMatrix();
         ofFill();
         ofSetColor(255,255,255,255);
         ofSetBoxResolution(10);
         ofRotateX(ofGetElapsedTimef()*10);
         ofDrawBox(0.5);
         ofPopMatrix();
         **/
        
        // A WHITE FLOOR
        ofSetColor(255,255,255,255);
        ofRect(-1,-1, 2, 2);
        
        drawBulletFloor();
        
        /** Some random moving balls
         for (int i = 0; i< 200; i++) {
         ofDrawSphere(ofSignedNoise((ofGetElapsedTimef()*0.01)+i, 0, 0), ofSignedNoise((ofGetElapsedTimef()*0.01)+i, (ofGetElapsedTimef()*0.01)+i, 0), ofSignedNoise(0,0,(ofGetElapsedTimef()*0.01)+i)*0.5,  0.05);
         }
         **/
        
        light.disable();
        dirLight.disable();
        ofDisableLighting();
        
        ofDisableDepthTest();
        ofSetColor(0,0,0,255);
        ofEllipse(dancerPos->x, dancerPos->y, 0.02, 0.02);
        ofEnableDepthTest();
        
        //glDisable(GL_FOG);
        
    } glPopMatrix();
    
}

//--------------------------------------------------------------
void testApp::draw()
{
    ofEnableDepthTest();
    ofBackground(ofColor(0,0,0));
    
    fbo.begin();
    
    ofClear(0, 0, 0);

#pragma mark DRAW FLOOR
    
    floor->beginLeft(); {
        ofPushMatrix();
        /* this will rotate the floor to match the screen space
         ofTranslate(0, -1);
         ofRotate(90, 1, 0, 0);
         ofTranslate(0, 1);
         */
        //voronoiWall->draw();
        //boxFloor->draw( dancerPos.get() );
        //lines->draw();
        wireMesh->draw(meshOffsetFloor.get());
        ofPopMatrix();
    } floor->endLeft();
    
    floor->beginRight(); {
        ofPushMatrix();
        /* this will rotate the floor to match the screen space
         ofTranslate(0, -1);
         ofRotate(90, 1, 0, 0);
         ofTranslate(0, 1);
         */
        //voronoiWall->draw();
        //boxFloor->draw( dancerPos.get() );
        //lines->draw();
        wireMesh->draw(meshOffsetFloor.get());
        ofPopMatrix();
    } floor->endRight();
    
#pragma mark DRAW WALL

    wall->beginLeft(); {
        //voronoiWall->draw();
        //lines->draw();
        //ceilingPlane->draw();
        //ribbon->draw();
        wireMesh->draw(meshOffsetWall.get());
    } wall->endLeft();
    
    wall->beginRight(); {
        //voronoiWall->draw();
        //ceilingPlane->draw();
        //ribbon->draw();
        //lines->draw();
        wireMesh->draw(meshOffsetWall.get());
    } wall->endRight();
    
    
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
    ofDrawBitmapString(ofToString(ofGetFrameRate()), 40, 40);
    //activePlane->drawInfo();
    
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
    
    settings.save("stereoplanes.xml");
    cout<<"exit"<<endl;
}