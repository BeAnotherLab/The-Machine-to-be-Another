#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxImageSequenceRecorder.h"

#define OSC_CONTROL_ON 4
#define OSC_CONTROL_OFF 5

#define OSC_CONTROL_STATUS OSC_CONTROL_ON

#define HOST "localhost"
#define PORT 8015

//oscMessages from and to Phone
#define PHONE_LISTENER_PORT 6999
#define PHONE_SENDER_PORT 8080
#define PHONE_IP "169.254.215.94"

#define CAPTURE_FRAME_RATE 24

#define LISTEN_PORT 8017
#define SENDER_PORT 8016
#define IP_COMPUTER "10.0.0.4"


class testApp : public ofBaseApp{

	public:
	void setup();
	void update();
	void draw();
    
    void soundPlayer();

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
		
	ofxOscSender sender;
    
    #if (OSC_CONTROL_STATUS == OSC_CONTROL_ON)
        ofxOscReceiver phoneOscReceiver;
        ofxOscSender   phoneOscSender;
    #endif

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
            
    char currentKey;

	ofSoundPlayer sounds[8];
	int rxButtons[8];

	ofxImageSequenceRecorder recorder;
	bool recording;    
};
