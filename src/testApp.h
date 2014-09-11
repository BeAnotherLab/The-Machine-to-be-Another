#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxImageSequenceRecorder.h"

#define VERSION_GENDER_SWAP 0
#define VERSION_MACHINE 1

#define CAMERA_OVR 2
#define CAMERA_WEBCAM 3

#define OSC_CONTROL_ON 4
#define OSC_CONTROL_OFF 5

#define OSC_CONTROL_STATUS OSC_CONTROL_OFF

//the version should be configured here
//#define VERSION VERSION_GENDER_SWAP
#define VERSION VERSION_MACHINE

//the camera type should be configured here
//#define CAMERA CAMERA_OVR
#define CAMERA CAMERA_WEBCAM

#define HOST "localhost"
#define PORT 8015

//oscMessages from and to Phone
#define PHONE_LISTENER_PORT 6999
#define PHONE_SENDER_PORT 8080
#define PHONE_IP "169.254.215.94"

#define CAPTURE_FRAME_RATE 24

#if (VERSION == VERSION_GENDER_SWAP)
	# define LISTEN_PORT 8017
	# define SENDER_PORT 8016
	# define IP_COMPUTER "10.0.0.4"
#endif

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

	#if (VERSION == VERSION_GENDER_SWAP)
		ofxOscSender senderComputer;
		ofxOscReceiver receiver;
		float rx_pitch, rx_yaw, rx_roll;
        float rx_pitch_cal, rx_yaw_cal, rx_roll_cal;
        float startTime_Sync;
        float startTime_NoSync;
		float rx_pitch_limited;
        float rx_yaw_limited;
        float endTimer;
    #endif
    
    #if (CAMERA == CAMERA_OVR)                    
        ofTexture left;
        ofTexture right;
    #endif        
    
    #if (CAMERA == CAMERA_WEBCAM)
        ofVideoGrabber vidGrabberLeft, vidGrabberRight; 
    #endif 

	int camWidth, camHeight;
	int x_offset, y_offset;
	float pitch, yaw, roll;
    float pitch_cal, yaw_cal, roll_cal;
	int layer_offset;
	bool symmetry;
    
    
    ofSoundPlayer  sound0, sound1, sound2, sound3, sound4, sound5, sound6, sound7;
    
    string currentKey;
    
	ofxImageSequenceRecorder recorder;
	bool recording;
    
};
