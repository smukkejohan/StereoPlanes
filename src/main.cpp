#include "ofMain.h"
#include "testApp.h"
#include "ofGlProgrammableRenderer.h"

//========================================================================
int main( ){    
    
    ofSetCurrentRenderer(ofGLProgrammableRenderer::TYPE);
    ofSetupOpenGL(1280, 800, OF_WINDOW);
    
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new testApp());
    
    

}
