#pragma once
#include "ofxOsc.h"
#include "OVR.h"

#define ONE_WAY_SWAP 0
#define TWO_WAY_SWAP 1
#define MONO 2
#define STEREO 3

using namespace OVR;
using namespace std;

class machine
{
public:			
	ovrHmd hmd;
	int setup_type; //0 = one-way swap, 1 = two-way swap
	int camera_type; //2 for mono, 3 for stereo camera
	int camWidth, camHeight; //camera size
	int x_offset, y_offset;	//used to adjust display position and separation
	float pitch, yaw, roll; //headtracking received from oculus		
    float pitch_cal, yaw_cal, roll_cal; //used to calibrate headtracking values
	float rx_pitch, rx_yaw, rx_roll; //received headtracking
            
    ofVideoGrabber vidGrabberLeft, vidGrabberRight; //for using with webcam or PS3 camera		
	
	ofFbo fboLeft, fboRight; //where we draw image for each different eye

    ofImage overlay;

	ofShader hmdWarpShader;
	float K0,K1,K2,K3,_x,_y,_w,_h,as,DistortionXCenterOffset;

	int dimTimer;
	bool dimmed; 					

	void machine::setup(int setup_type, int camera_type);	
	void machine::initOculus();
	void machine::update();
	void machine::drawVideo();
	void machine::drawOverlay();	
	void machine::calibrate();		
	float* machine::getCalibratedHeadtracking();
	void machine::triggerDim();
	void machine::dim();		
	ofVec2f machine::getDistance();
	void machine::clear();

	machine(void);
	~machine(void);
};

