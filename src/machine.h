 #pragma once
#include "ofxOsc.h"
#include "OVR.h"
#include "ofxXmlSettings.h"
#include <ovrvision.h>		//Ovrvision SDK
//#include "COculusVR.h" //Oculus SDK

#define ONE_WAY_SWAP 0
#define TWO_WAY_SWAP 1
#define PS3_HORIZONT 0
#define PS3_VERTI 1
#define ON_SERVO_ROLL 1
#define OFF_SERVO_ROLL 0

#define MONO 0 //one webcam
#define STEREO 1 //two webcams
#define OVRVISION 2 //ovrvision

using namespace OVR;
using namespace std;

//DK2 screen size
#define DK2_WIDTH	(1920)
#define DK2_HEIGHT	(1080)

//DK1 screen size
#define DK1_WIDTH	(1280)
#define DK1_HEIGHT	(800)

class machine
{
public:			
	ovrHmd hmd;
	int setup_type; //0 = one-way swap, 1 = two-way swap
	int ps3_position; //0 = vertical, 1 horizontal
	int servo_roll; //0 = no servo, 1 = servo
	int camera_type; //2 for mono, 3 for stereo camera
	int camWidth, camHeight; //camera size
	int ipd;	//used to adjust distance between eyes
	float pitch, yaw, roll; //headtracking received from oculus		
    float pitch_cal, yaw_cal, roll_cal; //used to calibrate headtracking values
	float rx_pitch, rx_yaw, rx_roll; //received headtracking
    float zoom; //used to zoom camera input in/out        
	int speed;
	int alignment;

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

	ofxXmlSettings * settings;
	int swapLR;

	void machine::setup(ofxXmlSettings *settings);	
	void machine::initOculus();
	void machine::update();
	void machine::drawVideo();
	void machine::drawMonitor();
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

