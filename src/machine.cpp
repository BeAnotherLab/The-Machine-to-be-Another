#include "machine.h"

void machine::setup(ofxXmlSettings * se)
{
	ovr_Initialize();		
	hmd = ovrHmd_Create(0);	
	if (!hmd || !ovrHmd_ConfigureTracking(hmd, ovrTrackingCap_Orientation, 0)) {
		cout << "Unable to detect Rift head tracker" << endl;		
	}

	ovrHmd_RecenterPose(hmd); 
	vidGrabberLeft.listDevices();
	settings = se;
	setup_type = settings->getValue("settings:setup_type", ONE_WAY_SWAP);
	camera_type = settings->getValue("settings:camera_type", MONO);
	swapLR = settings->getValue("settings:swapLR", 0);
	//tested with PS3Eye camera.	
	ipd = settings->getValue("settings:ipd", 8);
	camWidth = 640;
	camHeight = 480;		
	
	if (camera_type == MONO) {
		vidGrabberLeft.setVerbose(true);
        vidGrabberLeft.setDeviceID(settings->getValue("settings:camera_id", 1));							
		vidGrabberLeft.setDesiredFrameRate(120);
		vidGrabberLeft.initGrabber(camWidth,camHeight);			
		vidGrabberLeft.setAnchorPercent(0.5,0.5);
	} else if (camera_type == STEREO) {		
	    vidGrabberRight.setVerbose(true);
		vidGrabberRight.setDeviceID(settings->getValue("settings:camera_id", 2));
		vidGrabberRight.setDesiredFrameRate(120);
		vidGrabberRight.initGrabber(camWidth,camHeight);	
		vidGrabberRight.setAnchorPercent(0.5,0.5);
	} else if (camera_type == OVRVISION) {
		g_pOvrvision = new OVR::Ovrvision();
	    g_pOvrvision->Open(0,OVR::OV_CAMVGA_FULL);   
	    left.allocate(camWidth,camHeight,GL_RGB);
		left.setAnchorPercent(0.5,0.5);
	    right.allocate(camWidth,camHeight,GL_RGB);
		right.setAnchorPercent(0.5,0.5);
	}

	pitch = 0;
	yaw = 0;
	roll = 0;
	rx_pitch = 0;
	rx_yaw = 0;
	rx_roll = 0;
	pitch_cal = 0;
	yaw_cal = 0;
	roll_cal = 0;

	//These are the parameters for the polynomial warp function to correct for the Oculus Rift and Webcam Lenses. Proper values still to be found
    //kept as ref, but they need to be properly calibrated according to camera and lens used.
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

	if (hmd->Type == ovrHmd_DK2) { 
		fboLeft.allocate(DK2_WIDTH/2, DK2_HEIGHT);
		fboRight.allocate(DK2_WIDTH/2, DK2_HEIGHT);
	}
	else if(hmd->Type == ovrHmd_DK1) {
		fboLeft.allocate(DK1_WIDTH/2, DK1_HEIGHT);
		fboRight.allocate(DK1_WIDTH/2, DK1_HEIGHT);
	}
	fboLeft.setAnchorPercent(0.5, 0.5);
	fboRight.setAnchorPercent(0.5, 0.5);

	//was used for experimenting with torchlight-like overlay, left here as ref for later
	overlay.loadImage("pictures/overlay4.png");
	overlay.resize(2000*1.25,2000);
	overlay.setAnchorPercent(0.5, 0.5);   

	zoom = settings->getValue("settings:zoom", 0.85);
	speed = settings->getValue("settings:speed", 920);
	alignment = settings->getValue("settings:alignment", 0);
	ipd = settings->getValue("settings:ipd", 8);	

	dimTimer = ofGetElapsedTimeMillis();
	dimmed = false;		
}

