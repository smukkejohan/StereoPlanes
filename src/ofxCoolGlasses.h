#pragma once

#include "ofMain.h"

/*struct ofxStereoCameraViewMode
{
	enum Mode
	{
		PARALLEL,
		TOE_IN
	};
};

struct ofxStereoCameraOutputMode
{
	enum Mode
	{
		SIDE_BY_SIDE,
        CROSS_EYED,
		LINE_BY_LINE,
		SUTTER,
		ANAGLYPH
	};
};
*/


class ofxCoolGlasses {
public:
    
    /*
	ofxStereoCamera() : physical_eye_seperation_cm(6.5), physical_focus_distance_cm(200), focus_distance(200), view(ofxStereoCameraViewMode::PARALLEL), output(ofxStereoCameraOutputMode::ANAGLYPH), OF_CAMERA_CLASS() {}
    */
    
    ofCamera left;
    ofCamera right;
    
    ofxCoolGlasses(){};
    ~ofxCoolGlasses(){};
    
	void setup(int w, int h)
	{
		width = w;
		height = h;
        
		//left_fbo.allocate(width, height, GL_RGBA);
		//right_fbo.allocate(width, height, GL_RGBA);
        
        left.setScale(1, -1, 1);
        left.setNearClip(0.1);
        
        right.setScale(1, -1, 1);
        right.setNearClip(0.1);

	}
    
    void setPosition(ofVec3f pos) {
        left.setPosition(pos);
        right.setPosition(pos);
    }
    

	void update(ofRectangle viewport = ofGetCurrentViewport())
	{
        
//		OF_CAMERA_CLASS::begin();
//		OF_CAMERA_CLASS::end();

		eye = physical_eye_seperation_cm * 0.5 / physical_focus_distance_cm;
        
		/*aspect = viewport.width / viewport.height;
		fovy_tan = tanf(PI * left.getFov() / 360.);
		fovx_tan = fovy_tan * aspect;
         */
        
		zNear = left.getNearClip();
		zFar  = left.getFarClip();
        
        right.setPosition(left.getPosition().x + eye, left.getPosition().y, left.getPosition().z);
        
        right.setupOffAxisViewPortal(viewport.getTopLeft(), viewport.getBottomLeft(), viewport.getBottomRight());
        left.setupOffAxisViewPortal(viewport.getTopLeft(), viewport.getBottomLeft(), viewport.getBottomRight());

	}

	/*void draw(int x = 0, int y = 0)
	{
		//draw(x, y, width, height);
        
	}*/

	/*void draw(int x, int y, int w, int h)
	{
		ofPushStyle();
		glPushMatrix();
		
		ofSetColor(255, 255);

		glTranslatef(x, y, 0);

		if (output == ofxStereoCameraOutputMode::SIDE_BY_SIDE)
		{
			right_fbo.draw(0, 0, w / 2, h);
			left_fbo.draw(0 + w / 2, 0, w / 2, h);
		}

		glPopMatrix();
		ofPopStyle();
	}*/

	//

	void setPhysicalEyeSeparation(float cm) { physical_eye_seperation_cm = cm; }
	void setPhysicalFocusDistance(float cm) { physical_focus_distance_cm = cm; }
	void setFocusDistance(float v) { focus_distance = v; }

	/*void setViewMode(ofxStereoCameraViewMode::Mode m) { view = m; }*/
	/*void setOutputMode(ofxStereoCameraOutputMode::Mode m) { output = m; }*/

	//

	void beginLeft()
	{
        
        left.begin();
        
      //  glTranslated(width*.5, height*.5, 0);

      //  glScaled(height,height,height);
        
		//left_fbo.begin();
		//ofFloatColor c = ofGetCurrentRenderer()->getBgColor();
		//ofClear(c);

		//ofPushView();
		//glPushMatrix();

		//glMatrixMode(GL_PROJECTION);
		//glLoadIdentity();

        //glFrustum(-zNear * (fovx_tan + eye), zNear * (fovx_tan - eye), -zNear * fovy_tan, zNear * fovy_tan, zNear, zFar);
        //glTranslated(-eye * focus_distance, 0, 0);

		//glMatrixMode(GL_MODELVIEW);
		//glLoadIdentity();
		//glMultMatrixf(left.getModelViewMatrix().getPtr());
        
	}

	void endLeft()
	{
		/*glPopMatrix();
		ofPopView();

		left_fbo.end();*/
        left.end();
	}

	void beginRight()
	{
        
        right.begin();
		/*right_fbo.begin();

		ofFloatColor c = ofGetCurrentRenderer()->getBgColor();
		ofClear(c);

		ofPushView();
		glPushMatrix();

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		if (view == ofxStereoCameraViewMode::PARALLEL)
		{
			glFrustum(-zNear * (fovx_tan - eye), zNear * (fovx_tan + eye), -zNear * fovy_tan, zNear * fovy_tan, zNear, zFar);
			glTranslated(eye * focus_distance, 0, 0);
		}
		else if (view == ofxStereoCameraViewMode::TOE_IN)
		{
			glMultMatrixf(this->getProjectionMatrix().getPtr());
			glRotatef(rot, 0, 1, 0);
			glTranslated(eye * focus_distance, 0, 0);
		}

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glMultMatrixf(this->getModelViewMatrix().getPtr());*/
	}

	void endRight()
	{
		/*glPopMatrix();
		ofPopView();

		right_fbo.end();*/
        
        right.end();
	}
	
    
	/*ofTexture& getLeftTexture()
	{
		//return left_fbo.getTextureReference();
	}
	
	ofTexture& getRightTexture()
	{
		return right_fbo.getTextureReference();
	}*/

	/*ofFbo * getLeftFbo()
	{
		return &left_fbo;
	}
	
	ofFbo * getRightFbo()
	{
		return &right_fbo;
	}*/
    
protected:

/*	ofxStereoCameraViewMode::Mode view;
	ofxStereoCameraOutputMode::Mode output;
*/
    
	int width, height;

	float eye, focus_distance;

	float physical_eye_seperation_cm;
	float physical_focus_distance_cm;

	//ofFbo left_fbo, right_fbo;

	// toe-in

	float rot;

	// parallel

	float zNear, zFar;
	float fovx_tan, fovy_tan;
	float aspect;
	
	// sutter
	//int frame_count;

private:

    /*
	void begin() {}
	void end() {}
     */

};