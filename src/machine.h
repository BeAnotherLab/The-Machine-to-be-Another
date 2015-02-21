#pragma once
#include "ofxOsc.h"
#include "OVR.h"
#include <ovrvision.h>		//Ovrvision SDK
//#include "COculusVR.h" //Oculus SDK

#define ONE_WAY_SWAP 0
#define TWO_WAY_SWAP 1
#define MONO 2 //one webcam
#define STEREO 3 //two webcams
#define OVRVISION 4 //ovrvision

using namespace OVR;
using namespace std;

//Oculus Rift screen size
#define RIFTSCREEN_WIDTH	(1920)
#define RIFTPSCREEN_HEIGHT	(1080)

//Application screen size
#define APPSCREEN_WIDTH		(1280)
#define APPSCREEN_HEIGHT	(800)

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
	
	ofTexture left, right; //for using with ovrvision cameras    

	ofFbo fboLeft, fboRight; //where we draw image for each different eye

    ofImage overlay;

	ofShader hmdWarpShader;
	float K0,K1,K2,K3,_x,_y,_w,_h,as,DistortionXCenterOffset;

	int dimTimer;
	bool dimmed; 					

	//ovrvisionstuff
	//Objects
	OVR::Ovrvision* g_pOvrvision;

	void machine::setup(int setup_type, int camera_type);	
	void machine::initOculus();
	void machine::update();
	void machine::drawVideo();
	void machine::drawOverlay();	
	void machine::calibrate();		
	float* machine::getHeadtracking();
	void machine::triggerDim();
	void machine::dim();		
	ofVec2f machine::getDistance();
	void machine::clear();
	void machine::debug();
	void machine::distortion();
	machine(void);
	~machine(void);
};

