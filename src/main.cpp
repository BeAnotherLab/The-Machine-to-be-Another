#include "testApp.h"
#include "ofAppGlutWindow.h"

//========================================================================
int main(){

    ofAppGlutWindow window1;	
	ofSetupOpenGL(&window1, 1920,1080, OF_WINDOW);			// <-------- setup the GL context	

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());

}
