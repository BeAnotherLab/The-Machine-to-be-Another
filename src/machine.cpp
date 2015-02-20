#include "machine.h"

void machine::setup(int s, int c)
{
	initOculus();
	vidGrabberLeft.listDevices();
	setup_type = s;
	camera_type = c;
	//tested with PS3Eye camera.	
	x_offset = 0;
	camWidth = 640;
	camHeight = 480;		
	
	if (c == MONO) {
		vidGrabberLeft.setVerbose(true);
		vidGrabberLeft.setDeviceID(0);
		vidGrabberLeft.setDesiredFrameRate(120);
		vidGrabberLeft.initGrabber(camWidth,camHeight);							
	} else if (c == STEREO) {		
	    vidGrabberRight.setVerbose(true);
		vidGrabberRight.setDeviceID(2);
		vidGrabberRight.setDesiredFrameRate(120);
		vidGrabberRight.initGrabber(camWidth,camHeight);	
	} else if (c == OVRVISION) {
		g_pOvrvision = new OVR::Ovrvision();
	    g_pOvrvision->Open(0,OVR::OV_CAMVGA_FULL);   
	    left.allocate(camWidth,camHeight,GL_RGB);
	    right.allocate(camWidth,camHeight,GL_RGB);
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

	fboLeft.allocate(camWidth,camHeight);
	fboLeft.setAnchorPercent(0.5, 0.5);
	fboRight.allocate(camWidth,camHeight);
	fboRight.setAnchorPercent(0.5, 0.5);

	//was used for experimenting with torchlight-like overlay, left here as ref for later
	overlay.loadImage("pictures/overlay4.png");
	overlay.resize(2000*1.25,2000);
	overlay.setAnchorPercent(0.5, 0.5);   

	dimTimer = ofGetElapsedTimeMillis();
	dimmed = false;		
}

//--------------------------------------------------------------
void machine::initOculus() {	
	ovr_Initialize();	
	hmd = ovrHmd_Create(0);	
	if (!hmd || !ovrHmd_ConfigureTracking(hmd, ovrTrackingCap_Orientation, 0)) {
		cout << "Unable to detect Rift head tracker" << endl;		
	}
	ovrHmd_RecenterPose(hmd);
}

void machine::update() {	
	ovrTrackingState state = ovrHmd_GetTrackingState(hmd, 0);
	Quatf pose = state.HeadPose.ThePose.Orientation;
	pose.GetEulerAngles<Axis_Y, Axis_X, Axis_Z>(&yaw, &pitch, &roll); //rotation order affects gimbal lock.
	
	//ofMap(yaw, -180, 0, 180, 360);
	if (camera_type == MONO) {
		vidGrabberLeft.update();
	}else if (camera_type == STEREO) {
		vidGrabberRight.update();
	} else if (camera_type == OVRVISION) {
		//left.loadData(g_pOvrvision->GetCamImage(OVR::OV_CAMEYE_LEFT), 640,480, GL_RGB);
		g_pOvrvision->PreStoreCamData();
		right.loadData(g_pOvrvision->GetCamImage(OVR::OV_CAMEYE_LEFT, OVR::OV_PSQT_HIGH), 640,480, GL_RGB);
		//right.loadData(g_pOvrvision->GetCamImage(OVR::OV_CAMEYE_RIGHT), 640,480, GL_RGB);	
		left.loadData(g_pOvrvision->GetCamImage(OVR::OV_CAMEYE_RIGHT, OVR::OV_PSQT_HIGH), 640,480, GL_RGB);	
	}

	ofVec2f distance = getDistance();	
	
	fboLeft.begin();					
		ofBackground(0);
		ofPushMatrix();			
			ofTranslate(camWidth/2, camHeight/2);
			ofRotate(0, 0, 0, 1); //rotate from centre						
				//vidGrabberLeft.draw(-x_offset-camWidth/2+distance.x*250, -camHeight/2 -distance.y*250);				
				if (camera_type == OVRVISION) {
					left.draw(-camWidth/2+distance.y*250, -camHeight/2 -distance.x*250);
				} else {
					vidGrabberLeft.draw(-camWidth/2+distance.y*250, -camHeight/2 -distance.x*250);				
				}
			//overlay.draw(distance.x*500,  -240-distance.y*500);
		ofPopMatrix();							
	//dim();
	fboLeft.end();	
			
		fboRight.begin();						
			ofBackground(0);
			ofPushMatrix();			
				ofTranslate(camWidth/2, camHeight/2);
				ofRotate(0, 0, 0, 1); //rotate from centre		
					if (camera_type == STEREO) {			
						//vidGrabberRight.draw(x_offset-camWidth/2+distance.x*250, -camHeight/2 -distance.y*250);	
						vidGrabberRight.draw(-camWidth/2+distance.y*250, -camHeight/2 -distance.x*250);	
					} else if (camera_type == OVRVISION) {
						right.draw(-camWidth/2+distance.y*250, -camHeight/2 -distance.x*250);	
					} else {
						//vidGrabberLeft.draw(x_offset-camWidth/2+distance.x*250, -camHeight/2 -distance.y*250);	
						vidGrabberLeft.draw(-camWidth/2+distance.y*250, -camHeight/2 -distance.x*250);	
					}
			ofPopMatrix();
			ofSetColor(0);
			//dim();
			ofSetColor(255);						
		fboRight.end();	
	
}

ofVec2f machine::getDistance() {
	if (setup_type == TWO_WAY_SWAP) {			
		ofVec2f self = ofVec2f(pitch-pitch_cal, yaw-yaw_cal);
		ofVec2f other = ofVec2f(rx_pitch, rx_yaw);		
		return other - self;
	}
	else if (setup_type == ONE_WAY_SWAP) {
		return ofVec2f(0,0);
	}	
}

void machine::drawVideo() {			
	if (camera_type == MONO) { //drawing the videograbber once in each fbo doesn't work, drawing the left fbo twice
		fboLeft.draw(-x_offset + ofGetWidth()/4, ofGetHeight()/2); //draw left	
		fboLeft.draw(x_offset + 3*ofGetWidth()/4, ofGetHeight()/2); //draw right
	} else {
	 cout << ofGetMouseY()*0.15 << endl;
		 fboLeft.draw(-x_offset+ofGetWidth()/4, ofGetHeight()/2+ofGetMouseX()*0.5-500); //draw left. 29.1 is to adjust for slight cameras disalignment
		 fboRight.draw(x_offset+3*ofGetWidth()/4, ofGetHeight()/2); //draw right	
	}		
}

void machine::debug() {
	ofDrawBitmapString("pitch : " + ofToString(ofRadToDeg(pitch)), 10,10);
	ofDrawBitmapString("yaw   : " + ofToString(ofRadToDeg(yaw)), 10,25);
	ofDrawBitmapString("roll  : " + ofToString(ofRadToDeg(roll)), 10,40);
	
	ofDrawBitmapString("yaw calibration value : " + ofToString(ofRadToDeg(yaw_cal)), 180,10);
	ofDrawBitmapString("yaw calibrated value  : " + ofToString(ofRadToDeg(yaw - yaw_cal)), 180,25);
}

void machine::drawOverlay() {
		
}

void machine::dim() {
	int timeDim = ofGetElapsedTimeMillis() - dimTimer;
	ofSetColor(0);
	if (timeDim < 2000) {//if dim/undim was triggered less than 2 seconds ago
		if (dimmed == true) { //if we must dim the lights
			ofSetColor(0,ofMap(timeDim,0,2000,0,255));
			ofRect(0,0,ofGetWidth(),ofGetHeight());
		}
		else { //if we must turn the lights back on;
			ofSetColor(0,ofMap(timeDim,0,2000,255,0));
			ofRect(0,0,ofGetWidth(), ofGetHeight());
		}
	}
	else if (dimmed == true) { // stay dark
		ofRect(0,0,ofGetWidth(), ofGetHeight());
	}	
}

void machine::triggerDim() {
	dimTimer = ofGetElapsedTimeMillis();
	dimmed = !dimmed;	
}

void machine::calibrate() {	
//	yaw_cal=ofMap(yaw-, -180, 180, 0, 360);
	ovrHmd_RecenterPose(hmd);
	//pitch_cal = pitch;
	//yaw_cal = yaw;
	//roll_cal = roll;	
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

/*
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
*/
