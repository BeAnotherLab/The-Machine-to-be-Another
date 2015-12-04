 #pragma once
#include "ofxOsc.h"
#include "OVR.h"
#include "videoPlayer.h"
#include "ofxXmlSettings.h"
#include <ovrvision.h>		//Ovrvision SDK
#include "ofxFensterManager.h"

#define ONE_WAY_SWAP 0
#define TWO_WAY_SWAP 1
#define LANDSCAPE 0
#define PORTRAIT 1
#define SERVO_ROLL_ON 1
#define SERVO_ROLL_OFF 0

#define MONO 0 //one webcam
#define STEREO 1 //two webcams
#define OVRVISION 2 //ovrvision

using namespace OVR;
using namespace std;

//HMD type
#define OCULUS_RIFT_DK1 1
#define OCULUS_RIFT_DK2 2

//DK2 screen size
#define DK2_WIDTH	1920
#define DK2_HEIGHT	1080

//DK1 screen size
#define DK1_WIDTH	1280
#define DK1_HEIGHT	800

class machine
{
public:			
	ovrHmd hmd;
	int setup_type; //0 = one-way swap, 1 = two-way swap
	int orientation; //0 = portrait, 1 landscape
    int drift_correction; //to correct user yaw drift
	int servo_roll; //0 = no servo, 1 = servo
	int camera_type; 
	int camWidth, camHeight; //camera size
	int ipd;	//used to adjust distance between eyes
	float pitch, yaw, roll; //headtracking received from oculus		
	float rx_pitch, rx_yaw, rx_roll; //received headtracking from other computer
    float zoom; //used to zoom camera input in/out        
	int speed; //ratio of headtracking angle to image displacement
	int alignment; //to correct vertical alignment for stereo camera
	int swapLR; 
	bool dimmed;	

    ofVideoGrabber vidGrabberLeft, vidGrabberRight; //for using with webcam or PS3 camera		
	
	videoPlayer videoPlayer; //contains videos to be played back

	ofTexture left, right; //for using with ovrvision cameras    

	ofFbo fboLeft, fboRight; //where we draw image for each different eye

    ofImage overlay;

	ofShader hmdWarpShader;
	float K0,K1,K2,K3,_x,_y,_w,_h,as,DistortionXCenterOffset;					

	//ovrvision stuff
	OVR::Ovrvision* g_pOvrvision;

	ofxXmlSettings * settings;	

	//limiting viewing range to [-RANGE, +RANGE]
	#define RANGE 180
	float dimValue;

	void machine::setup(ofxXmlSettings *settings);
	void machine::update();
	void machine::drawInFbo(ofImage * img, ofFbo * fbo);
	void machine::drawVideo();
	void machine::drawMonitor(ofxFenster* window);
	void machine::drawOverlay();	
	void machine::calibrate();		
	float* machine::getHeadtracking();
	void machine::triggerDim();
	void machine::dim(int screen);		
	ofVec2f machine::getDistance();
	void machine::clear();
	void machine::debug();
	void machine::distortion();
	void machine::headTrackingRange();	
	void machine::updateHeadtracking();
	void machine::drawFromCamera();
	void machine::drawFromVideo();

	machine(void);
	~machine(void);
};

