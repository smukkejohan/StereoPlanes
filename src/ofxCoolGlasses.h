#pragma once

#include "ofMain.h"

class ofxCoolGlasses {
public:
    
    ofCamera left;
    ofCamera right;
    
    ofxCoolGlasses(): physical_eye_seperation_cm(6.5), physical_focus_distance_cm(200), focus_distance(200) {};
    ~ofxCoolGlasses(){};
    
	void setup(int w, int h)
	{
		width = w;
		height = h;
        
		leftFbo.allocate(width, height, GL_RGBA);
		rightFbo.allocate(width, height, GL_RGBA);
        
        left.setScale(1, 1, 1);
        left.setNearClip(0.1);
        
        right.setScale(1, 1, 1);
        right.setNearClip(0.1);

	}
    
    void setPosition(ofVec3f pos) {
        left.setPosition(pos);
        right.setPosition(pos);
    }
    
    void setupPerspective(){
        left.setupPerspective();
        right.setupPerspective();
    }
    
    void lookAt(ofVec3f v){
        left.lookAt(v);
        right.lookAt(v);
    }
    
	void update(ofRectangle viewport = ofGetCurrentViewport())
	{
 
		eye = physical_eye_seperation_cm / physical_focus_distance_cm;
        
		/*aspect = viewport.width / viewport.height;
		fovy_tan = tanf(PI * left.getFov() / 360.);
		fovx_tan = fovy_tan * aspect;
         */
        
		//zNear = left.getNearClip();
		//zFar  = left.getFarClip();
        
        right.setPosition(left.getPosition().x + eye, left.getPosition().y, left.getPosition().z);
        
        right.setupOffAxisViewPortal(viewport.getTopLeft(), viewport.getBottomLeft(), viewport.getBottomRight());
        left.setupOffAxisViewPortal(viewport.getTopLeft(), viewport.getBottomLeft(), viewport.getBottomRight());

	}

	void setPhysicalEyeSeparation(float cm) { physical_eye_seperation_cm = cm; }
	void setPhysicalFocusDistance(float cm) { physical_focus_distance_cm = cm; }
	void setFocusDistance(float v) { focus_distance = v; }

	void beginLeft()
	{
        leftFbo.begin();
		ofFloatColor c = ofGetCurrentRenderer()->getBgColor();
		ofClear(c);
        left.begin();

	}

	void endLeft()
	{
        left.end();
        leftFbo.end();

	}

	void beginRight()
	{
        rightFbo.begin();
		ofFloatColor c = ofGetCurrentRenderer()->getBgColor();
		ofClear(c);
        right.begin();
    }

	void endRight()
	{
	    right.end();
        rightFbo.end();
	}
    
    ofFbo leftFbo, rightFbo;

protected:

	int width, height;

	float eye, focus_distance;

	float physical_eye_seperation_cm;
	float physical_focus_distance_cm;

	// parallel

	float zNear, zFar;
	float fovx_tan, fovy_tan;
	float aspect;
	
private:

};