void machine::update() {

	ovrTrackingState state = ovrHmd_GetTrackingState(hmd, 0);
	Quatf pose = state.HeadPose.ThePose.Orientation;
	pose.GetEulerAngles<Axis_Y, Axis_Z, Axis_X>(&yaw, &roll, &pitch); //rotation order affects gimbal lock.

	if (camera_type == MONO) {
		vidGrabberLeft.update();
	} else if (camera_type == STEREO) {
		vidGrabberRight.update();
	} else if (camera_type == OVRVISION) {		
		g_pOvrvision->PreStoreCamData();
		right.loadData(g_pOvrvision->GetCamImage(OVR::OV_CAMEYE_LEFT, OVR::OV_PSQT_NONE), 640, 480, GL_RGB);		
		left.loadData(g_pOvrvision->GetCamImage(OVR::OV_CAMEYE_RIGHT, OVR::OV_PSQT_NONE), 640, 480, GL_RGB);	
	}
	
	ofVec2f distance = getDistance();	
	ofSetColor(255);

	fboLeft.begin();					
		ofBackground(0);				
		ofPushMatrix();			
			ofTranslate(fboLeft.getWidth()/2, fboLeft.getHeight()/2); //move to fbo center					
			ofRotate(ofRadToDeg(roll-rx_roll), 0, 0, 1); //rotate from centre
				if (camera_type == OVRVISION) {										
					ofPushMatrix();
						ofRotate(180, 0, 0, 1);
						left.draw(- distance.y*speed, - distance.x*speed + alignment, camWidth*zoom, camHeight*zoom);					
					ofPopMatrix();					
				} else {
					ofPushMatrix();
						ofRotate(90);
						vidGrabberLeft.draw( -(-distance.x*speed), -distance.y*speed, camWidth*zoom, camHeight*zoom);	
					//vidGrabberLeft.draw(distance.y*speed, distance.x*speed, camWidth*zoom, camHeight*zoom);
					ofPopMatrix();
				}				
		ofPopMatrix();										
	fboLeft.end();	
			
	fboRight.begin();						
		ofBackground(0);
		ofPushMatrix();			
			ofTranslate(fboRight.getWidth()/2, fboRight.getHeight()/2); //move to fbo center	
			ofRotate(ofRadToDeg(roll-rx_roll), 0, 0, 1);//rotate from centre				
				if (camera_type == STEREO) {				
					vidGrabberRight.draw(distance.y*speed, - distance.x*speed - alignment, camWidth*zoom, camHeight*zoom);	
				} else if (camera_type == OVRVISION) {		
					ofPushMatrix();
						ofRotate(180, 0, 0, 1);
						right.draw(- distance.y*speed, - distance.x*speed - alignment, camWidth*zoom, camHeight*zoom);	
					ofPopMatrix();					
				} else { // mono
					ofPushMatrix();
						ofRotate(90);
						vidGrabberLeft.draw(-(-distance.x*speed), -distance.y*speed, camWidth*zoom, camHeight*zoom);	
					//vidGrabberLeft.draw(distance.y*speed, distance.x*speed, camWidth*zoom, camHeight*zoom);
					ofPopMatrix();
				}
		ofPopMatrix();			
	fboRight.end();	
}

ofVec2f machine::getDistance() {
	if (setup_type == TWO_WAY_SWAP) {			
		ofVec2f self = ofVec2f(pitch, yaw);
		ofVec2f other = ofVec2f(rx_pitch, rx_yaw);		
		return self - other;
	}
	else if (setup_type == ONE_WAY_SWAP) {
		return ofVec2f(0,0);
	}	
}

void machine::drawVideo() {				
	if (camera_type == MONO) { //drawing the videograbber once in each fbo doesn't work, drawing the left fbo twice instead
		fboLeft.draw(-ipd + ofGetWidth()/4, ofGetHeight()/2); //draw left	
		fboLeft.draw(ipd + 3*ofGetWidth()/4, ofGetHeight()/2); //draw right
	} else {
	     if(swapLR == 0) {
			 fboLeft.draw(ofGetWidth()/4, ofGetHeight()/2); //draw left.		 
			 fboRight.draw(3*ofGetWidth()/4, ofGetHeight()/2); //draw right	
		 }
		  if(swapLR == 1) {
			 fboRight.draw(ofGetWidth()/4, ofGetHeight()/2); //draw right to the left.		 
			 fboLeft.draw(3*ofGetWidth()/4, ofGetHeight()/2); //draw left to the right	
		 }
	}		
	ofSetColor(255);
	//dim();
 	if (dimmed) {
		ofSetColor(0);
		ofRect(0,0,ofGetWidth(), ofGetHeight());
	}
	ofSetColor(255);
}

