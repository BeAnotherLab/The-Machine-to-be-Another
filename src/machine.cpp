#include "machine.h"

void machine::setup(int t, string h, int p)
{
	type = t;

	//tested with PS3Eye camera.	
	x_offset = 256;
	camWidth = 640;
	camHeight = 480;		
	vidGrabber.setVerbose(true);
	vidGrabber.setDeviceID(0);
	vidGrabber.setDesiredFrameRate(120);
	vidGrabber.initGrabber(camWidth,camHeight);			
	
	pitch = 0;
	yaw = 0;
	roll = 0;
	pitch_cal = 0;
	yaw_cal = 0;
	roll_cal = 0;

	sender.setup(h, p);    
	receiver.setup(p);    	

	fbo.allocate(480,640);
	fbo.setAnchorPercent(0.5, 0.5);

	//was used for experimenting with torchlight-like overlay, left here as ref for later
	overlay.loadImage("pictures/overlay4.png");
	overlay.resize(2000*1.25,2000);
	overlay.setAnchorPercent(0.5, 0.5);
    //These are the parameters for the polynomial warp function to correct for the Oculus Rift and Webcam Lenses. Proper values still to be found
    //kept as ref, but need to be properly calibrated according to camera and lens used.
	K0 = 1.0;
    K1 = 5.74;
    K2 = 0.27;
    K3 = 0.0;
    _x = 0.0f;
    _y = 0.0f;
    _w = 1.0f;
    _h = 1.0f;
    as = 640.0f/480.0f;
	DistortionXCenterOffset = 90;	        
    hmdWarpShader.load("shaders/HmdWarpExp");

	dimTimer = ofGetElapsedTimeMillis();
	dimmed= false;
}

void machine::update() {
	vidGrabber.update();
	ofVec2f distance = getDistance();	

	fbo.begin();			
		ofBackground(0);
		ofPushMatrix();			
			ofTranslate(camHeight/2, camWidth/2);
			ofRotate(270, 0, 0, 1); //rotate from centre					
			vidGrabber.draw(-320+distance.x*250, -240 -distance.y*250);
			//overlay.draw(distance.x*500,  -240-distance.y*500);
		ofPopMatrix();
		int timeDim = ofGetElapsedTimeMillis() - dimTimer;
		ofSetColor(0);
		dim();
	fbo.end();	
}

ofVec2f machine::getDistance() {
	if (type==TWO_WAY_SWAP) {			
		ofVec2f mine = ofVec2f(pitch-pitch_cal,yaw-yaw_cal);
		ofVec2f their = ofVec2f(rx_pitch,rx_yaw);		
		return their - mine;
	}
	else if (type==ONE_WAY_SWAP) {
		return ofVec2f(0,0);
	}	
}

void machine::drawVideo() {		
	fbo.draw(-x_offset + ofGetWidth()/2, ofGetHeight()/2); //draw left
	fbo.draw(x_offset + ofGetWidth()/2, ofGetHeight()/2); //draw right
}

void machine::drawOverlay() {
		
}

void machine::oscControl() {
	//receive headtracking and touchOSC messages
	ofxOscMessage rx_msg;
	while (receiver.hasWaitingMessages()) {
		if (rx_msg.getAddress() == "/dim") {			
			if (rx_msg.getArgAsFloat(0) == 1.0f) triggerDim();			
			#if COMPUTER == 1
				sender.sendMessage(rx_msg);
			#endif
		}		
		else if (rx_msg.getAddress() == "/ht") {
			calibrate();
			#if COMPUTER == 1
				sender.sendMessage(rx_msg);
			#endif
		}		
	}		
}

void machine::sendHeadTracking(){
	//send calibrated angles value over OSC.
	ofxOscMessage m;
	m.setAddress("/ori");	
	m.addFloatArg(roll-roll_cal);
	m.addFloatArg(pitch-pitch_cal);
	m.addFloatArg(yaw-yaw_cal);	
	sender.sendMessage(m);     	    
}

void machine::receiveHeadTracking() {
	ofxOscMessage rx_msg;
	receiver.getNextMessage(&rx_msg);				
    if (rx_msg.getAddress() == "/ori") {
		rx_roll = rx_msg.getArgAsFloat(0);
		rx_pitch = rx_msg.getArgAsFloat(1);
 		rx_yaw = rx_msg.getArgAsFloat(2);                  
	}	
}


void machine::dim() {
	int timeDim = ofGetElapsedTimeMillis() - dimTimer;
	ofSetColor(0);
	if (timeDim < 2000) { //if dim/undim was triggered less than 2 seconds ago
		if (dimmed == true) { //if we must dim the lights
			ofSetColor(0,ofMap(timeDim,0,2000,0,255));
			ofRect(0,0,ofGetWidth(),ofGetHeight());
		}
		else { //if we must turn the lights back on;
			ofSetColor(0,ofMap(timeDim,0,2000,255,0));
			ofRect(0,0,ofGetWidth(),ofGetHeight());
		}
	}
	else if (dimmed == true) { // stay dark
		ofRect(0,0,ofGetWidth(),ofGetHeight());
	}	
}

void machine::triggerDim() {
	dimTimer = ofGetElapsedTimeMillis();
	dimmed = !dimmed;	
}

void machine::calibrate() {	
		pitch_cal = pitch;
		yaw_cal = yaw;
		roll_cal = roll;	
}

machine::machine(void)
{

}

machine::~machine(void)
{

}

/*hmdWarpShader.begin();
	hmdWarpShader.setUniformTexture("tex", vidGrabber.getTextureReference(), 0);
    hmdWarpShader.setUniform2f("LensCenter", DistortionXCenterOffset, 0 );
    hmdWarpShader.setUniform2f("ScreenCenter", _x + _w*1.0f, _y + _h*1.0f );
    hmdWarpShader.setUniform2f("Scale", (_w/1.0f) * 1.0f, (_h/1.0f) * 1.0f * as );
    hmdWarpShader.setUniform2f("ScaleIn", (1.0f/_w), (1.0f/_h) / as );
    hmdWarpShader.setUniform4f("HmdWarpParam", K0, K1, K2, K3 );*/
	
	//hmdWarpShader.end();

		/*
    float planeScale = 0.75;
    int planeWidth = ofGetWidth() * planeScale;
    int planeHeight = ofGetHeight() * planeScale;
    int planeGridSize = 20;
    int planeColumns = planeWidth / planeGridSize;
    int planeRows = planeHeight / planeGridSize;
 */
    //ofPlanePrimitive plane = *(new ofPlanePrimitive());		
	//plane.set(planeWidth, planeHeight, planeColumns, planeRows, OF_PRIMITIVE_TRIANGLES);
	//plane.mapTexCoordsFromTexture(vidGrabber.getTextureReference);		
	//ofRotate(90,0,0,1);
	
	
	/*
	glBegin(GL_QUADS);
    glTexCoord2f(1,0); glVertex3f(0,0,0);
    glTexCoord2f(0,0); glVertex3f(640,0,0);
    glTexCoord2f(0,1); glVertex3f(640,800,0);
    glTexCoord2f(1,1); glVertex3f(0,800,0);
    glEnd();*/	
