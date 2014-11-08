#pragma once
#include "ofxOsc.h"

#define OSC_CONTROL_ON 4
#define OSC_CONTROL_OFF 5

#define OSC_CONTROL_STATUS OSC_CONTROL_ON

//oscMessages from and to Phone
#define PHONE_LISTENER_PORT 6999
#define PHONE_SENDER_PORT 8080
#define PHONE_IP "169.254.215.94"

class soundPlayer
{
public:
	int count; //the number of sounds deduced by the input string
	
	ofxOscSender phoneOscSender;

	vector<ofSoundPlayer> sounds; 
	stringstream sound_names;
	vector<int> rxButtons;	
	bool something_is_playing;
	void loadSounds(string s); //list sounds in /data/bin/sounds to be loaded 
	void playSound(int id);	
	void update();

	//soundPlayer();
	//~soundPlayer(void);

	
};