void machine::drawMonitor() {	
	if (dimmed==true){
		ofSetColor(75);
		fboLeft.draw(ofGetWidth()/2, ofGetHeight()/2);	
	} else {
		fboLeft.draw(ofGetWidth()/2, ofGetHeight()/2);	
	}
	ofSetColor(255);
}

void machine::debug() {	
	
	string framerate = ofToString(ofGetFrameRate());		
	ofDrawBitmapString("FPS : " + framerate, 10, 10);
	ofDrawBitmapString("pitch : " + ofToString(ofRadToDeg(pitch)), 10,20); //10
	ofDrawBitmapString("yaw   : " + ofToString(ofRadToDeg(yaw)), 10,30);
	ofDrawBitmapString("roll  : " + ofToString(ofRadToDeg(roll)), 10,40);

	ofDrawBitmapString("zoom : " + ofToString(zoom), 10, 50);
	ofDrawBitmapString("speed   : " + ofToString(speed), 10, 60);
	ofDrawBitmapString("alignment  : " + ofToString(alignment), 10, 70);		
	ofDrawBitmapString("side : " + ofToString(ipd), 10, 80);	

	ofDrawBitmapString("distance.x : " + ofToString(getDistance().x), 10, 90);	
	ofDrawBitmapString("distance.y : " + ofToString(getDistance().y), 10, 100);	
	
	ofDrawBitmapString("swap L/R: " + ofToString(swapLR), 10, 110);
	if (dimmed == true) {
		ofDrawBitmapString("oculus screen is OFF", 10, 120); 
	} else if (dimmed == false) {
		ofDrawBitmapString("oculus screen is ON", 10, 120); 
	}
	
	//ofDrawBitmapString("tracking caps " + ofToString(hmd->TrackingCaps), 10, 120);
	//ofDrawBitmapString("yaw drift correction : " + ofToString(hmd->TrackingCaps && 0x001), 10, 130);
	//ofDrawBitmapString("yaw drift correction : " + ofToString(hmd->TrackingCaps % 2), 10, 140);

}

void machine::drawOverlay() {
		
}

void machine::dim() {
	int timeDim = ofGetElapsedTimeMillis() - dimTimer;
	//ofDrawBitmapString("alpha  : " + ofToString(ofMap(timeDim,0,2000,0,255)), 10,100);	
	ofSetColor(0);
	if (timeDim < 2000) {//if dim/undim was triggered less than 2 seconds ago
		if (dimmed == true) { //if we must dim the lights			
			ofSetColor(0,ofMap(timeDim,0,2000,0,255));			
			ofRect(0,0,ofGetWidth(),ofGetHeight());
			ofCircle(ofGetMouseX(),ofGetMouseY(),50);
		}
		else { //if we must turn the lights back on;
			ofSetColor(255);
			ofDrawBitmapString("alpha  : " + ofToString(ofMap(timeDim,0,2000,255,0)), 10,100);	
			ofSetColor(0,ofMap(timeDim,0,2000,255,0));			
			ofRect(0,0,ofGetWidth(), ofGetHeight());
			ofCircle(ofGetMouseX(),ofGetMouseY(),50);
		}
	}
	else if (dimmed == true) { // stay dark
		ofRect(0,0,ofGetWidth(), ofGetHeight());
		ofCircle(ofGetMouseX(),ofGetMouseY(),50);
	}		
	ofSetColor(255);
//	ofCircle(ofGetMouseX(),ofGetMouseY(),50);
}

void machine::triggerDim() {
	dimTimer = ofGetElapsedTimeMillis();
	dimmed = !dimmed;	
}

void machine::calibrate() {	
	ovrHmd_RecenterPose(hmd);
	//pitch_cal = pitch;
}

float* machine::getHeadtracking(){
	float* c;
	c = new float[3];
	c[0] = roll;
	c[1] = pitch;
	c[2] = yaw;
	return c;
}

void machine::clear() {
	/*pSensor.Clear();
    pHMD.Clear();
	pManager.Clear();  	
	System::Destroy();	*/
}

machine::machine(void)
{

}

machine::~machine(void)
{
	
}

