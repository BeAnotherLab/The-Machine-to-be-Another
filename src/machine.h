#pragma once
#include "ofxOsc.h"

class machine
{
public:			
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
	
    float time_sync;
    float time_no_sync;
	float rx_pitch_limited;
    float rx_yaw_limited;
    float endTimer;                    
            
    ofVideoGrabber vidGrabber;    	
    
	ofShader hmdWarpShader;
	float K0,K1,K2,K3,_x,_y,_w,_h,as,DistortionXCenterOffset;

	int dimTimer;
	bool dimmed; 

	void machine::setup();
	void machine::update();
	void machine::drawVideo();
	void machine::drawOverlay();	
	void machine::calibrate();		
	void machine::triggerDim();
	void machine::dim();

	machine(void);
	~machine(void);
};

