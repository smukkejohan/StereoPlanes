//
//  Ribbon.cpp
//  StereoPlanes
//
//  Created by Johan Bichel Lindegaard on 16/01/14.
//
//

#include "Ribbon.h"


void Ribbon::setup(ofParameterGroup * params) {
    
    params->add(speed.set("Ribbonspeed z", 0, 0, 1));
    
    params->add(origin.set("Ribbon origin", ofVec3f(0.1,0.5,0), ofVec3f(-200,-200,-200), ofVec3f(200,200,200)));
    
    params->add(autoAmp.set("Ribbon auto amplitude", ofVec2f(0.0,0.0), ofVec2f(-200,-200), ofVec2f(200,200)));
    params->add(autoSpeed.set("Ribbon auto speed",   ofVec2f(0.0,0.0), ofVec2f(0,0), ofVec2f(1,1)));
    params->add(autoOn.set("Ribbon auto on"));
    
    points.push_back(origin.get());
    
}

void Ribbon::update() {
    
    ofVec3f sumOfAllPoints(0,0,0);
    for(unsigned int i = 0; i < points.size(); i++){
        points[i].z += 10*speed.get();
        sumOfAllPoints += points[i];
    }
   // center = sumOfAllPoints / points.size();
    
    //ofVec3f p(ofSignedNoise(ofGetElapsedTimef()/10) * 200, ofSignedNoise(ofGetElapsedTimef()*2) * 200, 0);
    //origin.set(p);
    
    ofVec3f newPoint;
    
    newPoint = origin.get();
    
    if(autoOn.get()) {
        
        ofVec3f p( ofSignedNoise(ofGetElapsedTimef() * autoSpeed.get().x) * autoAmp.get().x,
                   ofSignedNoise(ofGetElapsedTimef() * autoSpeed.get().y) * autoAmp.get().y, 0 );
        newPoint = origin.get() + p;
        
    }
    
    if(lastPoint != newPoint) {
        points.push_back(newPoint);
        lastPoint = newPoint;
    }
    
}

void Ribbon::draw() {
    
    
    glPushMatrix();
    
    ofSetColor(255);
	//do the same thing from the first example...
    ofMesh mesh;
	mesh.setMode(OF_PRIMITIVE_TRIANGLES);
    
	for(unsigned int i = 1; i < points.size(); i++){
        
		//find this point and the next point
		ofVec3f thisPoint = points[i-1];
		ofVec3f nextPoint = points[i];
        
		//get the direction from one to the next.
		//the ribbon should fan out from this direction
		ofVec3f direction = (nextPoint - thisPoint);
        
		//get the distance from one point to the next
		float distance = direction.length();
        
		//get the normalized direction. normalized vectors always have a length of one
		//and are really useful for representing directions as opposed to something with length
		ofVec3f unitDirection = direction.normalized();
        
		//find both directions to the left and to the right
		ofVec3f toTheLeft = unitDirection.getRotated(-90, ofVec3f(0,0,1));
		ofVec3f toTheRight = unitDirection.getRotated(90, ofVec3f(0,0,1));
        
		//use the map function to determine the distance.
		//the longer the distance, the narrower the line.
		//this makes it look a bit like brush strokes
		float thickness = ofMap(distance, 0, 60, 6, 2, true);
        
		//calculate the points to the left and to the right
		//by extending the current point in the direction of left/right by the length
		ofVec3f leftPoint = thisPoint+toTheLeft*thickness;
		ofVec3f rightPoint = thisPoint+toTheRight*thickness;
        
        ofColor color = ofColor(ofMap(distance, 0, 60, 100, 255));
        
		//add these points to the triangle strip
		mesh.addVertex(ofVec3f(leftPoint.x, leftPoint.y, leftPoint.z));
        mesh.addColor(color);
		mesh.addVertex(ofVec3f(rightPoint.x, rightPoint.y, rightPoint.z));
        mesh.addColor(color);
        
	}
    
	//end the shape
	mesh.draw();
    ofSetColor(255);
    mesh.drawWireframe();
    
    glPopMatrix();
    
    
}