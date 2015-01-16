#pragma once
#include "ofxOsc.h"

#define ONE_WAY_SWAP 0
#define TWO_WAY_SWAP 1

class machine
{
public:			
	//0 = one-way swap, 1 = two-way swap
	int type;
	//camera size
	int camWidth, camHeight;
	//used to adjust display position and separation
	int x_offset, y_offset;
	//headtracking received from oculus
	float pitch, yaw, roll;	
	//used to calibrate headtracking values
    float pitch_cal, yaw_cal, roll_cal;	
	//received headtracking
	float rx_pitch, rx_yaw, rx_roll;	
            
    ofVideoGrabber vidGrabber;    	
    ofImage overlay;
	ofFbo fbo;

	ofShader hmdWarpShader;
	float K0,K1,K2,K3,_x,_y,_w,_h,as,DistortionXCenterOffset;

	int dimTimer;
	bool dimmed; 					

	void machine::setup(int type);	
	void machine::update();
	void machine::drawVideo();
	void machine::drawOverlay();	
	void machine::calibrate();		
	float* machine::getCalibratedHeadtracking();
	void machine::triggerDim();
	void machine::dim();		

	ofVec2f machine::getDistance();
	
	machine(void);
	~machine(void);
};

