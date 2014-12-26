#pragma once
#include "ofMain.h"
#include "ofxImageSequenceRecorder.h"
#include "soundPlayer.h"
#include "machine.h"

#define PORT 8015	
#define HOST "192.168.2.106"

#define COMPUTER 1 //computer 1 is in charge of getting input from touchOSC tablet and relaying it to computer 2
#define CAPTURE_FRAME_RATE 24

class ofApp : public ofBaseApp{
	public:	
	void setup();
	void update();
	void draw();
    
	void keyPressed(int key);		
	void mousePressed(int x, int y, int button);			
	void exit();    
	void clear();	
	void oscRepeat();
	void record();
	void initOculus();
		
	machine machine;
	
	ofxImageSequenceRecorder recorder;
	bool recording;    
	
	soundPlayer player;
};
