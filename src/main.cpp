#include "ofMain.h"
#include "testApp.h"
#include <ofAppGlutWindow.h>

//========================================================================
int main( ){    
    
    ofAppGlutWindow window;
//    window.setDoubleBuffering(false);
    window.setGlutDisplayString("rgb alpha double samples>=8 depth");
    
    ofSetupOpenGL(&window, 1024*2, 600, OF_WINDOW);
    
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new testApp());
    
    

}
