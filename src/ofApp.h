#pragma once
#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxImageSequenceRecorder.h"
#include "soundPlayer.h"

#define HOST "localhost"
#define PORT 8015

#define CAPTURE_FRAME_RATE 24

#define LISTEN_PORT 8017
#define SENDER_PORT 8016
#define IP_COMPUTER "10.0.0.4"

class ofApp : public ofBaseApp{

	public:
	void setup();
	void update();
	void draw();
    
	void keyPressed  (int key);	
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	void exit();    
	void clear();
	void output();    
		
	//send headtracking to pure data
	ofxOscSender sender;    

	//send headtracking to other gender swap computer
	ofxOscSender senderComputer;
	ofxOscReceiver receiver;

	float rx_pitch, rx_yaw, rx_roll;
    float rx_pitch_cal, rx_yaw_cal, rx_roll_cal;
    float startTime_Sync;
    float startTime_NoSync;
	float rx_pitch_limited;
    float rx_yaw_limited;
    float endTimer;
            
    ofVideoGrabber vidGrabber;
    
	int camWidth, camHeight;
	int x_offset, y_offset;
	float pitch, yaw, roll;
    float pitch_cal, yaw_cal, roll_cal;
	int layer_offset;
	bool symmetry;
            
    string currentKey;
	
	ofxImageSequenceRecorder recorder;
	bool recording;    

	//sound player controlled by osc
	soundPlayer player;

};
