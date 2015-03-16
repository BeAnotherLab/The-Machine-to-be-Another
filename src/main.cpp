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


//PARA EL COMPU QUE TIENE CONECTADA LA PS3

//LEER LINEA 19 MACHINE.CPP
//LEER LINEA 122 Y 144 EN MACHINE.CPP
//LEER LA LINEA 168 EN MACHINE.CPP
//LEER LA LINEA 13 EN TESTAPP.CPP

