#pragma once
#include "ofMain.h"
#include "soundPlayer.h"
#include "machine.h"
#include "oscController.h"
#include "ofxImageSequenceRecorder.h"

#define COMPUTER 1 //computer 1 is in charge of getting input from touchOSC tablet and relaying it to computer 2
#define CAPTURE_FRAME_RATE 24 //for video recording



class ofApp : public ofBaseApp{
	public:	
	void setup();
	void update();
	void draw();
    
	void keyPressed(int key);		
	void mousePressed(int x, int y, int button);			
	void exit();    
	void clear();		
	void record();	
		
	machine machine;
	
	ofxImageSequenceRecorder recorder;
	oscController controller;
	bool recording;    
	
	soundPlayer player;
};
