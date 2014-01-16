#include "testApp.h"
#include <OpenGL/OpenGL.h>
#include <ofGLUtils.h>
#include "ofxBulletWorldRigid.h"

// Syphon together with 3D primitive and NoFill does not run

vector<ofVec3f> points;

//--------------------------------------------------------------
void testApp::setup()
{
    ofSetFrameRate(30);
    ofSetVerticalSync(true);
    ofSetBackgroundAuto(true);
    ofBackground(0);
    
    dancerHeight = 0.5;
    
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
    parameters.add(camPos.set("Cam position", ofVec3f(0.,0.,-1), ofVec3f(-3,-3,-8.), ofVec3f(3,3,-0.25)));
    parameters.add(eyeSeperation.set("Eye Seperation", 6.5, 0., 7.));
    parameters.add(dancerPos.set("Dancer position", ofVec2f(-1.,-1.), ofVec2f(-1,-1), ofVec2f(1,1)));
    parameters.add(dancerEllipseSize.set("Ellipse Size", 0., 0., .5));
    parameters.add(dancerEllipseBrightness.set("Ellipse Brightness", 0., 0., 1.));
    
    parameters.add(shivering.set("Shiver", 0, 0, 6));
    parameters.add(wallSpeed.set("wallSpeed", 0, 0, 2));
    parameters.add(subdivisions.set("Subdivisions", 4, 0, 400));
    
    parameters.add(wallBreakPos.set("wallBreakPos", ofVec3f(0.1,0.5,0), ofVec3f(-1,-1,-1), ofVec3f(1,1,1)));
    parameters.add(wallBreakReach.set("wallBreakReach", ofVec3f(0.2,2,1), ofVec3f(0,0,0), ofVec3f(2,2,2)));
    parameters.add(wallBreakStrength.set("wallBreakStrength", 0, 0, 1.8));
    
    gui.setup(parameters);
    
    oscReceiver.setup(9001);
    
    // Bullet stuff
    
    world.setup();
    
    // gravity should be .98 for slowmo realism    
	world.setGravity(ofVec3f(0.f, 0.f, .4f));
    
    ground.create( world.world, ofVec3f(0., 0, 0.5), 0., 100.f, 100.f, 1.f );
	ground.setProperties(.25, .95);
	ground.add();

    wallBack.create( world.world, ofVec3f(0., -1.45, 0), 0., 100.f, 1.f, 100.f );
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
 btTransform tr	= ofGetBtTransformFromVec3f( a_loc );
 tr.setRotation( btQuaternion(btVector3(a_rot.x(), a_rot.y(), a_rot.z()), a_rot.w()) );

 */
    dancerCylinder.create(world.world,ofVec3f(0, 0, -dancerHeight/2.),/* ofQuaternion(0, ofVec3f(0, 0,1)),*/ 1. ,fmaxf(dancerEllipseSize, 0.25),fmaxf(dancerEllipseSize, 0.25), fmaxf(dancerEllipseSize, dancerHeight));

    dancerCylinder.setProperties(.75, .0);
    dancerCylinder.add();
    
    //  TODO: Operator grabbing of bullet objects from first view?
    //	world.enableGrabbing();
    //	world.enableDebugDraw();
    //	world.setCamera(&camera);
    
    
    // Voronoi wall
    vbounds.set(-0.9, -1, 1.8, 2);
    voronoi.setBounds(vbounds);
    
    genTheVoronoi();
    
} 


void testApp::genTheVoronoi() {
    int n = subdivisions.get();
    for(int i=0; i<n; i++) {
        vpts.push_back(ofRandomPointInRect(vbounds));
    }
    
    //vpts.push_back(ofVec3f(0,0,0));
    
    voronoi.clear();
    for(int i=0; i<vpts.size(); i++) {
        voronoi.addPoint(vpts[i]);
    }
    voronoi.generateVoronoi();

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
            
		} else if(m.getAddress() == "/Cameraz/x"){
            
            ofVec3f pos = camPos.get();
            pos.z = m.getArgAsFloat(0);
			camPos.set(pos);
            
		} else if(m.getAddress() == "/eyeSeperation/x"){
			eyeSeperation.set(m.getArgAsFloat(0));
            
		} else if(m.getAddress() == "/Dancer/x"){
            dancerPos.set(ofVec2f(m.getArgAsFloat(0), dancerPos.get().y));
		} else if(m.getAddress() == "/Dancer/y"){
            dancerPos.set(ofVec2f(dancerPos.get().x, m.getArgAsFloat(0)));
		}
    }

    dancerCylinder.setActivationState( DISABLE_DEACTIVATION );

    // this is a hack, thigs should not be moved like this in bullet, but it will do for now.
    
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
    //TODO: Camera frustrums share position, but with individual viewports
    for(int i=0; i<planes.size(); i++) {
        planes[i]->cam.setPosition(camPos.get());
        planes[i]->cam.setPhysicalEyeSeparation(eyeSeperation.get());
        planes[i]->update();
        //cout<<camPos.get()<<endl;
    }
    
    if(addSphere){
        ofxBulletSphere * sphere = new ofxBulletSphere();
        float mass = ofRandom(0.02,0.075);
        sphere->create(world.world, ofVec3f(ofRandom(-0.25,0.25), ofRandom(-0.25,0.25), -1), mass*0.1, mass);
        sphere->setProperties(0.1, 0.1);
        spheres.push_back(sphere);
        sphere->add();
        addSphere = false;
    }
    
    world.update();
    
    
    
    wallTime += 0.01 * wallSpeed;
    
}