/*
	// Configure Stereo settings.
	Sizei recommenedTex0Size = ovrHmd_GetFovTextureSize(hmd, ovrEye_Left,
	hmd->DefaultEyeFov[0], 1.0f);
	Sizei recommenedTex1Size = ovrHmd_GetFovTextureSize(hmd, ovrEye_Right,
	hmd->DefaultEyeFov[1], 1.0f);
	Sizei renderTargetSize;
	renderTargetSize.w = recommenedTex0Size.w + recommenedTex1Size.w;
	renderTargetSize.h = max ( recommenedTex0Size.h, recommenedTex1Size.h );
	const int eyeRenderMultisample = 1;
	pRendertargetTexture = pRender->CreateTexture(texture_RGBA | Texture_RenderTarget | eyeRenderMultisample,renderTargetSize.w, renderTargetSize.h, NULL);
	// The actual RT size may be different due to HW limits.
	renderTargetSize.w = pRendertargetTexture->GetWidth();
	renderTargetSize.h = pRendertargetTexture->GetHeight();
	*/


/*
	// 1st, draw on screen:
	ofSetHexColor(0x66CC33);	
	ofDrawRectangle(100,100,300,300);
	
	ofSetHexColor(0xffffff);
	ofPushMatrix();
		ofTranslate(200,200,0);
		ofRotate(counter,0,0,1);
		ofDrawCircle(0,0,80);
		ofDrawCircle(100,0,10);	// a small one
	ofPopMatrix();
	ofSetHexColor(0x333333);
	ofDrawBitmapString("(a) on screen", 150,200);

	ofSetHexColor(0xFFCC33);	
	ofDrawCircle(mouseX, mouseY,20);
	

	// 2nd, grab a portion of the screen into a texture
	// this is quicker then grabbing into an ofImage
	// because the transfer is done in the graphics card
	// as opposed to bringing pixels back to memory
	// note: you need to allocate the texture to the right size
	texScreen.loadScreenData(100,100,300,300);
	
	

	// finally, draw that texture on screen, how ever you want
	// (note: you can even draw the texture before you call loadScreenData, 
	// in order to make some trails or feedback type effects)
	ofPushMatrix();
		ofSetHexColor(0xffffff);
		ofTranslate(550,300,0);
		//glRotatef(counter, 0.1f, 0.03f, 0);
		float width = 200 + 100 * sin(counter/200.0f);
		float height = 200 + 100 * sin(counter/200.0f);;
		texScreen.draw(-width/2,-height/2,width,height);
	ofPopMatrix();

	ofPushMatrix();
		ofSetHexColor(0xffffff);
		ofTranslate(700,210,0);
		ofRotate(counter, 0.1f, 0.03f, 0);
		texScreen.draw(-50,-50,100,100);
	ofPopMatrix();

	ofSetHexColor(0x333333);
	ofDrawBitmapString("(b) in a texture, very meta!", 500,200);

*/

/*
void machine::distortion(){


	hmdWarpShader.begin();
		hmdWarpShader.setUniformTexture("tex", vidGrabber.getTextureReference(), 0);
		hmdWarpShader.setUniform2f("LensCenter", DistortionXCenterOffset, 0 );
		hmdWarpShader.setUniform2f("ScreenCenter", _x + _w*1.0f, _y + _h*1.0f );
		hmdWarpShader.setUniform2f("Scale", (_w/1.0f) * 1.0f, (_h/1.0f) * 1.0f * as );
		hmdWarpShader.setUniform2f("ScaleIn", (1.0f/_w), (1.0f/_h) / as );
		hmdWarpShader.setUniform4f("HmdWarpParam", K0, K1, K2, K3 );
	hmdWarpShader.end();
		
    float planeScale = 0.75;
    int planeWidth = ofGetWidth() * planeScale;
    int planeHeight = ofGetHeight() * planeScale;
    int planeGridSize = 20;
    int planeColumns = planeWidth / planeGridSize;
    int planeRows = planeHeight / planeGridSize;
 
    //ofPlanePrimitive plane = *(new ofPlanePrimitive());		
	//plane.set(planeWidth, planeHeight, planeColumns, planeRows, OF_PRIMITIVE_TRIANGLES);
	//plane.mapTexCoordsFromTexture(vidGrabber.getTextureReference);		
	//ofRotate(90,0,0,1);
			
	glBegin(GL_QUADS);
    glTexCoord2f(1,0); glVertex3f(0,0,0);
    glTexCoord2f(0,0); glVertex3f(640,0,0);
    glTexCoord2f(0,1); glVertex3f(640,800,0);
    glTexCoord2f(1,1); glVertex3f(0,800,0);
    glEnd();
}
*/