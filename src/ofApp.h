#pragma once
#include "ofMain.h"
#include "ofxImageSequenceRecorder.h"
#include "soundPlayer.h"
#include "machine.h"

#define HOST "localhost"
#define PORT 8015
//#define LISTEN_PORT 8017
//#define SENDER_PORT 8016
#define HOST "192.168.178.28"
#define CAPTURE_FRAME_RATE 24

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

	machine machine;

    string currentKey;
	
	ofxImageSequenceRecorder recorder;
	bool recording;    

	//sound player controlled by osc
	soundPlayer player;
		
	//send headtracking to other gender swap computer or to pure data
	ofxOscReceiver phoneOscReceiver, receiver;
	ofxOscSender   phoneOscSender, sender;    

};