void testApp::drawVoronoiWall() {
    
    ofRectangle bounds = ofRectangle(wallBreakPos.get().x-wallBreakReach.get().x/2, wallBreakPos.get().y-wallBreakReach.get ().y/2, wallBreakReach.get().x, wallBreakReach.get().y);
    
    
    bool changed = false;
    while(subdivisions.get() > voronoi.getPoints().size()) {
        voronoi.addPoint(ofRandomPointInRect(vbounds));
        changed = true;
    }
    
    while(subdivisions.get() < voronoi.getPoints().size()) {
        voronoi.getPoints().erase(voronoi.getPoints().begin());
        changed = true;
    }
    
    if(changed) {
        voronoi.generateVoronoi();
    }
    
    //TODO:Factor out to seperate class
    
    glPushMatrix();
    
    ofNoFill();
    
    light.enable();
    dirLight.enable();
    
    // draw a frame for the breaking wall
    ofFill();
    ofSetColor(200,230,200);
    // left
    /*ofRect(-1, -1, 0.1, 2);
    ofRect(0.9, -1, 0.1, 2);
    ofRect(-1, -1, 2, 0.1);
    ofRect(-1, 0.9, 2, 2);
    */
    
    
    ofPushMatrix();
    ofNoFill();
    ofSetLineWidth(5);
    ofSetColor(0);
        ofTranslate(wallBreakPos.get().x, wallBreakPos.get().y);
       // ofEllipse(0, 0, wallBreakReach.get().x, wallBreakReach.get().y);
    
    ofPopMatrix();
    
    //ofDrawRect(bounds.getPosition(), );
    
    
    //voronoi.draw();
    //voronoi.getPoints().size();
    
    for(int i=0; i < voronoi.cells.size(); i++) {
        
        ofMesh vcell;
        vcell.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
        
        bool inbreakzone = false;
        
        for(int v=0; v<voronoi.cells[i].pts.size(); v++) {
            
            vcell.addVertex(voronoi.cells[i].pts[v]);
            
            ofColor col;
            //if(i%2 == 0) {
            //    col.set(ofMap(i,0,voronoi.cells.size(),255, 2),255,250);
            //} else {
                col.set(ofMap(0.0, -0.2, 0.2, 255,100));
            //}
            
            if(bounds.inside(voronoi.cells[i].pts[v])) {
                inbreakzone = false;
            }
            
            vcell.addColor(col);
        }
        
        ofPushMatrix();
        
            if(inbreakzone) {
                
            }
        
       // ofRect(bounds.getPosition().x, bounds.getPosition().y, 0, bounds.getWidth(), bounds.getHeight());
        
        
            if(!bounds.inside(vcell.getCentroid())) {
                

                float z = ofSignedNoise(wallTime + i) * wallBreakStrength.get();
                ofTranslate(0, 0, z);
                //ofRotateY( ofSignedNoise(ofGetElapsedTimef()*2 + i) * shivering);
                
                for(int c=0; c<vcell.getColors().size(); c++) {
                    vcell.getColors()[c];
                    
                    vcell.setColor(c, ofColor(ofMap(z, -0.2, 0.2, 255,100)));
                                             
                }
                
                vcell.draw();
                
            }
        
        ofPopMatrix();
    }
    
    light.disable();
    dirLight.disable();
    
    ofDisableLighting();
    glPopMatrix();

}

void testApp::drawBulletFloor(){
    
    //TODO:Factor out to seperate class

    ofPushMatrix(); {
        //world.drawDebug();
        
        for(int i=0; i<spheres.size(); i++) {
            spheres[i]->draw();
        }
        
        // dancerCylinder.draw();
        
    } ofPopMatrix();
    
    
    //ofRect(-2, -2, 4, 4);
    
    
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
        ofSetColor(255*dancerEllipseBrightness,255*dancerEllipseBrightness,255*dancerEllipseBrightness,255);
        ofEllipse(dancerPos->x, dancerPos->y, dancerEllipseSize, dancerEllipseSize);
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
    
    floor->beginLeft();
    drawFloor();
    floor->endLeft();
    
    floor->beginRight();
    drawFloor();
    floor->endRight();
    
    wall->beginLeft();
    drawVoronoiWall();
    wall->endLeft();
    
    wall->beginRight();
    drawVoronoiWall();
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