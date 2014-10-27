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


class ofApp : public ofBaseApp{

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
            
    ofVideoGrabber vidGrabber;    	
    ofTexture tex;

	float K0,K1,K2,K3,_x,_y,_w,_h,as,DistortionXCenterOffset;

	int camWidth, camHeight;
	int x_offset, y_offset;
	float pitch, yaw, roll;
	int layer_offset;
	bool symmetry;
            
    char currentKey;

	ofSoundPlayer sounds[8];
	int rxButtons[8];

	ofxImageSequenceRecorder recorder;
	bool recording;    

	ofShader hmdWarpShader;
};
