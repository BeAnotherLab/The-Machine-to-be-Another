#pragma once

#include "ofxOsc.h"
#include "machine.h"
#include "soundPlayer.h"
#include "videoPlayer.h"
#include "ofxXmlSettings.h"

class oscController {
public:
	machine* myMachine;
	soundPlayer* mySoundPlayer;
	videoPlayer* myVideoPlayer;
	int computerType;
	ofxOscReceiver receiver; //receive from other computer
	ofxOscSender tabletOscSender; //send to control device
	ofxOscSender sender; //send to other computer or to servos    

	void oscController::setup(machine* machine, soundPlayer* soundplayer, videoPlayer* videoPlayer, ofxXmlSettings * settings);
	void oscController::loop();
	void oscController::oscRepeat(ofxOscMessage rx_msg);
	void oscController::sendSoundPlaying(bool isPlaying, int soundIndex);	
};